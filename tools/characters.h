#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <QString>
#include <QWidget>

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

struct TableContent4C {
    QString entry1;
    QString entry2;
    QString entry3;
    QString entry4;
};

struct TableContent5C {
    QString entry1;
    QString entry2;
    QString entry3;
    QString entry4;
    QString entry5;
};

struct TableContent6C {
    QString entry1;
    QString entry2;
    QString entry3;
    QString entry4;
    QString entry5;
    QString entry6;
};

struct TableContentUniversal {
    QString entry1;
    QString entry2;
    QString entry3;
    QString entry4;
    QString entry5;
    QString entry6;
    QString entry7;
    QString entry8;
    QString entry9;
    QString entry10;
    QString entry11;
};

enum System{
    Unspecific,
    DSA5
};

#endif // CHARACTERS_H
