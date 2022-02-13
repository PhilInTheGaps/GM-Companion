#pragma once

#include "service.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>

class Discord : public Service
{
    Q_OBJECT
    AUTO_PROPERTY(QString, channel)
    AUTO_PROPERTY(bool, enabled)
public:
    static Discord *getInstance();

    void post(const QString &endpoint, const QByteArray &data);
    void post(const QString &endpoint, QHttpMultiPart *multipart);

public slots:
    void connectService() override;
    void disconnectService() override;
    void testConnection();
    void invite();

private:
    explicit Discord(QObject *parent = nullptr);

    inline static Discord *single = nullptr;

    static constexpr const char* API_ENDPOINT = "/discord";

    QNetworkAccessManager *m_networkManager = nullptr;

    QString m_channel;
    bool m_enabled = false;

    [[nodiscard]] static auto serverUrl() -> QString;
};
