#include "AppEngine.hpp"


AppEngine::AppEngine(QObject *parent)
    : QObject{parent} {
    log = &AsyncLogger::instance();


    // Например если нужно выводить на весь экран
    // только в релизной версии на целевом устройстве
#ifdef __ARM_ARCH
#ifndef QT_DEBUG
    m_Fullscreen = true;
#endif
#endif
    connect(&m_msg,  &MessagesHandler::messageReceived,
            this, &AppEngine::qmlMessageUpdate);

    connect(&conf,  &ConfigReader::infoMessage,
            &m_msg, &MessagesHandler::sendInfo);

    connect(log,         &AsyncLogger::ErrorOccured,
            &m_msg, &MessagesHandler::sendError);


    if ( conf.readSettings("config.json") ) {
        log->setLogLevel(conf.getLogicSettings().logLvl);
        m_Fullscreen = conf.getAppSettings().fullScreen;
    } else {
        log->logError("Could not find config file or incorrect file  structure");
        m_msg.sendError("Could not find config file \nor incorrect file structure");
    }
}

void AppEngine::doSomething(uint btn_id)
{
    log->logInfo(QString("Button %1 has been clicked.").arg(btn_id));
}

void AppEngine::start()
{
}

void AppEngine::saveSettings()
{
    AppSettings newAppSettings = conf.getAppSettings();

    newAppSettings.fullScreen = m_Fullscreen;

    conf.setAppSettings(newAppSettings);
    conf.saveSettings("config.json");
}
