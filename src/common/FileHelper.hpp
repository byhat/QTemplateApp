#pragma once

#include <optional>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QMutex>

/**
 * @brief Класс-обёртка для создания файлов для логгера
 * @details Потокобезопасный класс для создания файлов
 * для логгера. Позволяет указывать путь к папке с файлом и
 * имя файла. Если не указано, то прописывает значения по умолчанию.
 * @todo Расширить функционал удалением/переименованием файла
 */
class FileHelper {
public:
    /**
     * @brief Метод создания файла
     * @param dirPath путь к файлу
     * @param fileName имя файла
     * @return Указатель на объект файла
     * @details Если не указано имя и/или путь,
     * используется путь, по которому запущена программа,
     * в имя файла записывает дату и время его создания соответственно
     */
    std::unique_ptr<QFile> createFile(const QString &dirPath  = QString(),
                                      const QString &fileName = QString()) {
        QMutexLocker locker(&m_mutex);

        auto dir = createDir(dirPath);

        if (!dir || !dir.has_value()) {
            qWarning() << __FUNCTION__
                       << "Error: Failed to create or access directory:" << dirPath;
            return nullptr;
        }

        QString name;
        if (fileName.isEmpty()) {
            name = QString("log_%1.log")
                       .arg(QDateTime::currentDateTime()
                                .toString("yyyy-MM-dd_hh:mm:ss"));
        } else {
            name = fileName;
        }

        auto file = std::make_unique<QFile>(dir->filePath(name));

        return file;
    }

private:
    /**
     * @brief Создание директории, если требуется
     * @param dirPath Путь к директории
     * @return Объект класса директории
     */
    std::optional<QDir> createDir(const QString &dirPath) {
        QMutexLocker locker(&m_mutex);
        QString path;

        (dirPath.isEmpty()) ? path = "./" : path = dirPath;

        QDir dir(path);

        if (!dir.exists()) {
            if (!dir.mkpath(".")) {
                qWarning() << __FUNCTION__
                           << "Error: Failed to create directory:" << dirPath;
                return std::nullopt;
            }
        }

        if (!dir.isReadable() || dir.isRoot()) {
            qWarning() << __FUNCTION__
                       << "Error: Insufficient permissions for directory:" << dirPath;
            return std::nullopt;
        }
        return dir;
    }

private:
    QRecursiveMutex m_mutex; //!< Мьютекс для синхронизации доступа
};
