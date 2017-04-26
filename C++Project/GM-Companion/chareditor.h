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

    void on_addRowSkillsTable2_generic_clicked();

    void on_removeRowSkillsTable2_generic_clicked();

    void on_addRowSkillsTable3_generic_clicked();

    void on_removeRowSkillsTable3_generic_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_14_clicked();

    void on_iconButton_clicked();

private:
    Ui::CharEditor *ui;

    void save();

    void addRow(QTableWidget* table);
    void removeRow(QTableWidget* table);
};

#endif // CHAREDITOR_H
