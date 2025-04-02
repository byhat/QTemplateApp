#pragma once

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QThread>
#include <QtConcurrent>
#include <QWaitCondition>

#include "structures.hpp"


using namespace Logic;

/**
 * @brief Обработка сообщений для их отображения в QML
 */
class MessagesHandler : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Перечисление для уровней сообщения
     */
    enum MsgLevel {
        Debug,   //!< Уровень для отладочной информации.
        Warning, //!< Предупреждения о потенциальных проблемах.
        Error,   //!< Сообщения об ошибках.
        Off      //!< Логирование отключено.
    };
    Q_ENUM(MsgLevel)

    explicit MessagesHandler(QObject *parent = nullptr)
        : QObject(parent),
        m_stop(false),
        m_currentMsgLvl(Error) {

        this->moveToThread(&m_workerThread);

        connect(&m_workerThread, &QThread::started, this, [this](){
            m_future = QtConcurrent::run([this]() {
                this->processMessages();
            });
        });

        connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, [&](){
            this->~MessagesHandler();
        });

        m_workerThread.start();
    }

    ~MessagesHandler()
    {
        {
            QMutexLocker locker(&m_mutex);
            m_stop.storeRelaxed(true);
            m_condition.wakeAll();
        }

        m_workerThread.quit();
        m_workerThread.wait();
    }

public slots:
    /**
     * @brief  Явный метод для остановки обработки сообщений
     */
    void stopProcessing() {
        {
            QMutexLocker locker(&m_mutex);
            m_stop.storeRelaxed(true);
        }
        m_condition.wakeAll();
        m_future.waitForFinished();
    }
    /**
     * @brief Метод для установки минимального уровня сообщения
     * @param level Уровень сообщения
     * Сообщения ниже заданного уровня будут игнорироваться
     */
    void setLogLevel(MsgLevel level) {
        QMutexLocker locker(&m_mutex);
        m_currentMsgLvl = level;
    }

    /**
     * @brief Метод для отправки сообщения об ошибке
     * @param message сообщение об ошибке
     */
    void sendError(const QString &message) {
        QmlDialogMessage dialogMessage { message, MessageType::Error };
        enqueueMessage(dialogMessage);
    }

    /**
     * @brief Метод для отправки сообщения предупреждения
     * @param message сообщение предупреждения
     */
    void sendWarning(const QString &message) {
        QmlDialogMessage dialogMessage { message, MessageType::Warning };
        enqueueMessage(dialogMessage);
    }

    /**
     * @brief Метод для отправки информационного сообщения
     * @param message информационное сообщение
     */
    void sendInfo(const QString &message) {
        QmlDialogMessage dialogMessage { message, MessageType::Info };
        enqueueMessage(dialogMessage);
    }

signals:
    /**
     * @brief Сигнал для передачи сообщения в QML
     * @param message сообщение для отображения в диалоговом окне
     */
    void messageReceived(const QmlDialogMessage &message);

private:
    /**
     * @brief Слот для обработки сообщений
     */
    void processMessages() {
        forever {
            QmlDialogMessage message;

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

            emit messageReceived(message);
        }
    }

private:
    /**
     * @brief Метод добавления сообщения в очередь
     * @param message сообщение, добавляемое в очередь
     */
    void enqueueMessage(const QmlDialogMessage &message) {
        m_logQueue.enqueue(message);
        m_condition.wakeOne();
    }

    QThread                  m_workerThread;  //!< Поток для обработки сообщений
    QQueue<QmlDialogMessage> m_logQueue;      //!< Очередь сообщений
    QMutex                   m_mutex;         //!< Мьютекс для синхронизации доступа к очереди
    QWaitCondition           m_condition;     //!< Условная переменная для ожидания новых сообщений
    QAtomicInteger<bool>     m_stop;          //!< Флаг для остановки обработки
    MsgLevel                 m_currentMsgLvl; //!< Текущий уровень логгирования
    QFuture<void>            m_future;        //!< Для асинхронной работы
};

