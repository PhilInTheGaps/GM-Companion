#ifndef CHAREDITOR_H
#define CHAREDITOR_H

#include <QMainWindow>
#include <QTableWidget>

namespace Ui {
class CharEditor;
}

class CharEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit CharEditor(QWidget *parent = 0);
    ~CharEditor();

private slots:
    void on_systemComboBox_currentIndexChanged(int index);

    void on_saveButton_clicked();

    void on_cancelButton_clicked();

    void on_addRowSkillsTable1_generic_clicked();

    void on_removeRowSkillsTable1_generic_clicked();

private:
    Ui::CharEditor *ui;

    void save();

    void addRow(QTableWidget* table);
    void removeRow(QTableWidget* table);
};

#endif // CHAREDITOR_H
