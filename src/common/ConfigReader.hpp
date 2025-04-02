#pragma once

#include <QObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutex>
#include <QJsonParseError>

#include "structures.hpp"

/**
 * @brief Класс для чтения и сохранения конфигурационных настроек приложения
 * Этот класс предоставляет функциональность для работы с настройками приложения,
 * включая чтение из JSON-файла, сохранение в файл и управление настройками.
 * Реализованы механизмы потокобезопасности и обработки ошибок
 */
class ConfigReader : public QObject
{
    Q_OBJECT

public:
    explicit ConfigReader(QObject *parent = nullptr)
        : QObject(parent) {}

    /**
     * @brief Метод для чтения настроек из файла
     * Читает JSON-файл и десериализует данные в структуры AppSettings и LogicSettings.
     * Если файл не существует или содержит ошибки, эмитируется сигнал errorOccurred
     * @param filePath Путь к файлу настроек
     * @return true, если чтение прошло успешно, иначе false
     */
    bool readSettings(const QString &filePath) {
        QMutexLocker locker(&mutex);

        QFile file(filePath);
        if (!file.exists()) {
            emit errorOccurred(QString("Settings file does not exist: %1").arg(filePath));
            return false;
        }

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            emit errorOccurred(QString("Failed to open settings file: %1").arg(filePath));
            return false;
        }

        QByteArray jsonData = file.readAll();
        file.close();

        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            emit errorOccurred(QString("JSON parsing error: %1").arg(parseError.errorString()));
            return false;
        }

        if (!jsonDoc.isObject()) {
            emit errorOccurred("JSON is not an object");
            return false;
        }

        return deserializeSettings(jsonDoc.object());
    }

    /**
     * @brief Метод для сохранения текущих настроек в файл
     * Сериализует данные из структур AppSettings и LogicSettings в JSON-формат
     * и записывает их в указанный файл. Если файл уже существует, поведение зависит
     * от параметра overwrite
     * @param filePath Путь к файлу настроек
     * @param overwrite Флаг, указывающий, нужно ли перезаписывать существующий файл (по умолчанию true)
     * @return true, если сохранение прошло успешно, иначе false
     */
    bool saveSettings(const QString &filePath, bool overwrite = true) {
        QMutexLocker locker(&mutex);

        QFile file(filePath);
        if (file.exists() && !overwrite) {
            emit errorOccurred(QString("Settings file already exists \nand overwrite is disabled: %1").arg(filePath));
            return false;
        }

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            emit errorOccurred(QString("Failed to open settings \nfile for writing: %1").arg(filePath));
            return false;
        }

        QJsonObject rootObject;

        rootObject["appSettings"] = serializeAppSettings();
        rootObject["logicSettings"] = serializeLogicSettings();

        QJsonDocument jsonDoc(rootObject);

        if (file.write(jsonDoc.toJson()) == -1) {
            emit errorOccurred("Failed to write settings to file");
            return false;
        }

        file.close();

        emit infoMessage(QString("Config was saved"));
        return true;
    }


    /**
     * @brief Получение текущих настроек приложения
     * @return Константная ссылка на структуру AppSettings
     */
    const AppSettings &getAppSettings() const { return appSettings; }

    /**
     * @brief Получение текущих логических настроек
     * @return Константная ссылка на структуру LogicSettings
     */
    const LogicSettings &getLogicSettings() const {
        return logicSettings;
    }

    /**
     * @brief Установка новых настроек приложения
     * Обновляет текущие настройки приложения новыми значениями
     * Операция является потокобезопасной благодаря использованию мьютекса
     * @param newAppSettings Новые настройки приложения
     */
    void setAppSettings(const AppSettings &newAppSettings) {
        QMutexLocker locker(&mutex);
        appSettings = newAppSettings;
    }

    /**
     * @brief Установка новых логических настроек
     * Обновляет текущие логические настройки новыми значениями
     * Операция является потокобезопасной благодаря использованию мьютекса
     * @param newLogicSettings Новые логические настройки
     */
    void setLogicSettings(const LogicSettings &newLogicSettings) {
        QMutexLocker locker(&mutex);
        logicSettings = newLogicSettings;
    }

signals:
    /**
     * @brief Сигнал для уведомления об ошибках
     * @param errorMessage Текстовое описание ошибки
     */
    void errorOccurred(const QString &errorMessage);

    /**
     * @brief Сигнал для уведомления
     * @param msg Текстовое описание сообщения
     */
    void infoMessage(const QString &message);

private:
    AppSettings appSettings;       //!< Настройки приложения
    LogicSettings logicSettings;  //!< Логические настройки

    mutable QRecursiveMutex mutex; //!< Мьютекс для обеспечения потокобезопасности

    /**
     * @brief Вспомогательный метод для сериализации настроек приложения
     * Преобразует структуру AppSettings в JSON-объект
     * @return JSON-объект, содержащий настройки приложения
     */
    QJsonObject serializeAppSettings() const {
        QMutexLocker locker(&mutex);
        QJsonObject appSettingsObject;
        appSettingsObject["fullScreen"] = appSettings.fullScreen;
        appSettingsObject["enableDebugMode"] = appSettings.enableDebugMode;
        return appSettingsObject;
    }

    /**
     * @brief Вспомогательный метод для сериализации логических настроек
     * Преобразует структуру LogicSettings в JSON-объект
     * @return JSON-объект, содержащий логические настройки
     */
    QJsonObject serializeLogicSettings() const {
        QMutexLocker locker(&mutex);
        QJsonObject logicSettingsObject;
        logicSettingsObject["logLvl"] = logicSettings.logLvl;
        return logicSettingsObject;
    }

    /**
     * @brief Вспомогательный метод для десериализации данных из JSON-объекта
     * Загружает данные из JSON-объекта в структуры AppSettings и LogicSettings
     * @param rootObject Корневой JSON-объект, содержащий настройки
     * @return true, если десериализация прошла успешно, иначе false
     */
    bool deserializeSettings(const QJsonObject &rootObject) {
        QMutexLocker locker(&mutex);

        if (rootObject.contains("appSettings") && rootObject["appSettings"].isObject()) {
            appSettings.loadFromJson(rootObject["appSettings"].toObject());
        } else {
            emit errorOccurred("Missing or invalid 'appSettings' in JSON");
            return false;
        }

        if (rootObject.contains("logicSettings") && rootObject["logicSettings"].isObject()) {
            logicSettings.loadFromJson(rootObject["logicSettings"].toObject());
        } else {
            emit errorOccurred("Missing or invalid 'logicSettings' in JSON");
            return false;
        }

        return true;
    }
};
