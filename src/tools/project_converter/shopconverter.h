#ifndef SHOPCONVERTER_H
#define SHOPCONVERTER_H

#include <QObject>

class ShopConverter : public QObject
{
    Q_OBJECT
public:
    explicit ShopConverter(QObject *parent = nullptr);

    void convert();

signals:

public slots:
};

#endif // SHOPCONVERTER_H
