#ifndef DELETECHARACTERDIALOG_H
#define DELETECHARACTERDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class DeleteCharacterDialog;
}

class DeleteCharacterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteCharacterDialog(QWidget *parent = 0);
    ~DeleteCharacterDialog();

    void setCharacterFile(QString);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DeleteCharacterDialog *ui;

    QString characterFile;
};

#endif // DELETECHARACTERDIALOG_H
