#pragma once

#include <QQmlEngine>
#include <QObject>
#include <QString>
#include <QJsonObject>


namespace Logic {

    Q_NAMESPACE

    enum MessageType {
        Info,
        Warning,
        Error
    };

    Q_ENUM_NS(MessageType)
}

namespace Logger {

    Q_NAMESPACE

    /**
     * @brief Перечисление для уровней логгирования
     * @details Уровни логгирования:
     *  https://stackoverflow.com/questions/2031163/when-to-use-the-different-log-levels
     */
    enum LogLevel : quint64 {
        Trace,   //!< Наиболее подробный уровень логирования.
        Debug,   //!< Уровень для отладочной информации.
        Info,    //!< Информационные сообщения.
        Warning, //!< Предупреждения о потенциальных проблемах.
        Error,   //!< Сообщения об ошибках.
        Fatal,   //!< Критические ошибки.
        Off      //!< Логирование отключено.
    };
    Q_ENUM_NS(LogLevel)
}

using namespace Logger;
using namespace Logic;

/**
 * @brief Структура для хранения сообщений для QML
 */
struct QmlDialogMessage {
    Q_GADGET

public:
    Q_PROPERTY(QString     text  MEMBER text )
    Q_PROPERTY(MessageType type  MEMBER type )

    QString     text; //!< Текст сообщения
    MessageType type; //!< Тип сообщения

    bool operator == (const QmlDialogMessage &other) const {
        return text == other.text  &&
               type == other.type;
    }

    bool operator != (const QmlDialogMessage &other) const {
        return !(*this == other);
    }
};

Q_DECLARE_METATYPE(QmlDialogMessage)

/**
 * @brief Структура для настроек приложения
 */
struct AppSettings {
    Q_GADGET

public:
    Q_PROPERTY(bool fullScreen      MEMBER fullScreen)
    Q_PROPERTY(bool enableDebugMode MEMBER enableDebugMode)

    bool fullScreen;      //!< Приложение во весь экран или нет
    bool enableDebugMode; //!< Включить режим отладки или нет

    /**
     * @brief Метод для загрузки данных из JSON-объекта
     * @param json Объект с настройками приложения
     */
    void loadFromJson(const QJsonObject &json){
        fullScreen      = json["fullScreen"].toBool();
        enableDebugMode = json["enableDebugMode"].toBool();
    }

    bool operator == (const AppSettings &other) const {
        return fullScreen      == other.fullScreen &&
               enableDebugMode == other.enableDebugMode;
    }

    bool operator != (const AppSettings &other) const {
        return !(*this == other);
    }
};

Q_DECLARE_METATYPE(AppSettings)

/**
 * @brief  Структура для настроек логики
 */
struct LogicSettings {
    Q_GADGET

public:
    Q_PROPERTY(QString logLvl MEMBER logLvl)

    QString logLvl; //!< Параметр для логики

    /**
     * @brief Метод для загрузки данных из JSON-объекта
     * @param json Объект с настройками логики
     */
    void loadFromJson(const QJsonObject &json) {
        logLvl = json["logLvl"].toString();
    }

    bool operator == (const LogicSettings &other) const {
        return logLvl == other.logLvl;
    }

    bool operator != (const LogicSettings &other) const {
        return !(*this == other);
    }
};
