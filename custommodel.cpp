#include "custommodel.h"
#include <QFile>
#include <QDir>
#include <QModelIndex>

#include <QtMqtt/QMqttTopicName>
#include <QtMqtt/qmqttglobal.h>


CustomModel::CustomModel(QObject *parent)
    : QStringListModel(parent)
    , m_count(-1)
{
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

    // connect(&m_client, &QMqttClient::hostnameChanged, [this] () { qDebug() << m_client.hostname(); });
    // connect(&m_client, &QMqttClient::portChanged, [this] () { qDebug() << m_client.port(); });

    connect(&m_client, &QMqttClient::stateChanged, [this] (QMqttClient::ClientState state) { qDebug() << state; });
}

CustomModel::~CustomModel()
{
    if (m_client.state() == QMqttClient::ClientState::Connected)
        m_client.disconnectFromHost();
}

void CustomModel::run(QString _host, QString _port, QString _username, QString _password, QString _topic, QString _filepath)
{
    // reset of msg count
    m_count = -1;
    emit countChanged();

    // get msg's
    if (!read(_filepath)) {
        emit countChanged();
        return;
    }

    // param connection changed
    if (param_connectionChanged(_host, _port.toUShort(), _username, _password)) {
        if (m_client.state() == QMqttClient::ClientState::Connected) {
            m_client.disconnectFromHost();
        }
        m_client.connectToHost();
        return;
    }

    if (m_client.state() == QMqttClient::ClientState::Connected) {
        send();
        return;
    }


    qDebug() << m_client.hostname() << m_client.port() << m_client.username() << m_client.password();

    m_client.connectToHost();
}

bool CustomModel::read(const QString &absolutePath)
{
    m_filepath = absolutePath;

    m_count = 0;
    msg.clear();

    QFile file(absolutePath);
    if (!file.exists())
        return false;

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QString str = QString(file.readAll());
    if (str.isEmpty())
        return false;

    file.close();

#ifdef Q_OS_WIN
    msg = str.split("\r\n");        // maybe set regexp ?
#endif
#ifdef Q_OS_LINUX
    msg = str.split("\n");
#endif

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

    return ok;
}

void CustomModel::send()        // NOTE: maybe run in another thread ?
{
    m_count = 0;

    for (auto it = msg.cbegin(); it != msg.cend(); ++it) {
        if (m_client.state() == QMqttClient::ClientState::Connected) {

            // to publish
            m_client.publish(QMqttTopicName(topic()), it->toUtf8(), 0, false);

            // to log
            add_msg_log(*it);

            ++m_count;
        }
    }
    msg.clear();
    emit countChanged();
}

void CustomModel::setMqttClientConnectionSettings()
{
    m_client.setHostname(host());
    m_client.setPort(port());

    m_client.setUsername(username());
    m_client.setPassword(password());
}

void CustomModel::add_msg_log(const QString &msg)
{
    if (insertRow(rowCount())) {
        QModelIndex index = this->index(rowCount()-1, 0);
        if (index.isValid()) {
            setData(index, QVariant::fromValue(msg));
        }
    }
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
}

QString CustomModel::getCurrentDateTime() const
{
    return QDateTime::currentDateTime().toString("ddd MMM d yy hh:mm:ss.zzz UTCtt");
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
}
