#ifndef AUDIOEDITOR_H
#define AUDIOEDITOR_H

#include <QWidget>

namespace Ui {
class AudioEditor;
}

class AudioEditor : public QWidget
{
    Q_OBJECT

public:
    explicit AudioEditor(QWidget *parent = 0);
    ~AudioEditor();

private:
    Ui::AudioEditor *ui;
};

#endif // AUDIOEDITOR_H
