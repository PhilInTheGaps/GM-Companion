#ifndef TESTTOOL_H
#define TESTTOOL_H

#include <QWidget>

namespace Ui {
class TestTool;
}

class TestTool : public QWidget
{
    Q_OBJECT

public:
    explicit TestTool(QWidget *parent = 0);
    ~TestTool();

private:
    Ui::TestTool *ui;
};

#endif // TESTTOOL_H
