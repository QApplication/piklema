#include "logger.h"
#include <QCoreApplication>
#include <QDir>
#include <QDateTime>

#include "custommodel.h"
#include <algorithm>
#include <string>


Logger::Logger(const QString& name, CustomModel *parent)
    : QObject{parent}
    , mState(false)
    , mFormat("ddd yyyy-MMM-dd HH:mm:ss.zzz")
{
    if (!parent)
        return;

    // log file
    mFile.setFileName(QCoreApplication::instance()->applicationDirPath() + QDir::separator() + name);

    // history log
    parser();

    // new log
    mState = mFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    if (mState)
        mStream.setDevice(&mFile);
}

Logger::~Logger()
{
    if (!mState)
        return;

    mStream.flush();
    mFile.close();
}

void Logger::add(Status s, const QString &msg)
{
    if (!mState)
        return;

    // to view log
    QString str; str.reserve(200);
    QTextStream sout(&str);

    QDateTime timepoint = QDateTime::currentDateTime();

    sout << qSetFieldWidth(35) << Qt::left << timepoint.toString(format())
         << qSetFieldWidth(5) << Qt::center << static_cast<int>(s)
         << qSetFieldWidth(0) << Qt::left << msg << Qt::endl;
    sout.flush();

    add(str, std::hash<std::string>{}(str.toStdString()), timepoint, s, msg);

    // to file log
    mStream << str;
    mStream.flush();
}

void Logger::add(const QString &msg, size_t _hash, const QDateTime &timepoint, int status, const QString &src)
{
    mSrcData[_hash] = std::make_tuple(timepoint, status, src);  // to hash data
    static_cast<CustomModel*>(parent())->add(msg);              // to model data
}

std::optional<std::tuple<QDateTime,int,QString>> Logger::get(const QString &msg) const
{
    size_t _hash = std::hash<std::string>{}(msg.toStdString());
    if (!mSrcData.contains(_hash)) {
        return std::optional<std::tuple<QDateTime,int,QString>>();
    }
    return mSrcData[_hash];
}

void Logger::parser()
{
    if (!mFile.open(QIODevice::ReadOnly))
        return;

    auto msg_hist = QString(mFile.readAll()).split(
#ifdef Q_OS_WIN
        "\r\n"
#endif
#ifdef Q_OS_LINUX
        "\n"
#endif
        , Qt::SkipEmptyParts);

    for (auto const& r : msg_hist) {
        auto it = r.cbegin();

        QString _datetime;
        std::copy_n(it, 28, std::back_inserter(_datetime));
        QDateTime tp = QDateTime::fromString(_datetime, format());
        if (!tp.isValid())
            continue;

        QString _status;
        std::copy_n(std::next(it,37), 1, std::back_inserter(_status));

        QString _msg;
        std::copy_n(std::next(it,40), r.size()-40, std::back_inserter(_msg));

        const size_t _hash = std::hash<std::string>{}(r.toStdString());
        add(r, _hash, tp, _status.toInt(), _msg);
    }
    mFile.close();
}
