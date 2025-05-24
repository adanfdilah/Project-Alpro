/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_Audio_File;
    QWidget *centralwidget;
    QLabel *label_File_Name;
    QSlider *horizontalSlider_Audio_File_Duration;
    QSlider *horizontalSlider_Audio_Volume;
    QPushButton *pushButton_Volume;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_Seek_Back;
    QPushButton *pushButton_Stop;
    QPushButton *pushButton_Play;
    QPushButton *pushButton_Pause;
    QPushButton *pushButton_Seek_Forward;
    QLabel *label_Value_1;
    QLabel *label_Value_2;
    QPushButton *setTimerButton;
    QSpinBox *timerSpinBox;
    QListWidget *listWidget;
    QPushButton *pushButton_Shuffle;
    QPushButton *pushButton_Repeat;
    QTextEdit *lyricsDisplay;
    QMenuBar *menubar;
    QMenu *menuOpen;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(870, 600);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color:rgb(52,51,64)\n"
""));
        actionOpen_Audio_File = new QAction(MainWindow);
        actionOpen_Audio_File->setObjectName("actionOpen_Audio_File");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label_File_Name = new QLabel(centralwidget);
        label_File_Name->setObjectName("label_File_Name");
        label_File_Name->setGeometry(QRect(0, 0, 491, 51));
        label_File_Name->setStyleSheet(QString::fromUtf8("QLabel\n"
"{\n"
" color:rgb(255,255,255);\n"
" border:2px solid rgb(255,255,255);\n"
"}"));
        label_File_Name->setFrameShape(QFrame::Shape::Panel);
        label_File_Name->setLineWidth(2);
        horizontalSlider_Audio_File_Duration = new QSlider(centralwidget);
        horizontalSlider_Audio_File_Duration->setObjectName("horizontalSlider_Audio_File_Duration");
        horizontalSlider_Audio_File_Duration->setGeometry(QRect(0, 70, 491, 22));
        horizontalSlider_Audio_File_Duration->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal\n"
"{\n"
" height:10px;\n"
" width:460px;\n"
" background:red;\n"
" border-radius:5px;\n"
"}\n"
"QSlider::handle:horizontal\n"
"{\n"
"background:rgb(255,255,255);\n"
"width:20px;\n"
"height:20px;\n"
"margin:-7px -7px;\n"
"border-radius:10px;\n"
"}"));
        horizontalSlider_Audio_File_Duration->setOrientation(Qt::Orientation::Horizontal);
        horizontalSlider_Audio_Volume = new QSlider(centralwidget);
        horizontalSlider_Audio_Volume->setObjectName("horizontalSlider_Audio_Volume");
        horizontalSlider_Audio_Volume->setGeometry(QRect(60, 240, 421, 22));
        horizontalSlider_Audio_Volume->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal\n"
"{\n"
" height:10px;\n"
" width:395px;\n"
" background:red;\n"
" border-radius:5px;\n"
"}\n"
"QSlider::handle:horizontal\n"
"{\n"
"background:rgb(255,255,255);\n"
"width:20px;\n"
"height:20px;\n"
"margin:-7px -7px;\n"
"border-radius:10px;\n"
"}"));
        horizontalSlider_Audio_Volume->setOrientation(Qt::Orientation::Horizontal);
        pushButton_Volume = new QPushButton(centralwidget);
        pushButton_Volume->setObjectName("pushButton_Volume");
        pushButton_Volume->setGeometry(QRect(10, 230, 41, 41));
        pushButton_Volume->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border:none;\n"
"border-radius:15px;\n"
"background-color:rgb(255,255,255);\n"
"color:rgb(52,51,64);\n"
"padding:0px;\n"
"}"));
        pushButton_Volume->setIconSize(QSize(40, 40));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(20, 130, 451, 82));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_Seek_Back = new QPushButton(layoutWidget);
        pushButton_Seek_Back->setObjectName("pushButton_Seek_Back");
        pushButton_Seek_Back->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border:none;\n"
"border-radius:40px;\n"
"background-color:rgb(255,255,255);\n"
"color:rgb(52,51,64);\n"
"min-width:80px;\n"
"max-width:80px;\n"
"min-height:80px;\n"
"max-height:80px;\n"
"padding:0px;\n"
"}\n"
"QPushButton::Hover\n"
"{\n"
" background-color:rgb(140,140,140);\n"
"}"));
        pushButton_Seek_Back->setIconSize(QSize(30, 30));

        horizontalLayout->addWidget(pushButton_Seek_Back);

        pushButton_Stop = new QPushButton(layoutWidget);
        pushButton_Stop->setObjectName("pushButton_Stop");
        pushButton_Stop->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border:none;\n"
"border-radius:40px;\n"
"background-color:rgb(255,255,255);\n"
"color:rgb(52,51,64);\n"
"min-width:80px;\n"
"max-width:80px;\n"
"min-height:80px;\n"
"max-height:80px;\n"
"padding:0px;\n"
"}\n"
"QPushButton::Hover\n"
"{\n"
" background-color:rgb(140,140,140);\n"
"}"));
        pushButton_Stop->setIconSize(QSize(30, 30));

        horizontalLayout->addWidget(pushButton_Stop);

        pushButton_Play = new QPushButton(layoutWidget);
        pushButton_Play->setObjectName("pushButton_Play");
        pushButton_Play->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border:none;\n"
