#include "projectconverter.h"
#include "shopconverter.h"

#include <QDebug>

ProjectConverter::ProjectConverter(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Project Converter ...";
}

void ProjectConverter::convert()
{
    ShopConverter *shop_converter = new ShopConverter;

    shop_converter->convert();
}
