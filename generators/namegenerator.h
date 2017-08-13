#ifndef NAMEGENERATOR_H
#define NAMEGENERATOR_H

#include "managers/settingsmanager.h"

#include <QWidget>
#include <QSignalMapper>

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
    void on_generateNames(QString file);

    void on_spinBox_pointSize_valueChanged(int arg1);

private:
    Ui::NameGenerator *ui;

    void generateNamesTab();

    QSignalMapper *signalMapper;

    SettingsManager *settingsManager;
};

#endif // NAMEGENERATOR_H
