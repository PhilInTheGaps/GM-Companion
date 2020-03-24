#ifndef TESTSPOTIFY_H
#define TESTSPOTIFY_H

#include <QObject>

class TestSpotify : public QObject
{
    Q_OBJECT
public:
    TestSpotify() {}

private slots:
    void getUriType_data();
    void getUriType();

    void getId_data();
    void getId();
};

#endif // TESTSPOTIFY_H
