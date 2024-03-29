#pragma once

#include "characterviewer.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <memory>
#include <poppler/qt6/poppler-qt6.h>

class CharacterImageViewer : public CharacterViewer
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QString image READ image NOTIFY categoryChanged)
    Q_PROPERTY(int imageIndex READ categoryIndex NOTIFY categoryChanged)
    Q_PROPERTY(QStringList images READ categories NOTIFY categoriesChanged)

public:
    using CharacterViewer::CharacterViewer;

    void setCharacter(Character *character) override;

    [[nodiscard]] auto image() const -> QString
    {
        return m_image;
    }
    void setCurrentCategory(qsizetype index) override;

    Q_INVOKABLE void nextImage(bool right);

private:
    std::unique_ptr<Poppler::Document> m_pdfDocument;
    QString m_pdfCharacter;
    QString m_image;

    void loadImage(const QByteArray &data);
    void loadPDF(int index, const QByteArray &data);
    void loadPDFCategories();
    void setPDFPage(int index);

    static constexpr double IMAGE_RESOLUTION = 200.0;

private slots:
    void onCharacterFileListLoaded(const QList<CharacterFile *> &files);
    void onCharacterFileDataLoaded(qsizetype index, const QByteArray &data);
};
