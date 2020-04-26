#ifndef TESTFILEDIALOG_H
#define TESTFILEDIALOG_H

#include <QObject>

class TestFileUtils : public QObject
{
    Q_OBJECT
public:
    TestFileUtils() {}

private slots:
    void dirFromFolders_data();
    void dirFromFolders();

    void dirFromPath_data();
    void dirFromPath();

    void foldersFromDir_data();
    void foldersFromDir();

    void suffix_data();
    void suffix();

    void fileName_data();
    void fileName();

    void incrementFileName_data();
    void incrementFileName();
};


#endif // TESTFILEDIALOG_H
