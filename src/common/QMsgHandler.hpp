#pragma once

#include <QCoreApplication>
#include <QDebug>
#include <QMessageLogContext>
#include "AsyncLogger.hpp"

using namespace Logger;

/**
 * @brief Пользовательский обработчик сообщений Qt.
 * @param type Тип сообщения (QtDebugMsg, QtWarningMsg и т.д.).
 * @param context Контекст сообщения (файл, строка, функция).
 * @param msg Текст сообщения.
 */
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    Q_UNUSED(context);

    AsyncLogger &logger = AsyncLogger::instance();

    switch (type) {
    case QtDebugMsg:
        logger.logDebug(msg);
        break;
    case QtInfoMsg:
        logger.logInfo(msg);
        break;
    case QtWarningMsg:
        logger.logWarning(msg);
        break;
    case QtCriticalMsg:
        logger.logError(msg);
        break;
    case QtFatalMsg:
        logger.logFatal(msg);
        abort();
    default:
        break;
    }
}
