#ifndef AUDIOELEMENTIMAGEPROVER_H
#define AUDIOELEMENTIMAGEPROVER_H

#include <QQuickImageProvider>

class AudioElementImageProvider : public QQuickImageProvider
{
public:
    AudioElementImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    QPixmap getDefaultImage(const QString& type) const;
};

#endif // AUDIOELEMENTIMAGEPROVER_H
