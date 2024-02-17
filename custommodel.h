#ifndef CUSTOMMODEL_H
#define CUSTOMMODEL_H

#include <QStringListModel>

#include <QtMqtt/QMqttClient>


class CustomModel : public QStringListModel
{
    Q_OBJECT

    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged FINAL)
    Q_PROPERTY(unsigned short port READ port WRITE setPort NOTIFY portChanged FINAL)

    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged FINAL)

    Q_PROPERTY(QString topic READ topic WRITE setTopic NOTIFY topicChanged FINAL)
    Q_PROPERTY(QString filepath READ filepath WRITE setFilepath NOTIFY filepathChanged FINAL)

    Q_PROPERTY(long long count READ count WRITE setCount NOTIFY countChanged FINAL)


public:
    explicit CustomModel(QObject *parent = nullptr);
    ~CustomModel();

    Q_INVOKABLE void run(QString _host, QString _port, QString _username, QString _password, QString _topic, QString _filepath);
    Q_INVOKABLE void setDefaultConnectionSettings();
    Q_INVOKABLE QString getCurrentDateTime() const;

    Q_INVOKABLE QString host() const;
    Q_INVOKABLE void setHost(const QString &newHost);

    Q_INVOKABLE unsigned short port() const;
    Q_INVOKABLE void setPort(unsigned short newPort);

    Q_INVOKABLE QString username() const;
    Q_INVOKABLE void setUsername(const QString &newUsername);

    Q_INVOKABLE QString password() const;
    Q_INVOKABLE void setPassword(const QString &newPassword);

    Q_INVOKABLE QString topic() const;
    Q_INVOKABLE void setTopic(const QString &newTopic);

    Q_INVOKABLE QString filepath() const;
    Q_INVOKABLE void setFilepath(const QString &newFilepath);

    long long count() const;
    void setCount(long long newCount);


signals:
    void hostChanged();
    void portChanged();
    void usernameChanged();
    void passwordChanged();
    void topicChanged();
    void filepathChanged();
    void countChanged();

private:
    // member's
    QString m_host;
    unsigned short m_port;
    QString m_username;
    QString m_password;
    QString m_topic;
    QString m_filepath;

    bool read(const QString& absolutePath);     ///< file with messages
    long long m_count;
    QStringList msg;

    bool param_connectionChanged(const QString &_host, unsigned short _port, const QString &_username, const QString &_password);
    void send();

    QMqttClient m_client;
    void setMqttClientConnectionSettings();

    std::unordered_map<QString, QString> default_conn_settings;

    void add_msg_log(const QString& msg);

};


#endif // CUSTOMMODEL_H
