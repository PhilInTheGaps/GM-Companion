#ifndef GOOGLEDRIVE_H
#define GOOGLEDRIVE_H

#include <QObject>

class GoogleDrive : public QObject
{
    Q_OBJECT
public:
    explicit GoogleDrive(QObject *parent = nullptr);

    void authenticate();

    void downloadAll(QString folderID, QString subfolder);

signals:

public slots:

private:
};

#endif // GOOGLEDRIVE_H
