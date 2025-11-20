#pragma once

#include <QObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QQueue>
#include <QFuture>
#include <QtConcurrent>
#include <QWaitCondition>
#include <QRecursiveMutex>
#include <QAtomicInteger>
#include <QMutex>
#include <QGlobalStatic>

#include "FileHelper.hpp"

namespace Logger {
/**
 * @brief Класс асинхронной записи в лог
 */
class AsyncLogger : public QObject {
    Q_OBJECT

protected:
    ~AsyncLogger() { stopLogging(); }

public:
    /**
     * @brief Перечисление для уровней логгирования
     * @details Уровни логгирования:
     *  https://stackoverflow.com/questions/2031163/when-to-use-the-different-log-levels
     */
    enum LogLevel {
        Trace,   //!< Наиболее подробный уровень логирования.
        Debug,   //!< Уровень для отладочной информации.
        Info,    //!< Информационные сообщения.
        Warning, //!< Предупреждения о потенциальных проблемах.
        Error,   //!< Сообщения об ошибках.
        Fatal,   //!< Критические ошибки.
        Off      //!< Логирование отключено.
    };
    Q_ENUM(LogLevel)

    /**
     * @brief Метод для получения единственного экземпляра (синглтон)
     * @param logFilePath Путь до файла логов
     * @return Умный указатель на объект класса
     */
    static AsyncLogger &instance(const QString &logFilePath = QString()) {
        std::call_once(m_onceFlag, [&]() {
            m_instance = new AsyncLogger(logFilePath);
        });
        return *m_instance;
    }

    AsyncLogger(const AsyncLogger &) = delete;
    AsyncLogger &operator=(const AsyncLogger &) = delete;

    /**
     * @brief Метод для установки минимального уровня логгирования
     * @param level Уровень логгирования
     * Сообщения ниже заданного уровня будут игнорироваться
     */
    void setLogLevel(LogLevel level) {
        QMutexLocker locker(&m_mutex);
        m_currentLogLevel = level;
    }

    /**
     * @brief Метод для установки минимального уровня логгирования
     * @param level Уровень логгирования
     * Сообщения ниже заданного уровня будут игнорироваться
     */
    void setLogLevel(const QString &level) {
        QMutexLocker locker(&m_mutex);
        if (level == "TRACE")  { m_currentLogLevel = Trace  ; return; }
        if (level == "DEBUG")  { m_currentLogLevel = Debug  ; return; }
        if (level == "INFO")   { m_currentLogLevel = Info   ; return; }
        if (level == "WARNING"){ m_currentLogLevel = Warning; return; }
        if (level == "ERROR")  { m_currentLogLevel = Error  ; return; }
        if (level == "FATAL")  { m_currentLogLevel = Fatal  ; return; }

        emit ErrorOccured( QString("Incorrect log level string: %1").arg(level) );
    }

    /**
     * @brief  Явный метод для остановки логгирования
     */
    void stopLogging() {
        {
            QMutexLocker locker(&m_mutex);
            m_stop.storeRelaxed(true);
        }
        m_condition.wakeAll();
        m_future.waitForFinished();

        if (m_logFile->isOpen()) {
            m_logFile->close();
        }
    }

    /**
     * @brief Метод для логгирования уровня Debug
     * @param message сообщение для логгирования
     */
    void logDebug(const QString &message) {
        QMutexLocker locker(&m_mutex);
        logMessage(Debug, message);
    }

    /**
     * @brief Метод для логгирования уровня Info
     * @param message сообщение для логгирования
     */
    void logInfo(const QString &message) {
        QMutexLocker locker(&m_mutex);
        logMessage(Info, message);
    }

    /**
     * @brief Метод для логгирования уровня Warning
     * @param message сообщение для логгирования
     */
    void logWarning(const QString &message) {
        QMutexLocker locker(&m_mutex);
        logMessage(Warning, message);
    }

    /**
     * @brief Метод для логгирования уровня Error
     * @param message сообщение для логгирования
     */
    void logError(const QString &message) {
        QMutexLocker locker(&m_mutex);
        logMessage(Error, message);
    }

    /**
     * @brief Метод для логгирования уровня Fatal
     * @param message сообщение для логгирования
     */
    void logFatal(const QString &message) {
        QMutexLocker locker(&m_mutex);
        logMessage(Fatal, message);
    }

