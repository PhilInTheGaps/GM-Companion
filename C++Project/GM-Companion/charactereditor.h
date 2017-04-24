#ifndef CHARACTEREDITOR_H
#define CHARACTEREDITOR_H

#include "characters.h"

#include <QObject>
#include <QFrame>
#include <QLayout>
#include <QStackedWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QStringList>
#include <QListWidget>

class CharacterEditor : public QObject
{
    Q_OBJECT
public:
    explicit CharacterEditor(QObject *parent = 0);

    void open();

signals:

public slots:

private slots:
    void systemChanged(int);
    void save();
    void cancel();

    void skills1AddSkill_u();
    void skills2AddSkill_u();
    void skills3AddSkill_u();

    void skills1RemoveSkill_u();
    void skills2RemoveSkill_u();
    void skills3RemoveSkill_u();

    void inv1AddRow_u();
    void inv2AddRow_u();

    void inv1RemRow_u();
    void inv2RemRow_u();

private:
    QFrame* mainFrame;
    QHBoxLayout* mainLayout;
    System systemEnum;
    QStackedWidget* stackedWidget;

    // General Options
    QComboBox* systemComboBox;
    QLineEdit* displayNameEdit;
    QLineEdit* playerEdit;

    // Unspecified System:
    QTableWidget* generalTableWidget_u;
    QStringList generalInfoSettings_u = {"Name: ", "Surname: ", "Species: ", "Profession: ", "Culture: ", "Age: ", "Size: ", "Weight: "};

    QTableWidget* skills1TableWidget_u;
    QStringList skills1Settings_u;

    QTableWidget* skills2TableWidget_u;
    QStringList skills2Settings_u;

    QTableWidget* skills3TableWidget_u;
    QStringList skills3Settings_u;

    QListWidget* inventory1ListWidget_u;
    QListWidget* inventory2ListWidget_u;
};

#endif // CHARACTEREDITOR_H
