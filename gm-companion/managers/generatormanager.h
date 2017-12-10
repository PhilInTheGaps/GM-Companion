#ifndef GENERATORMANAGER_H
#define GENERATORMANAGER_H

#include <QWidget>

#include "managers/settingsmanager.h"

namespace Ui {
class GeneratorManager;
}

class GeneratorManager : public QWidget
{
    Q_OBJECT

public:
    explicit GeneratorManager(QWidget *parent = 0);
    ~GeneratorManager();

private:
    Ui::GeneratorManager *ui;

    SettingsManager *settingsManager;

    void loadGenerators();
};

#endif // GENERATORMANAGER_H
