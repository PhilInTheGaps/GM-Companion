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
    explicit CharacterImageViewer(QObject *parent);
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

    void loadImage(const QByteArray &data);
    void loadPDF(int index, const QByteArray &data);
    void loadPDFCategories();
    void setPDFPage(int index);

    static constexpr double IMAGE_RESOLUTION = 200.0;

private slots:
    void onCharacterFileListLoaded(const QList<CharacterFile *> &files);
    void onCharacterFileDataLoaded(int index, const QByteArray &data);
};

#endif // CHARACTERIMAGEVIEWER_H
