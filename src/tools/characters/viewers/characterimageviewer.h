#ifndef CHARACTERIMAGEVIEWER_H
#define CHARACTERIMAGEVIEWER_H

#include "characterviewer.h"
#include <QObject>

#include <poppler/qt5/poppler-qt5.h>

class CharacterImageViewer : public CharacterViewer
{
    Q_OBJECT
    Q_PROPERTY(QString image READ image NOTIFY categoryChanged)
    Q_PROPERTY(int imageIndex READ categoryIndex NOTIFY categoryChanged)
    Q_PROPERTY(QStringList images READ categories NOTIFY categoriesChanged)

public:
    explicit CharacterImageViewer();
    ~CharacterImageViewer();

    void setCharacter(Character* character);

    QString image() const { return m_image; }
    void setCurrentCategory(int index);

    Q_INVOKABLE void nextImage(bool right);

signals:
    void categoryChanged();
    void categoriesChanged();

private:
    Poppler::Document *m_pdfDocument = nullptr;
    QString m_pdfCharacter;
    QString m_image;

    void loadImage(QByteArray data);
    void loadPDF(int index, QByteArray data);
    void loadPDFCategories();
    void setPDFPage(int index);

private slots:
    void onCharacterFileListLoaded(QList<CharacterFile> files);
    void onCharacterFileDataLoaded(int index, QByteArray data);
};

#endif // CHARACTERIMAGEVIEWER_H
