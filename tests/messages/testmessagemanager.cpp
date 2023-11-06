#include "messages/messagemanager.h"
#include <QFuture>
#include <QTest>
#include <QtConcurrent/QtConcurrent>
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;

TEST(MessageManagerTest, CanAddMessages)
{
    MessageManager::instance()->clearMessages();

    auto message0 = std::make_shared<Message>(QDateTime(), QtMsgType::QtWarningMsg, u"category0"_s, u"message0"_s);
    auto message1 = std::make_shared<Message>(QDateTime(), QtMsgType::QtWarningMsg, u"category1"_s, u"message1"_s);
    auto message2 = std::make_shared<Message>(QDateTime(), QtMsgType::QtWarningMsg, u"category2"_s, u"message2"_s);

    EXPECT_EQ(MessageManager::instance()->messages()->rowCount(), 0);
    EXPECT_FALSE(MessageManager::instance()->hasNewErrors());

    MessageManager::instance()->addMessage(std::move(message0));
    EXPECT_EQ(MessageManager::instance()->messages()->rowCount(), 1);
    EXPECT_TRUE(MessageManager::instance()->hasNewErrors());

    MessageManager::instance()->addMessage(std::move(message1));
    EXPECT_EQ(MessageManager::instance()->messages()->rowCount(), 2);

    MessageManager::instance()->addMessage(std::move(message2));
    EXPECT_EQ(MessageManager::instance()->messages()->rowCount(), 3);

    MessageManager::instance()->clearMessages();
    EXPECT_EQ(MessageManager::instance()->messages()->rowCount(), 0);
    EXPECT_FALSE(MessageManager::instance()->hasNewErrors());

    MessageManager::instance()->addMessage(QDateTime(), QtMsgType::QtWarningMsg, u"category4"_s, u"message4"_s);
    EXPECT_EQ(MessageManager::instance()->messages()->rowCount(), 1);

    MessageManager::instance()->markAllAsRead();
    EXPECT_FALSE(MessageManager::instance()->hasNewErrors());
    EXPECT_EQ(MessageManager::instance()->messages()->rowCount(), 1);
}

TEST(MessageManagerTest, CanAddMessagesFromDifferentThreads)
{
    MessageManager::instance()->clearMessages();

    MessageManager::instance()->addMessage(QDateTime(), QtMsgType::QtWarningMsg, u"category0"_s, u"message0"_s);
    EXPECT_EQ(MessageManager::instance()->messages()->rowCount(), 1);

    QtConcurrent::run(QThreadPool::globalInstance(), []() {
        // via implicit move
        MessageManager::instance()->addMessage(QDateTime(), QtMsgType::QtWarningMsg, u"category1"_s, u"message1"_s);

        // via const ref
        auto ts = QDateTime();
        auto type = QtMsgType::QtWarningMsg;
        auto category = u"category2"_s;
        auto message = u"message2"_s;
        MessageManager::instance()->addMessage(ts, type, category, message);
    }).waitForFinished();

    QTest::qWait(50);

    EXPECT_EQ(MessageManager::instance()->messages()->rowCount(), 3);
}

TEST(MessageManagerTest, CanDistinguishBetweenErrorsAndDebugMessages)
{
    MessageManager::instance()->clearMessages();

    MessageManager::instance()->addMessage(QDateTime(), QtMsgType::QtDebugMsg, u"category0"_s, u"message0"_s);
    EXPECT_FALSE(MessageManager::instance()->hasNewErrors());

    MessageManager::instance()->addMessage(QDateTime(), QtMsgType::QtInfoMsg, u"category1"_s, u"message1"_s);
    EXPECT_FALSE(MessageManager::instance()->hasNewErrors());

    MessageManager::instance()->addMessage(QDateTime(), QtMsgType::QtWarningMsg, u"category2"_s, u"message2"_s);
    EXPECT_TRUE(MessageManager::instance()->hasNewErrors());
    MessageManager::instance()->clearMessages();

    MessageManager::instance()->addMessage(QDateTime(), QtMsgType::QtCriticalMsg, u"category3"_s, u"message3"_s);
    EXPECT_TRUE(MessageManager::instance()->hasNewErrors());
    MessageManager::instance()->clearMessages();
}

TEST(MessageManagerTest, CanGetMessageData)
{
    MessageManager::instance()->clearMessages();

    auto ts = QDateTime();
    MessageManager::instance()->addMessage(ts, QtMsgType::QtDebugMsg, u"category0"_s, u"message0"_s);

    auto index = MessageManager::instance()->messages()->index(0, 0);
    EXPECT_EQ(MessageManager::instance()->messages()->data(index, static_cast<int>(MessageModel::Roles::Timestamp)),
              ts);
    EXPECT_EQ(MessageManager::instance()->messages()->data(index, static_cast<int>(MessageModel::Roles::Type)),
              QtMsgType::QtDebugMsg);
    EXPECT_EQ(MessageManager::instance()
                  ->messages()
                  ->data(index, static_cast<int>(MessageModel::Roles::Category))
                  .toString()
                  .toStdString(),
              u"category0"_s.toStdString());
    EXPECT_EQ(MessageManager::instance()
                  ->messages()
                  ->data(index, static_cast<int>(MessageModel::Roles::Body))
                  .toString()
                  .toStdString(),
              u"message0"_s.toStdString());
}
