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

    void foldersFromDir_data();
    void foldersFromDir();
};


#endif // TESTFILEDIALOG_H
