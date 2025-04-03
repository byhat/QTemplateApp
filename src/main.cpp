#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "AppEngine.hpp"
#include "common/AsyncLogger.hpp"
#include "common/QMsgHandler.hpp"
#include "common/structures.hpp"

// Инициализация статических членов
AsyncLogger* AsyncLogger::m_instance = nullptr;
std::once_flag AsyncLogger::m_onceFlag;


int main(int argc, char *argv[])
{
    QGuiApplication::setOrganizationName("byhat");
    QGuiApplication::setOrganizationDomain("byhat.example");
    QGuiApplication::setApplicationName("QML App Template");
    QGuiApplication::setApplicationVersion("v1.0");

    auto &log = AsyncLogger::instance();

    qInstallMessageHandler(customMessageHandler);
    log.logInfo("Starting app");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    AppEngine appEngine;

    const QUrl url(QStringLiteral("qrc:/AppQml/qml/Main.qml"));

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url, &log](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                log.logFatal(QString("Couldnt found qml file objUrl(%1)").arg(objUrl.toString()));
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("app", &appEngine);

    qmlRegisterUncreatableMetaObject(Logic::staticMetaObject,
                                     "byhat.logic",
                                     1, 0,
                                     "Logic",
                                     "Access to enums & structures");

    engine.load(url);

    return app.exec();
}
