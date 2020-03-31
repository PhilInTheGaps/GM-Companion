#ifndef UTILS_H
#define UTILS_H

#include <QPixmap>

class Utils
{
public:
    static QString stringFromImage(const QPixmap& image);
    static QString rot13(const QString& input);
};

#endif // UTILS_H
