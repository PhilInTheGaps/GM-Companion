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
    void convertTo4(QString file);
    QJsonObject convertScenarioTo4(QJsonObject scenario);
};

#endif // AUDIOCONVERTER_H
