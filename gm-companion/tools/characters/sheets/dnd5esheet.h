#ifndef DND5E_H
#define DND5E_H

#include <QWidget>

namespace Ui {
class DnD5eSheet;
}

class DnD5eSheet : public QWidget
{
    Q_OBJECT

public:
    explicit DnD5eSheet(QWidget *parent = 0);
    ~DnD5eSheet();

    void save(QString filePath);
    void load(QString filePath);

private:
    Ui::DnD5eSheet *ui;
};

#endif // DND5E_H
