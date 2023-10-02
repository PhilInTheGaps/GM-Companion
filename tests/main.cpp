#include "filesystem/fileaccesslocal.h"
#include <QGuiApplication>
#include <QTimer>
#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    QGuiApplication app{argc, argv};

    Files::FileAccess::setInstance(std::make_shared<Files::FileAccessLocal>());

    QTimer::singleShot(0, &app, [&]() {
        auto testResult = RUN_ALL_TESTS();
        QGuiApplication::exit(testResult);
    });

    return QGuiApplication::exec();
}
