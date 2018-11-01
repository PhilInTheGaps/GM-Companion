#ifndef PROJECTCONVERTER_H
#define PROJECTCONVERTER_H

#include <QObject>

class ProjectConverter : public QObject
{
    Q_OBJECT
public:
    explicit ProjectConverter(QObject *parent = nullptr);

    Q_INVOKABLE void convert();

signals:

public slots:
};

#endif // PROJECTCONVERTER_H
