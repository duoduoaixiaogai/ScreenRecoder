#include "log.h"

Log* Log::mIns = nullptr;

Log::~Log() {
    spdlog::drop("basic_logger");
}

void Log::info(const QString msg) {
    mLogger->info(msg.toLatin1().data());
}

void Log::warn(const QString msg) {
    mLogger->warn(msg.toLatin1().data());
}

Log::Log(QObject *parent) : QObject(parent) {
    try {
        QString logPath = QCoreApplication::applicationDirPath() + QString("/logs/basic-log.txt");
        mLogger = spdlog::basic_logger_mt("basic_logger", logPath.toLatin1().data());
        mLogger->flush_on(spdlog::level::info);
    }
    catch (const spdlog::spdlog_ex &ex) {
        qDebug() << "Log failed: "  << ex.what();
    }
}
