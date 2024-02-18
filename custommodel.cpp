#include "custommodel.h"
#include <QFile>
#include <QDir>
#include <QModelIndex>
#include <QCoreApplication>


#include <QtMqtt/QMqttTopicName>
#include <QtMqtt/qmqttglobal.h>


CustomModel::CustomModel(QObject *parent)
    : QStringListModel(parent)
    , m_count(-1)
    , mLogger(new Logger("logs.txt", this))
{
    mLogger->add(Logger::Info, "Start");

    setDefaultConnectionSettings();
    setMqttClientConnectionSettings();

    connect(&m_client, &QMqttClient::connected, this, &CustomModel::send);

    connect(&m_client, &QMqttClient::disconnected, [this] () {
        qDebug() << QDateTime::currentDateTime() << m_client.state() << m_client.error();
    });

    connect(&m_client, &QMqttClient::messageReceived, [this] (const QByteArray &message, const QMqttTopicName &topic) {
        qDebug() << QDateTime::currentDateTime() << QString(message) << topic.name();
    });

    connect(&m_client, &QMqttClient::messageStatusChanged, [this] (int id, QMqtt::MessageStatus s, const QMqttMessageStatusProperties &properties) {
        qDebug() << QDateTime::currentDateTime() << id << properties.reason() << (quint8)s ;
    });

    connect(&m_client, &QMqttClient::messageSent, [this] (int id) {
        qDebug() << QDateTime::currentDateTime() << id;
    });

    connect(&m_client, &QMqttClient::stateChanged, [this] (QMqttClient::ClientState state) {

        QString state_str("default");
        switch (state) {
        case QMqttClient::ClientState::Connecting:
            state_str = "Connecting";
            break;
        case QMqttClient::ClientState::Connected:
            state_str = "Connected";
            break;
        case QMqttClient::ClientState::Disconnected:
            state_str = "Disconnected";
            break;
        default:
            break;
        }

        mLogger->add(Logger::Info, QString("Client state changed: %1").arg(state_str));
    });
}

CustomModel::~CustomModel()
{

    if (m_client.state() == QMqttClient::ClientState::Connected)
        m_client.disconnectFromHost();

    mLogger->add(Logger::Info, "Closed");
    delete mLogger;
}

void CustomModel::run(QString _host, QString _port, QString _username, QString _password, QString _topic, QString _filepath)
{
    mLogger->add(Logger::Info, "Run buttom is pressed");

    // reset of msg count
    m_count = -1;           // hide msg count label
    emit countChanged();

    // get msg's
    bool is_read = read(_filepath);
    if (!is_read) {
        m_count = 0;
        emit countChanged();
    }

    // param connection changed
    if (param_connectionChanged(_host, _port.toUShort(), _username, _password)) {
        if (m_client.state() == QMqttClient::ClientState::Connected) {
            m_client.disconnectFromHost();
        }
        m_client.connectToHost();
        return;
    }

    if (!is_read)
        return;

    if (m_client.state() == QMqttClient::ClientState::Connected) {
        send();
        return;
    }

    mLogger->add(Logger::Info, "Connecting to host");
    m_client.connectToHost();
}

bool CustomModel::read(const QString &absolutePath)
{
    if (absolutePath.isEmpty()) {
        mLogger->add(Logger::Critical, QString("File path is empty: ") + absolutePath);
        return false;
    }

    mLogger->add(Logger::Info, QString("Reading file: ") + absolutePath);

    m_filepath = absolutePath;

    m_count = 0;
    msg.clear();

    QFile file(absolutePath);
    if (!file.exists()) {
        mLogger->add(Logger::Error, QString("File is not exist: ") + absolutePath);
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        mLogger->add(Logger::Error, QString("File is not opening: ") + absolutePath);
        return false;
    }

    QString str = QString(file.readAll());
    if (str.isEmpty()) {
        mLogger->add(Logger::Warrning, QString("File is empty: %1").arg(absolutePath));
        return false;
    }

    file.close();

#ifdef Q_OS_WIN
    msg = str.split("\r\n", Qt::SkipEmptyParts);        // maybe set regexp ?
#endif
#ifdef Q_OS_LINUX
    msg = str.split("\n", Qt::SkipEmptyParts);
#endif

    if (msg.empty()) {
        mLogger->add(Logger::Warrning, QString("File is not contains of row msg's: ") + absolutePath);
    }

    return !msg.empty();
}

bool CustomModel::param_connectionChanged(const QString &_host, unsigned short _port, const QString &_username, const QString &_password)
{
    // ident check
    bool ok = false;    // save new param to model

    // param's
    if (host() != _host) {
        ok = true;
        m_host = _host;
        m_client.setHostname(host());
    }
    if (port() != _port) {
        ok = true;
        m_port = _port;
        m_client.setPort(port());
    }

    // auth
    if (username() != _username) {
        ok = true;
        m_username = _username;
        m_client.setUsername(username());
    }
    if (password() != _password) {
        ok = true;
        m_password = _password;
        m_client.setPassword(password());
    }

    mLogger->add(Logger::Warrning, QString("Changing of param's: %1:%2 %3 %4").arg(_host).arg(_port).arg(_username).arg(_password));

    return ok;
}

