#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"
#include "QSettings"
void MainWindow::displayRadios(){
    // Remove old radios
    QLayoutItem *child;
    while ((child = ui->radioDetailsLayout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    ui->radioSelectComboBox->clear();

    QSettings settings(QDir::homePath()+"/.gm-companion/radios.ini", QSettings::IniFormat);
    int size = settings.beginReadArray("Radios");

    for (int i = 0; i<size; i++){
        settings.setArrayIndex(i);

        QString name = settings.value("name", tr("Unnamed Radio")).toString();
        QString url = settings.value("url", tr("Corrupted URL")).toString();
        QString description = settings.value("description", tr("No description given")).toString();

        QFrame* frame = new QFrame;
        QVBoxLayout* frameLayout = new QVBoxLayout;
        frame->setLayout(frameLayout);
        frame->setMaximumHeight(200);

        QLabel* nameLabel = new QLabel(name);
        frameLayout->addWidget(nameLabel);

        QTextEdit* descriptionField = new QTextEdit;
        descriptionField->setReadOnly(true);
        descriptionField->setText(description);
        frameLayout->addWidget(descriptionField);

        ui->radioDetailsLayout->addWidget(frame);

        radioURLs.push_back(url);
        ui->radioSelectComboBox->addItem(name);
    }

    settings.endArray();

    ui->radioSelectComboBox->setCurrentIndex(0);
    ui->radioScrollArea->widget()->layout()->setAlignment(Qt::AlignTop);
}

// Add a Radio
void MainWindow::on_addRadioButton_clicked(){
    QString name = ui->radioNameField->text();
    QString url = ui->radioURLField->text();
    QString description = ui->radioDescriptionField->toPlainText();

    QSettings settings(QDir::homePath()+"/.gm-companion/radios.ini", QSettings::IniFormat);
    int size = settings.beginReadArray("Radios");
    settings.endArray();

    settings.beginWriteArray("Radios");

    settings.setArrayIndex(size);

    settings.setValue("name", name);
    settings.setValue("url", url);
    settings.setValue("description", description);

    settings.endArray();

    // Clear the fields
    ui->radioNameField->clear();
    ui->radioURLField->clear();
    ui->radioDescriptionField->clear();


    displayRadios();
}

// Radio Play Button Clicked
void MainWindow::on_radioPlayButton_clicked(){
    int index = ui->radioSelectComboBox->currentIndex();
    QString name = ui->radioSelectComboBox->currentText();
    QString url = radioURLs.at(index);
    QString description;

    radioPlayer->setMedia(QUrl(url));

    musicPlayer->stop();
    radioActive = true;
    radioPlayer->play();

    ui->musicTitleComboBox->clear();
    ui->musicTitleComboBox->addItem(tr("Radio: ")+name);

    ui->musicCoverLabel->clear();
    ui->musicCoverLabel->setToolTip(description);

    ui->musicNextButton->setDisabled(true);
    ui->musicReplayButton->setDisabled(true);
    ui->musicRandomButton->setDisabled(true);
}

void MainWindow::on_radioMetaDataChanged(){

}
