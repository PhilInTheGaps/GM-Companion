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

    void getIdFromUri_data();
    void getIdFromUri();

    void getIdFromHref_data();
    void getIdFromHref();
};

#endif // TESTSPOTIFY_H
