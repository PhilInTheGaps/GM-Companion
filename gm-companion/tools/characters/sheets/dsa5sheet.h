#ifndef DSA5SHEET_H
#define DSA5SHEET_H

#include <QWidget>

namespace Ui {
class DSA5Sheet;
}

class DSA5Sheet : public QWidget
{
    Q_OBJECT

public:
    explicit DSA5Sheet(QWidget *parent = 0);
    ~DSA5Sheet();

    void save(QString filePath);
    void load(QString filePath);

private:
    Ui::DSA5Sheet *ui;
};

#endif // DSA5SHEET_H
