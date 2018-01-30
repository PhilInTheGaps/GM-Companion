#ifndef DEFAULTSHEET_H
#define DEFAULTSHEET_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class DefaultSheet;
}

class DefaultSheet : public QWidget
{
    Q_OBJECT

public:
    explicit DefaultSheet(QWidget *parent = 0);
    ~DefaultSheet();

    void save(QString filePath);
    void load(QString filePath);

private:
    Ui::DefaultSheet *ui;
};

#endif // DEFAULTSHEET_H
