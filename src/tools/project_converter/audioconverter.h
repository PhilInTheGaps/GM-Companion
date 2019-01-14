#ifndef AUDIOCONVERTER_H
#define AUDIOCONVERTER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class AudioConverter : public QObject
{
    Q_OBJECT
public:
    explicit AudioConverter(QObject *parent = nullptr);

    void convert();

private:
    void convertTo3(QString file);
};

#endif // AUDIOCONVERTER_H
