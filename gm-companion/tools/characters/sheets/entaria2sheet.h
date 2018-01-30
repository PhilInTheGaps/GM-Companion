#ifndef ENTARIA2SHEET_H
#define ENTARIA2SHEET_H

#include <QWidget>

namespace Ui {
class Entaria2Sheet;
}

class Entaria2Sheet : public QWidget
{
    Q_OBJECT

public:
    explicit Entaria2Sheet(QWidget *parent = 0);
    ~Entaria2Sheet();

    void save(QString filePath);
    void load(QString filePath);

private:
    Ui::Entaria2Sheet *ui;
};

#endif // ENTARIA2SHEET_H
