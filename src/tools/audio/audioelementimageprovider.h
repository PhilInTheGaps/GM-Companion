#ifndef AUDIOELEMENTIMAGEPROVER_H
#define AUDIOELEMENTIMAGEPROVER_H

#include <QQuickImageProvider>

class AudioElementImageProvider : public QQuickImageProvider
{
public:
    AudioElementImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    static int getElementType(const QString &id);
};

#endif // AUDIOELEMENTIMAGEPROVER_H
