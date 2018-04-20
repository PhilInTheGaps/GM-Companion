#include "projectconverter.h"
#include "shopconverter.h"
#include "audioconverter.h"

#include <QDebug>

ProjectConverter::ProjectConverter(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Project Converter ...";
}

void ProjectConverter::convert()
{
    ShopConverter *shop_converter = new ShopConverter;

    shop_converter->convert();

    AudioConverter *audio_converter = new AudioConverter;
    audio_converter->convert();
}
