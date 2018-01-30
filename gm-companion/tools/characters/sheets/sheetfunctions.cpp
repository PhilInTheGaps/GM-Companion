#include "sheetfunctions.h"
#include <QHeaderView>

// Load value to line edit
void loadLineEdit(QLineEdit *line, QString value, QSettings *settings)
{
    QString s = settings->value(value, "").toString();

    line->setText(s);
    line->setToolTip(s);
}

// Load value to text edit
void loadTextEdit(QTextEdit *text, QString value, QSettings *settings)
{
    text->setText(settings->value(value, "").toString());
}

// Load values to table widget, horizontally
// Example:
// Header 1:    1   |   2   |   3   |
// Header 2:    A   |   B   |   C   |
void loadTableWidget_horizontal(QTableWidget *table, QString value, QStringList headers, QSettings *settings)
{
    int size = settings->beginReadArray(value);

    for (int i = 0; i < size; i++)
    {
        settings->setArrayIndex(i);

        for (int j = 0; j < headers.size(); j++)
        {
            QString s              = settings->value(headers.at(j), "").toString();
            QTableWidgetItem *item = new QTableWidgetItem(s);
            item->setToolTip(s);
            table->setItem(j, i, item);
        }
    }

    settings->endArray();

    table->resizeRowsToContents();
}

// Load values to table widget, vertically
// Example:
// Header 1 |   Header 2
//      1   |       2
//      A   |       B
void loadTableWidget_vertical(QTableWidget *table, QString value, QStringList headers, QSettings *settings)
{
    int size = settings->beginReadArray(value);

    for (int i = 0; i < size; i++)
    {
        settings->setArrayIndex(i);

        for (int j = 0; j < headers.size(); j++)
        {
            QString s              = settings->value(headers.at(j), "").toString();
            QTableWidgetItem *item = new QTableWidgetItem(s);
            item->setToolTip(s);
            table->setItem(i, j, item);
        }
    }

    settings->endArray();

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

// Save line edit value
void saveLineEdit(QLineEdit *line, QString value, QSettings *settings)
{
    settings->setValue(value, line->text());
}

// Save text edit value
void saveTextEdit(QTextEdit *text, QString value, QSettings *settings)
{
    settings->setValue(value, text->toPlainText());
}

// Save table values, horizontally
void saveTableWidget_horizontal(QTableWidget *table, QString value, QStringList headers, QSettings *settings)
{
    settings->beginWriteArray(value);

    for (int i = 0; i < table->columnCount(); i++)
    {
        settings->setArrayIndex(i);

        for (int j = 0; j < headers.size(); j++)
        {
            QTableWidgetItem *item = table->item(j, i);

            if (item != NULL)
            {
                settings->setValue(headers.at(j), item->text());
            }
            else
            {
                settings->setValue(headers.at(j), "");
            }
        }
    }

    settings->endArray();
}

// Save table values, vertically
void saveTableWidget_vertical(QTableWidget *table, QString value, QStringList headers, QSettings *settings)
{
    settings->beginWriteArray(value);

    for (int i = 0; i < table->rowCount(); i++)
    {
        settings->setArrayIndex(i);

        for (int j = 0; j < headers.size(); j++)
        {
            QTableWidgetItem *item = table->item(i, j);

            if (item != NULL)
            {
                settings->setValue(headers.at(j), item->text());
            }
            else
            {
                settings->setValue(headers.at(j), "");
            }
        }
    }

    settings->endArray();
}
