#ifndef CHARACTERIMAGEVIEWER_H
#define CHARACTERIMAGEVIEWER_H

#include "characterviewer.h"
#include <QObject>

class CharacterImageViewer : public CharacterViewer
{
    Q_OBJECT
    Q_PROPERTY(QString image READ image NOTIFY categoryChanged)
    Q_PROPERTY(int imageIndex READ categoryIndex NOTIFY categoryChanged)
    Q_PROPERTY(QStringList images READ categories NOTIFY categoriesChanged)

public:
    explicit CharacterImageViewer();

    void setCharacter(Character* character);

    QString image() const { return m_image; }
    void setCurrentCategory(int index);

    Q_INVOKABLE void nextImage(bool right);
    void updateImages();

private:
    QString m_image;

    void setPDFImage(int index);
};

#endif // CHARACTERIMAGEVIEWER_H
