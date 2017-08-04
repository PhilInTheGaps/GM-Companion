#ifndef CHARACTERVIEWERTOOL_H
#define CHARACTERVIEWERTOOL_H

#include <QWidget>

namespace Ui {
class CharacterViewerTool;
}

class CharacterViewerTool : public QWidget
{
    Q_OBJECT

public:
    explicit CharacterViewerTool(QWidget *parent = 0);
    ~CharacterViewerTool();

private:
    Ui::CharacterViewerTool *ui;
};

#endif // CHARACTERVIEWERTOOL_H
