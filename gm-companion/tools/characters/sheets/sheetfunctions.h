#ifndef SHEETFUNCTIONS_H
#define SHEETFUNCTIONS_H

#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QCheckBox>

void loadLineEdit(QLineEdit *line, QString value, QSettings *settings);
void loadTextEdit(QTextEdit *text, QString value, QSettings *settings);
void loadTableWidget_horizontal(QTableWidget* table, QString value, QStringList headers, QSettings *settings);
void loadTableWidget_vertical(QTableWidget* table, QString value, QStringList headers, QSettings *settings);
void loadCheckBox(QCheckBox *checkbox, QString value, QSettings *settings);

void saveLineEdit(QLineEdit *line, QString value, QSettings *settings);
void saveTextEdit(QTextEdit *text, QString value, QSettings *settings);
void saveTableWidget_horizontal(QTableWidget* table, QString value, QStringList headers, QSettings *settings);
void saveTableWidget_vertical(QTableWidget* table, QString value, QStringList headers, QSettings *settings);
void saveCheckBox(QCheckBox *checkbox, QString value, QSettings *settings);

#endif // SHEETFUNCTIONS_H
