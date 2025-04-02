#pragma once

#include <QObject>

#include "common/structures.hpp"
#include "common/AsyncLogger.hpp"
#include "common/MessagesHandler.hpp"
#include "common/ConfigReader.hpp"


using namespace Logger;
using namespace Logic;

class AppEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool m_Fullscreen MEMBER m_Fullscreen NOTIFY qmlDataUpdate)

public:
    explicit AppEngine(QObject *parent = nullptr);

public slots:
    /**
     * @brief Слот, обрабатывающий событие нажатия кнопки из QML
     * @param btn_id номер кнопки
     */
    void doSomething(uint btn_id);

    /**
     * @brief Метод для запуска работы основной логики
     */
    void start();

    /**
     * @brief Метод для сохранения текущих настроек приложения
     */
    void saveSettings();

signals:
    /**
     * @brief Обновление данных, отображаемых в QML
     */
    void qmlDataUpdate();

    /**
     * @brief Вызов экрана уведомления в GUI
     */
    void qmlMessageUpdate(const QmlDialogMessage &message);

private:
    /**
     * @brief Формат отображения окна приложения,
     * по умолчанию не на весь экран.
     * Ещё один вариант задавать - записывать в реестр
     */
    bool m_Fullscreen = false;

    AsyncLogger    *log;   //!< Логгер
    MessagesHandler m_msg; //!< Обработчик ошибок
    ConfigReader    conf;  //!< Чтение настроек

};
