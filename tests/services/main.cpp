#include <QCoreApplication>
#include <QTimer>
#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    QCoreApplication app{argc, argv};

    QTimer::singleShot(0, [&]() {
        auto testResult = RUN_ALL_TESTS();
        QCoreApplication::exit(testResult);
    });

    return QCoreApplication::exec();
}