"border-radius:40px;\n"
"background-color:rgb(255,255,255);\n"
"color:rgb(52,51,64);\n"
"min-width:80px;\n"
"max-width:80px;\n"
"min-height:80px;\n"
"max-height:80px;\n"
"padding:0px;\n"
"}\n"
"QPushButton::Hover\n"
"{\n"
" background-color:rgb(140,140,140);\n"
"}"));
        pushButton_Play->setIconSize(QSize(30, 30));

        horizontalLayout->addWidget(pushButton_Play);

        pushButton_Pause = new QPushButton(layoutWidget);
        pushButton_Pause->setObjectName("pushButton_Pause");
        pushButton_Pause->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border:none;\n"
"border-radius:40px;\n"
"background-color:rgb(255,255,255);\n"
"color:rgb(52,51,64);\n"
"min-width:80px;\n"
"max-width:80px;\n"
"min-height:80px;\n"
"max-height:80px;\n"
"padding:0px;\n"
"}\n"
"QPushButton::Hover\n"
"{\n"
" background-color:rgb(140,140,140);\n"
"}"));
        pushButton_Pause->setIconSize(QSize(30, 30));

        horizontalLayout->addWidget(pushButton_Pause);

        pushButton_Seek_Forward = new QPushButton(layoutWidget);
        pushButton_Seek_Forward->setObjectName("pushButton_Seek_Forward");
        pushButton_Seek_Forward->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border:none;\n"
"border-radius:40px;\n"
"background-color:rgb(255,255,255);\n"
"color:rgb(52,51,64);\n"
"min-width:80px;\n"
"max-width:80px;\n"
"min-height:80px;\n"
"max-height:80px;\n"
"padding:0px;\n"
"}\n"
"QPushButton::Hover\n"
"{\n"
" background-color:rgb(140,140,140);\n"
"}"));
        pushButton_Seek_Forward->setIconSize(QSize(30, 30));

        horizontalLayout->addWidget(pushButton_Seek_Forward);

        label_Value_1 = new QLabel(centralwidget);
        label_Value_1->setObjectName("label_Value_1");
        label_Value_1->setGeometry(QRect(10, 100, 60, 16));
        QFont font;
        font.setFamilies({QString::fromUtf8("Arial")});
        font.setPointSize(10);
        font.setBold(true);
        label_Value_1->setFont(font);
        label_Value_1->setStyleSheet(QString::fromUtf8("QLabel\n"
"{\n"
" color:rgb(255,255,255);\n"
"}"));
        label_Value_1->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_Value_2 = new QLabel(centralwidget);
        label_Value_2->setObjectName("label_Value_2");
        label_Value_2->setGeometry(QRect(410, 100, 60, 16));
        label_Value_2->setFont(font);
        label_Value_2->setStyleSheet(QString::fromUtf8("QLabel\n"
"{\n"
" color:rgb(255,255,255);\n"
"}"));
        label_Value_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        setTimerButton = new QPushButton(centralwidget);
        setTimerButton->setObjectName("setTimerButton");
        setTimerButton->setGeometry(QRect(70, 290, 80, 24));
        timerSpinBox = new QSpinBox(centralwidget);
        timerSpinBox->setObjectName("timerSpinBox");
        timerSpinBox->setGeometry(QRect(20, 290, 51, 25));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(510, 90, 256, 192));
        pushButton_Shuffle = new QPushButton(centralwidget);
        pushButton_Shuffle->setObjectName("pushButton_Shuffle");
        pushButton_Shuffle->setGeometry(QRect(170, 290, 80, 24));
        pushButton_Shuffle->setCheckable(true);
        pushButton_Repeat = new QPushButton(centralwidget);
        pushButton_Repeat->setObjectName("pushButton_Repeat");
        pushButton_Repeat->setGeometry(QRect(270, 290, 80, 24));
        pushButton_Repeat->setCheckable(true);
        lyricsDisplay = new QTextEdit(centralwidget);
        lyricsDisplay->setObjectName("lyricsDisplay");
        lyricsDisplay->setGeometry(QRect(520, 340, 311, 171));
        QFont font1;
        font1.setPointSize(12);
        lyricsDisplay->setFont(font1);
        lyricsDisplay->setFrameShape(QFrame::Shape::NoFrame);
        lyricsDisplay->setReadOnly(true);
        MainWindow->setCentralWidget(centralwidget);
        layoutWidget->raise();
        label_File_Name->raise();
        horizontalSlider_Audio_File_Duration->raise();
        horizontalSlider_Audio_Volume->raise();
        pushButton_Volume->raise();
        label_Value_1->raise();
        label_Value_2->raise();
        setTimerButton->raise();
        timerSpinBox->raise();
        listWidget->raise();
        pushButton_Shuffle->raise();
        pushButton_Repeat->raise();
        lyricsDisplay->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 870, 21));
        menuOpen = new QMenu(menubar);
        menuOpen->setObjectName("menuOpen");
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuOpen->menuAction());
        menuOpen->addAction(actionOpen_Audio_File);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen_Audio_File->setText(QCoreApplication::translate("MainWindow", "Open Audio File", nullptr));
        label_File_Name->setText(QCoreApplication::translate("MainWindow", "FILE NAME", nullptr));
        pushButton_Volume->setText(QString());
        pushButton_Seek_Back->setText(QString());
        pushButton_Stop->setText(QString());
        pushButton_Play->setText(QString());
        pushButton_Pause->setText(QString());
        pushButton_Seek_Forward->setText(QString());
        label_Value_1->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        label_Value_2->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        setTimerButton->setText(QCoreApplication::translate("MainWindow", "Set Timer", nullptr));
        pushButton_Shuffle->setText(QCoreApplication::translate("MainWindow", "Shuffle", nullptr));
        pushButton_Repeat->setText(QCoreApplication::translate("MainWindow", "Repeat", nullptr));
        menuOpen->setTitle(QCoreApplication::translate("MainWindow", "Open", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
