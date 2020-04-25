#ifndef TESTFILEDIALOG_H
#define TESTFILEDIALOG_H

#include <QObject>

class TestNetworkUtils : public QObject
{
    Q_OBJECT
public:
    TestNetworkUtils() {}

private slots:
    void basicAuthHeader_data();
    void basicAuthHeader();
};


#endif // TESTFILEDIALOG_H
