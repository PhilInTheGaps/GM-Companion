#ifndef CHARACTERSHEETEDITOR_H
#define CHARACTERSHEETEDITOR_H

#include <QWidget>

namespace Ui {
class CharacterSheetEditor;
}

class CharacterSheetEditor : public QWidget
{
    Q_OBJECT

public:
    explicit CharacterSheetEditor(QWidget *parent = 0);
    ~CharacterSheetEditor();

private:
    Ui::CharacterSheetEditor *ui;
};

#endif // CHARACTERSHEETEDITOR_H
