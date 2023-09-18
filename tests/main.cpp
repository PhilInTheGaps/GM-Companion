#include <QGuiApplication>
#include <QTimer>
#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    QGuiApplication app{argc, argv};

    QTimer::singleShot(0, &app, [&]() {
        auto testResult = RUN_ALL_TESTS();
        QGuiApplication::exit(testResult);
    });

    return QGuiApplication::exec();
}
