#ifndef NAMEGENERATOR_H
#define NAMEGENERATOR_H

#include <QWidget>

namespace Ui {
class NameGenerator;
}

class NameGenerator : public QWidget
{
    Q_OBJECT

public:
    explicit NameGenerator(QWidget *parent = 0);
    ~NameGenerator();

private:
    Ui::NameGenerator *ui;
};

#endif // NAMEGENERATOR_H
