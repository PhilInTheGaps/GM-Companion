#ifndef GENERATORMANAGER_H
#define GENERATORMANAGER_H

#include <QWidget>

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

    void loadGenerators();
};

#endif // GENERATORMANAGER_H
