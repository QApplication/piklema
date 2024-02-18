#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDateTime>
#include <tuple>


class CustomModel;

class Logger : public QObject
{
    Q_OBJECT
public:
    // msg status
    enum Status : int
    {
        Info = 0,
        Warrning,
        Critical,
        Error
    };
    Q_ENUM(Status)

    explicit Logger(const QString& name, CustomModel *parent);
    ~Logger();

    void add(Status s, const QString& msg);
    const QString &format() const { return mFormat; }
    std::optional<std::tuple<QDateTime,int,QString>> get(const QString& msg) const;


private:
    bool mState;
    QFile mFile;
    QTextStream mStream;
    const QString mFormat;

    QHash<size_t, std::tuple<QDateTime,int,QString>> mSrcData;
    void add(const QString& msg, size_t _hash, const QDateTime& timepoint, int status, const QString& src);

    void parser();      ///< get history log

    Q_DISABLE_COPY_MOVE(Logger)
};

#endif // LOGGER_H
