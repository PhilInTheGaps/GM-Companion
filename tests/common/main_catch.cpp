#include <QCoreApplication>
#include <QTimer>
#include <catch2/catch_session.hpp>

int main(int argc, char *argv[])
{
    QCoreApplication app{argc, argv};

    QTimer::singleShot(0, [&]() {
        auto testResult = Catch::Session().run(argc, argv);
        QCoreApplication::exit(testResult);
    });

    return QCoreApplication::exec();
}
