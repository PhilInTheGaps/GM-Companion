#ifndef TESTFILEDIALOG_H
#define TESTFILEDIALOG_H

#include <QObject>

class TestUtils : public QObject
{
    Q_OBJECT
public:
    TestUtils() {}

private slots:
    void rot13_data();
    void rot13();

    void hasWildcardMatch_data();
    void hasWildcardMatch();
};


#endif // TESTFILEDIALOG_H
