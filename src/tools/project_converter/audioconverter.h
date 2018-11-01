#ifndef AUDIOCONVERTER_H
#define AUDIOCONVERTER_H

#include <QObject>

class AudioConverter : public QObject
{
    Q_OBJECT
public:
    explicit AudioConverter(QObject *parent = nullptr);

    void convert();

signals:

public slots:
};

#endif // AUDIOCONVERTER_H
