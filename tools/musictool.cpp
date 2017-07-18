#include "musictool.h"
#include "ui_musictool.h"
#include "functions.h"
#include "flowlayout.h"

#include <QStringList>
#include <QDebug>
#include <QScrollArea>
#include <QToolButton>

MusicTool::MusicTool(QWidget *parent) : QWidget(parent), ui(new Ui::MusicTool)
{
    ui->setupUi(this);

//    settingsManager = new SettingsManager;

//    generateButtons();
}

MusicTool::~MusicTool()
{
    delete ui;
}

void MusicTool::generateButtons()
{
    qDebug() << tr("Generating Music Buttons...");

    QStringList musicCategories = getFolders(settingsManager->getSetting(Setting::musicPath)); // List of all categories

    for (QString folder : musicCategories){
        if (!folder.contains(".")){
            QScrollArea *scrollArea = new QScrollArea;
            QFrame *frame = new QFrame;
            QVBoxLayout* frameLayout = new QVBoxLayout;
            frame->setLayout(frameLayout);
            frameLayout->setAlignment(Qt::AlignTop);

            ui->tabWidget_Small->addTab(scrollArea, cleanText(folder));
            scrollArea->setWidget(frame);
            scrollArea->setWidgetResizable(true);

            if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".png").exists()){
                frame->setStyleSheet("QFrame{background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".png);"
                                     "background-position: center top;}");
            }
            else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".jpg").exists()){
                frame->setStyleSheet("QFrame{background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".jpg);"
                                     "background-position: center top;}");
            }

            // Get Groups
            QStringList groups;
            QList<QFrame*> groupFrames;

            QSettings settings(settingsManager->getSetting(Setting::musicPath)+"/"+folder+"/groups.ini", QSettings::IniFormat);
            int groupsSize = settings.beginReadArray("Groups");
            for (int i = 0; i<groupsSize; i++){
                settings.setArrayIndex(i);
                QString group = settings.value("group").toString();
                groups.push_back(group);

                QFrame* f = new QFrame;
                f->setStyleSheet("background: none;");
                FlowLayout* fLayout = new FlowLayout;
                f->setLayout(fLayout);

                QLabel* l = new QLabel(group);
                l->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
                l->setStyleSheet("background: none;");

                frame->layout()->addWidget(l);
                frame->layout()->addWidget(f);

                groupFrames.push_back(f);
            }
            settings.endArray();

            // Generating musicButtons
            QString path = settingsManager->getSetting(Setting::musicPath)+"/"+folder;
            QStringList musicFolders = getFolders(path);

            QLabel* allLabel = new QLabel("General");
            allLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
            allLabel->setStyleSheet("background: none;");
            frame->layout()->addWidget(allLabel);

            QFrame* allFrame = new QFrame;
            allFrame->setStyleSheet("background: none;");
            FlowLayout *flowLayoutMusic = new FlowLayout;
            allFrame->setLayout(flowLayoutMusic);
            frame->layout()->addWidget(allFrame);

            for (QString s : musicFolders){
                if (!s.contains(".")){
                    QToolButton* b = new QToolButton;
//                    connect(b, SIGNAL(clicked()), signalMapperMusic, SLOT(map()));
//                    signalMapperMusic->setMapping(b, settingsManager->getSetting(Setting::musicPath)+"/"+folder+"/"+s);

                    if (settingsManager->getSetting(Setting::buttonStyle) == "small"){

                        b->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                        b->setIconSize(QSize(64,64));
                        b->setMinimumWidth(150);
                        b->setMinimumHeight(90);
                        b->setStyleSheet("background-color: #222222;");

                        if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".png").exists()){
                            QPixmap pixmap(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".png");
                            QIcon icon(pixmap);
                            b->setIcon(icon);
                        }
                        else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".jpg").exists()){
                            QPixmap pixmap(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".jpg");
                            QIcon icon(pixmap);
                            b->setIcon(icon);
                        }


                    }else{
                        if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".png").exists()){
//                            b->setStyleSheet(getStyleSheet(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".png"));
                        }
                        else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".jpg").exists()){
//                            b->setStyleSheet(getStyleSheet(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".jpg"));
                        }
                        else if (QFile(QApplication::applicationDirPath()+"/resources/button.png").exists()){
//                            b->setStyleSheet(getStyleSheet(QApplication::applicationDirPath()+"/resources/button.png"));
                        }
                        else{
                            b->setStyleSheet("QToolButton {"
                                             "min-width: 152; "
                                             "min-height: 152; "
                                             "padding: 1px; "
                                             "border-radius: 2px; "
                                             "background-color: #222222;"
                                             "background-position: center center;}"
                                             "QToolButton QWidget{"
                                             "color: white"
                                             "} ");
                        }

                        b->setFixedSize(152, 152);
                    }


                    QFont font;
                    font.setPixelSize(15);
                    b->setFont(font);

                    QString title = cleanText(s);

                    if (title.length()>20){
                        int i = title.indexOf(" ", 14);
                        title.remove(i, 1);
                        title.insert(i, "\n");
                    }

                    b->setText(title);

                    b->setToolTip(cleanText(s));

                    bool inGroup = false;
                    for (int gIndex = 0; gIndex<groups.size(); gIndex++){
                        int size = settings.beginReadArray(groups.at(gIndex));

                        for (int j = 0; j<size; j++){
                            settings.setArrayIndex(j);
                            QString value = settings.value("folder").toString();

                            if (s == value){
                                inGroup = true;

                                QFrame* gFrame = groupFrames.at(gIndex);
                                gFrame->layout()->addWidget(b);

                                break;
                            }
                        }

                        settings.endArray();
                    }

                    if (!inGroup)
                        flowLayoutMusic->addWidget(b);
                }
            }
        }
    }
}

void MusicTool::on_radioButton_List_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MusicTool::on_radioButton_Large_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MusicTool::on_radioButton_Small_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
