#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <QFrame>

QStringList getCharacterList();

QFrame *getCharacterPage(QString character);

enum System{
    Unspecific,
    DSA5
};

#endif // CHARACTERS_H
