#ifndef CHARACTERPAGE_H
#define CHARACTERPAGE_H

#include <QStackedWidget>
#include <QStringList>
#include "characters.h"
#include <QList>

namespace Ui {
class CharacterPage;
}

class CharacterPage : public QStackedWidget
{
    Q_OBJECT

public:
    explicit CharacterPage(QWidget *parent = 0);
    ~CharacterPage();

    void updateUi();

    int systemID = 0;

    QList<TableContent>* generalInfo_generic;

    QList<TableContent>* skills1_generic;
    QList<TableContent>* skills2_generic;
    QList<TableContent>* skills3_generic;

    QList<TableContent3C>* weapons_generic;
    QList<TableContent3C>* armor_generic;

    QList<TableContent3C>* inv1_generic;
    QList<TableContent3C>* inv2_generic;

private:
    Ui::CharacterPage *ui;
};

#endif // CHARACTERPAGE_H
