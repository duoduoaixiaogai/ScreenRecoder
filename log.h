#ifndef LOG_H
#define LOG_H

#include <spdlog/sinks/basic_file_sink.h>

#include <QCoreApplication>
#include <QDebug>


class Log : public QObject {
public:
    ~Log();
    void info(const QString msg);
    void warn(const QString msg);
    static Log* instance() {
        if (!mIns) {
            mIns = new Log;
        }
        return mIns;
    }

    static void delInstance() {
        if (mIns) {
            delete mIns;
            mIns = nullptr;
        }
    }

private:
    explicit Log(QObject *parent = nullptr);
    //variable
    std::shared_ptr<spdlog::logger> mLogger;
    static Log *mIns;
};

#endif // LOG_H
