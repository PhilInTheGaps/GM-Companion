#ifndef NAMEGENERATOR_H
#define NAMEGENERATOR_H

#include "gm-companion/managers/settingsmanager.h"

#include <QWidget>
#include <QVBoxLayout>

namespace Ui {
class NameGenerator;
}

class NameGenerator : public QWidget
{
    Q_OBJECT

public:
    explicit NameGenerator(QWidget *parent = 0);
    ~NameGenerator();

private slots:
    void on_spinBox_pointSize_valueChanged(int arg1);

private:
    Ui::NameGenerator *ui;

    void generateNamesTab();
    void generateNameButtons(QString path, QString folder, bool resource = false);
    void addButton(QString path, QString subFolder, QVBoxLayout *layout);
    void generateNames(QString path);

    SettingsManager *settingsManager;
};

#endif // NAMEGENERATOR_H
