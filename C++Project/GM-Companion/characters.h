#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <QFrame>

QStringList getCharacterList();

QFrame *getCharacterPage(QString character);

enum System{
    DSA5, Unknown
};

#endif // CHARACTERS_H
