#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <QFrame>

QStringList getCharacterList();

QWidget *getCharacterPage(QString character);

struct TableContent {
    QString leftEntry;
    QString rightEntry;
};

struct TableContent3C {
    QString leftEntry;
    QString midEntry;
    QString rightEntry;
};

enum System{
    Unspecific,
    DSA5
};

#endif // CHARACTERS_H
