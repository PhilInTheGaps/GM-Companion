#ifndef DICETOOL_H
#define DICETOOL_H

#include <QWidget>

namespace Ui {
class DiceTool;
}

class DiceTool : public QWidget
{
    Q_OBJECT

public:
    explicit DiceTool(QWidget *parent = 0);
    ~DiceTool();

private:
    Ui::DiceTool *ui;
};

#endif // DICETOOL_H
