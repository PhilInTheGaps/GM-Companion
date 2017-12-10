#ifndef NOTESTOOL_H
#define NOTESTOOL_H

#include <QWidget>
#include <QTreeWidgetItem>

#include "managers/settingsmanager.h"

namespace Ui {
class NotesTool;
}

class NotesTool : public QWidget
{
    Q_OBJECT

public:
    explicit NotesTool(QWidget *parent = 0);
    ~NotesTool();

    void getNotes();

private slots:
    void on_pushButton_encrypt_clicked();

    void on_treeWidget_notes_itemClicked(QTreeWidgetItem *item, int column);

    void on_textEdit_notes_textChanged();

    void on_pushButton_addNote_clicked();

    void on_pushButton_addCategory_clicked();

    void on_pushButton_deleteNote_clicked();

    void on_pushButton_deleteCategory_clicked();

    void on_fontComboBox_currentTextChanged(const QString &arg1);

    void on_spinBox_fontSize_valueChanged(int arg1);

    void on_pushButton_update_clicked();

private:
    Ui::NotesTool *ui;

    void readNotes(QString file);

    void addNotes();
    void addCategory();

    void deleteNotes();
    void deleteCategory();

    void updateText();

    SettingsManager *settingsManager;

    QString currentCategory;
};

#endif // NOTESTOOL_H
