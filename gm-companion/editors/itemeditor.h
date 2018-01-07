#ifndef ITEMEDITOR_H
#define ITEMEDITOR_H

#include "gm-companion/managers/settingsmanager.h"

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>

namespace Ui {
class ItemEditor;
}

class ItemEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ItemEditor(QWidget *parent = 0);
    ~ItemEditor();

private slots:
    void on_pushButton_addItem_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_addCategory_clicked();

private:
    Ui::ItemEditor *ui;

    SettingsManager *settingsManager;

    void loadItems();
    void addItemToList(int index, QStringList info);

    void getCategories();

    void removeItem(int index);
};

#endif // ITEMEDITOR_H
