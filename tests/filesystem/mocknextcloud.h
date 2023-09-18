#pragma once

#include "tests/testhelper/abstractmocknetworkmanager.h"
#include "tests/testhelper/mockfs.h"

class MockNextCloud : public AbstractMockNetworkManager
{
    Q_OBJECT
public:
    explicit MockNextCloud(const QString &host, QObject *parent = nullptr);

private:
    auto sendMockReply(Operation op, const QNetworkRequest &req, const QByteArray &data) -> QNetworkReply * override;
    auto sendDavReply(Operation op, const QNetworkRequest &req, const QString &path, const QByteArray &data)
        -> QNetworkReply *;
    auto sendPropFindReply(const QString &path) -> QNetworkReply *;

    static auto destinationFromHeader(const QNetworkRequest &req) -> QString;

    static auto mockFilesToDResponses(const QString &path, const std::vector<MockFile> &files) -> QString;
    static auto mockFileToDResponse(const QString &path, const MockFile &file) -> QString;
    static auto mockDirsToDResponses(const QString &path, const std::vector<MockDir> &dirs) -> QString;
    static auto mockDirToDResponse(const QString &path, const MockDir &dir) -> QString;

    MockDir m_mockDir;
    bool m_shouldAuthPollReturnSuccess = false;
};
