#ifdef _WIN32
// Contains Windows specific functions that don't work on other operating systems
#include "mainwindow.h"
#include "ui_mainwindow.h"

// Creates thumbnail toolbar
void MainWindow::createThumbnailToolbar(){
    QWinThumbnailToolBar* thumbnailToolBar = new QWinThumbnailToolBar(this);
    thumbnailToolBar->setWindow(this->windowHandle());

    playToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    playToolButton->setEnabled(true);
    playToolButton->setToolTip(tr("Music: Play"));
    playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playToolButton, SIGNAL(clicked()), this, SLOT(on_musicPlayButton_clicked()));

    pauseToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    pauseToolButton->setEnabled(true);
    pauseToolButton->setToolTip(tr("Music: Pause"));
    pauseToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    connect(pauseToolButton, SIGNAL(clicked()), this, SLOT(on_musicPauseButton_clicked()));

    nextToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    nextToolButton->setEnabled(true);
    nextToolButton->setToolTip(tr("Music: Next"));
    nextToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    connect(nextToolButton, SIGNAL(clicked()), this, SLOT(on_musicNextButton_clicked()));

    thumbnailToolBar->addButton(playToolButton);
    thumbnailToolBar->addButton(pauseToolButton);
    thumbnailToolBar->addButton(nextToolButton);
}
#endif
