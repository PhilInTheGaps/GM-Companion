#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QWidget>

namespace Ui {
class UpdateDialog;
}

class UpdateDialog : public QWidget
{
    Q_OBJECT

public:
    explicit UpdateDialog(QString title, QString versionString, QWidget *parent = 0);
    ~UpdateDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::UpdateDialog *ui;
};

#endif // UPDATEDIALOG_H