    /**
     * @brief Метод для логгирования уровня Trace
     * @param message сообщение для логгирования
     */
    void logTrace(const QString &message) {
        QMutexLocker locker(&m_mutex);
        logMessage(Trace, message);
    }

signals:
    /**
     * @brief Сигнал о возникновении ошибки
     * @param message сообщение ошибки
     */
    void ErrorOccured(const QString &message);

private:
    /**
     * @brief Приватный конструктор для предотвращения
     * создания экземпляров вне синглтона
     * @param logFilePath Путь к файлу
     * @details Если путь к файлу не указан, то записывается
     * там, где вызвана программа. Если время сбилось и дата,
     * и время совпали, то записывается в конец существующего
     * файла
     */
    explicit AsyncLogger(const QString &logFilePath = QString(),
                         const QString &logFileName = QString())
        : m_logFilePath(logFilePath),
          m_logFileName(logFileName),
          m_currentLogLevel(Info),
          m_stop(false),
          m_maxFileSize(DEFAULT_FILE_SIZE) {

        FileHelper fhelp;
        m_logFile = fhelp.createFile(m_logFilePath, m_logFileName);
        if (m_logFile.get() == nullptr) {
            qCritical() << __FUNCTION__
                        << "Failed to open log file:"
                        << m_logFilePath
                        << m_logFileName;

            emit ErrorOccured(QString("Failed to open log file: %1 %2")
                                  .arg(m_logFilePath)
                                  .arg(m_logFileName));
        }

        if (!m_logFile->open(QIODevice::WriteOnly |
                             QIODevice::Append |
                             QIODevice::Text)) {
            qWarning() << __FUNCTION__
                       << "Error: Failed to create file:"
                       << m_logFile->fileName();

            emit ErrorOccured(QString("Error: Failed to create file: %1 %2")
                                  .arg(m_logFile->fileName()));
        }

        m_future = QtConcurrent::run([this]() {
            this->processLogQueue();
        });
    }

    /**
     * @brief Внутренний метод для записи лога
     * с указанием уровня
     * @param level Уровень логгирования
     * @param message сообщение для записи в лог
     * @details Пропускает сообщение, если его уровень
     *  ниже установленного.
     */
    void logMessage(LogLevel level, const QString &message) {
        if (level < m_currentLogLevel) {
            return;
        }

        m_logQueue.enqueue(formatMessage(level, message));
        m_condition.wakeOne();
    }

    /**
     * @brief Форматирование сообщения с указанием уровня логгирования
     * @param level Уровень логгирования
     * @param message сообщение для записи в лог
     * @return Форматированное сообщение
     */
    QString formatMessage(LogLevel level, const QString &message) {
        QString levelStr;
        switch (level) {
            case Trace:   levelStr = "TRACE";   break;
            case Debug:   levelStr = "DEBUG";   break;
            case Info:    levelStr = "INFO";    break;
            case Warning: levelStr = "WARNING"; break;
            case Error:   levelStr = "ERROR";   break;
            case Fatal:   levelStr = "FATAL";   break;
        }
        return QString("[%1] [%2] %3")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .arg(levelStr)
            .arg(message);
    }

    /**
     * @brief Обработка очереди логгирования
     * @details Ждёт пока в очереди не появится
     * новое сообщение. Проверяет если размер
     * файла лога больше заданного, то создаёт
     * новый файл и пишет в него
     */
    void processLogQueue() {
        forever {
            QString message;

            {
                QMutexLocker locker(&m_mutex);

                while (m_logQueue.isEmpty() && !m_stop.loadRelaxed()) {
                    m_condition.wait(&m_mutex);
                }

                if (m_stop.loadRelaxed()) {
                    break;
                }

                message = m_logQueue.dequeue();
            }

            if (m_logFile->isOpen() && m_logFile->size() >= m_maxFileSize) {
                rotateLogFile();
            }

            if (m_logFile->isOpen()) {
                QTextStream stream(m_logFile.get());
                stream << message << Qt::endl;
            } else {
                qCritical() << __FUNCTION__
                            << message;
                emit ErrorOccured(message);
            }
        }
    }

    /**
     * @brief Создание нового файла
     */
    void rotateLogFile() {
        FileHelper fhelp;

        m_logFile->close();
        m_logFile = fhelp.createFile(m_logFilePath);

        if (m_logFile.get() == nullptr) {
            qCritical() << __FUNCTION__
                        << "Failed to open log file:" << m_logFilePath;

            emit ErrorOccured(QString("Failed to open log file: %1")
                                  .arg(m_logFilePath));
        }

        if (!m_logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            qWarning() << __FUNCTION__
                       << "Error: Failed to create file:"
                       << m_logFile->fileName();

            emit ErrorOccured(QString("Error: Failed to create file: %1")
                                  .arg(m_logFile->fileName()));
        }
    }

private:
    static constexpr quint64 KILOBYTE = 1024;
    static constexpr quint64 MEGABYTE = 1024 * KILOBYTE;
    static constexpr quint64 GIGABYTE = 1024 * MEGABYTE;
    static constexpr quint64 DEFAULT_FILE_SIZE = 4 * GIGABYTE;

    static AsyncLogger   *m_instance; //!< Единственный экземпляр класса
    static std::once_flag m_onceFlag; //!< Для потокобезопасного создания экземпляра

    std::unique_ptr<QFile> m_logFile;         //!< Файл для логов
    QString                m_logFilePath;     //!< Путь к файлу
    QString                m_logFileName;     //!< Имя файла
    QQueue<QString>        m_logQueue;        //!< Очередь сообщений для записи
    QMutex                 m_mutex;           //!< Обычный мьютекс для синхронизации доступа к очереди
    QWaitCondition         m_condition;       //!< Условие для ожидания новых сообщений
    QAtomicInteger<bool>   m_stop;            //!< Атомарный флаг остановки потока
    QFuture<void>          m_future;          //!< Для асинхронной работы
    LogLevel               m_currentLogLevel; //!< Текущий уровень логгирования
    quint64                m_maxFileSize;     //!< Максимальный размер файла (в байтах)
};
}