void CustomModel::send()        // NOTE: maybe run in another thread ?
{
    mLogger->add(Logger::Info, QString("Sending begin ..."));

    m_count = 0;

    for (auto it = msg.cbegin(); it != msg.cend(); ++it) {
        if (m_client.state() == QMqttClient::ClientState::Connected) {

            // to publish
            m_client.publish(QMqttTopicName(topic()), it->toUtf8(), 0, false);

            // to log
            mLogger->add(Logger::Info, *it);

            ++m_count;
        }
    }
    msg.clear();

    mLogger->add(Logger::Info, QString("Sending end ... count = %1").arg(m_count));
    emit countChanged();
}

void CustomModel::setMqttClientConnectionSettings()
{
    m_client.setHostname(host());
    m_client.setPort(port());

    m_client.setUsername(username());
    m_client.setPassword(password());
}

void CustomModel::setDefaultConnectionSettings()
{
    m_host = "test.mosquitto.org";
    emit hostChanged();
    m_port = 1883;
    emit portChanged();

    m_username = "";
    emit usernameChanged();
    m_password = "";
    emit passwordChanged();

    m_topic = "piklema/test";
    emit topicChanged();
    m_filepath = "";
    emit filepathChanged();

    mLogger->add(Logger::Info, QString("set Default Connection Settings: %1::%2 %3").arg(m_host).arg(m_port).arg(m_topic));
}

QString CustomModel::getCurrentDateTime() const
{
    return QDateTime::currentDateTime().toString("ddd MMM d yy hh:mm:ss.zzz tt");
}

long long CustomModel::count() const
{
    return m_count;
}

void CustomModel::setCount(long long newCount)
{
    if (m_count == newCount)
        return;
    m_count = newCount;
    emit countChanged();
}

void CustomModel::add(const QString &msg)
{
    if (insertRow(rowCount())) {
        QModelIndex index = this->index(rowCount()-1, 0);
        if (index.isValid()) {
            setData(index, msg);
        }
    }
}

QHash<int, QByteArray> CustomModel::roleNames() const
{
    QHash<int,QByteArray> r = QStringListModel::roleNames();

    // additional roles
    r.insert(LogParam::StatusRole, QString("status").toUtf8());
    r.insert(LogParam::TimeRole, QString("time").toUtf8());
    r.insert(LogParam::MsgRole, QString("msg").toUtf8());

    return r;
}

QVariant CustomModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole: return QStringListModel::data(index, role);

    case LogParam::StatusRole:
    {
        auto v = mLogger->get(QStringListModel::data(index, Qt::DisplayRole).toString());
        if (!v.has_value())
            return QVariant("black");

        switch (std::get<1>(v.value())) {
        case Logger::Info: return "green";
        case Logger::Warrning: return "orange";
        case Logger::Critical: return "magenta";
        case Logger::Error: return "red";
        default: return "transparent";
        }
    }
    break;

    case LogParam::TimeRole:
    {
        auto v = mLogger->get(QStringListModel::data(index, Qt::DisplayRole).toString());
        return v.has_value()
                   ? std::get<0>(v.value()).toString(mLogger->format())
                   : QVariant("");
    }

    case LogParam::MsgRole:
    {
        auto v = mLogger->get(QStringListModel::data(index, Qt::DisplayRole).toString());
        return v.has_value()
                   ? std::get<2>(v.value())
                   : QVariant(Logger::Error);
    }

    default: return QVariant();
    }

}

QString CustomModel::host() const
{
    return m_host;
}

void CustomModel::setHost(const QString &newHost)
{
    if (m_host == newHost)
        return;
    m_host = newHost;
    emit hostChanged();

    mLogger->add(Logger::Info, QString("set Host: %1").arg(m_host));
}

unsigned short CustomModel::port() const
{
    return m_port;
}

void CustomModel::setPort(unsigned short newPort)
{
    if (m_port == newPort)
        return;
    m_port = newPort;
    emit portChanged();

    mLogger->add(Logger::Info, QString("set Port: %1").arg(QString::number(m_port)));
}

QString CustomModel::username() const
{
    return m_username;
}

void CustomModel::setUsername(const QString &newUsername)
{
    if (m_username == newUsername)
        return;
    m_username = newUsername;
    emit usernameChanged();

    mLogger->add(Logger::Info, QString("set Username: %1").arg(m_username));
}

QString CustomModel::password() const
{
    return m_password;
}

void CustomModel::setPassword(const QString &newPassword)
{
    if (m_password == newPassword)
        return;
    m_password = newPassword;
    emit passwordChanged();

    mLogger->add(Logger::Info, QString("set Password: %1").arg(m_password));
}

QString CustomModel::topic() const
{
    return m_topic;
}

void CustomModel::setTopic(const QString &newTopic)
{
    if (m_topic == newTopic)
        return;
    m_topic = newTopic;
    emit topicChanged();

    mLogger->add(Logger::Info, QString("set Topic: %1").arg(m_topic));
}

QString CustomModel::filepath() const
{
    return m_filepath;
}

void CustomModel::setFilepath(const QString &newFilepath)
{
    if (m_filepath == newFilepath)
        return;
    m_filepath = newFilepath;
    emit filepathChanged();

    mLogger->add(Logger::Info, QString("set absoluteFilePath: %1").arg(m_filepath));
}
