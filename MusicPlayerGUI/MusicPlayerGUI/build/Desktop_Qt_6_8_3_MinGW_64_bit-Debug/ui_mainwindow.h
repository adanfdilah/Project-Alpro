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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
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
    QPushButton *previousButton;
    QPushButton *pushButton_Stop;
    QPushButton *pushButton_Play;
    QPushButton *pushButton_Pause;
    QPushButton *nextButton;
    QLabel *label_Value_1;
    QLabel *label_Value_2;
    QPushButton *setTimerButton;
    QSpinBox *timerSpinBox;
    QPushButton *pushButton_Shuffle;
    QPushButton *pushButton_Repeat;
    QLineEdit *searchLineEdit;
    QPushButton *pushButton_DaftarLagu;
    QPushButton *pushButton_KembaliKeLirik;
    QStackedWidget *stackedView;
    QWidget *page_Lyrics;
    QTextEdit *lyricsDisplay;
    QWidget *page_Search;
    QListWidget *listWidget_SearchResult;
    QWidget *page_DaftarLagu;
    QListWidget *listWidget;
    QMenuBar *menubar;
    QMenu *menuOpen;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(491, 606);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color:rgb(52,51,64)\n"
""));
        actionOpen_Audio_File = new QAction(MainWindow);
        actionOpen_Audio_File->setObjectName("actionOpen_Audio_File");
        QFont font;
        font.setFamilies({QString::fromUtf8("Bahnschrift")});
        font.setPointSize(10);
        actionOpen_Audio_File->setFont(font);
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
        pushButton_Volume->setGeometry(QRect(10, 230, 40, 40));
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
        previousButton = new QPushButton(layoutWidget);
        previousButton->setObjectName("previousButton");
        previousButton->setStyleSheet(QString::fromUtf8("QPushButton\n"
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
        previousButton->setIconSize(QSize(30, 30));

        horizontalLayout->addWidget(previousButton);

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

        nextButton = new QPushButton(layoutWidget);
        nextButton->setObjectName("nextButton");
        nextButton->setStyleSheet(QString::fromUtf8("QPushButton\n"
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
        nextButton->setIconSize(QSize(30, 30));

        horizontalLayout->addWidget(nextButton);

        label_Value_1 = new QLabel(centralwidget);
        label_Value_1->setObjectName("label_Value_1");
        label_Value_1->setGeometry(QRect(10, 100, 60, 16));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Arial")});
        font1.setPointSize(10);
        font1.setBold(true);
        label_Value_1->setFont(font1);
        label_Value_1->setStyleSheet(QString::fromUtf8("QLabel\n"
"{\n"
" color:rgb(255,255,255);\n"
"}"));
        label_Value_1->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_Value_2 = new QLabel(centralwidget);
        label_Value_2->setObjectName("label_Value_2");
        label_Value_2->setGeometry(QRect(410, 100, 60, 16));
        label_Value_2->setFont(font1);
        label_Value_2->setStyleSheet(QString::fromUtf8("QLabel\n"
"{\n"
" color:rgb(255,255,255);\n"
"}"));
        label_Value_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        setTimerButton = new QPushButton(centralwidget);
        setTimerButton->setObjectName("setTimerButton");
        setTimerButton->setGeometry(QRect(250, 280, 23, 25));
        QFont font2;
        font2.setPointSize(7);
        setTimerButton->setFont(font2);
        setTimerButton->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border:none;\n"
"border-radius:10px;\n"
"background-color:rgb(255,255,255);\n"
"color:rgb(52,51,64);\n"
"padding:0px;\n"
"}\n"
"QPushButton::Hover\n"
"{\n"
" background-color:rgb(140,140,140);\n"
"}"));
        timerSpinBox = new QSpinBox(centralwidget);
        timerSpinBox->setObjectName("timerSpinBox");
        timerSpinBox->setGeometry(QRect(200, 280, 51, 25));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(52, 51, 64, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        timerSpinBox->setPalette(palette);
        pushButton_Shuffle = new QPushButton(centralwidget);
        pushButton_Shuffle->setObjectName("pushButton_Shuffle");
        pushButton_Shuffle->setGeometry(QRect(380, 280, 40, 25));
        pushButton_Shuffle->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border:none;\n"
"border-radius:10px;\n"
"background-color:rgb(255,255,255);\n"
"color:rgb(52,51,64);\n"
"padding:0px;\n"
"}"));
        pushButton_Shuffle->setCheckable(true);
        pushButton_Repeat = new QPushButton(centralwidget);
        pushButton_Repeat->setObjectName("pushButton_Repeat");
        pushButton_Repeat->setGeometry(QRect(430, 280, 40, 25));
        pushButton_Repeat->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border:none;\n"
"border-radius:10px;\n"
"background-color:rgb(255,255,255);\n"
"color:rgb(52,51,64);\n"
"padding:0px;\n"
"}"));
        pushButton_Repeat->setCheckable(true);
        searchLineEdit = new QLineEdit(centralwidget);
        searchLineEdit->setObjectName("searchLineEdit");
        searchLineEdit->setGeometry(QRect(19, 280, 171, 25));
        searchLineEdit->setAutoFillBackground(false);
        searchLineEdit->setStyleSheet(QString::fromUtf8("QLineEdit\n"
"{\n"
"border:none;\n"
"border-radius:10px;\n"
"background-color:rgb(255,255,255);\n"
"color:rgb(15,15,15);\n"
"padding:3px;\n"
"}"));
        pushButton_DaftarLagu = new QPushButton(centralwidget);
        pushButton_DaftarLagu->setObjectName("pushButton_DaftarLagu");
        pushButton_DaftarLagu->setGeometry(QRect(330, 280, 40, 25));
        pushButton_DaftarLagu->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border:none;\n"
"border-radius:10px;\n"
"background-color:rgb(255,255,255);\n"
"color:rgb(52,51,64);\n"
"padding:0px;\n"
"}\n"
"QPushButton::Hover\n"
"{\n"
" background-color:rgb(140,140,140);\n"
"}"));
        pushButton_KembaliKeLirik = new QPushButton(centralwidget);
        pushButton_KembaliKeLirik->setObjectName("pushButton_KembaliKeLirik");
        pushButton_KembaliKeLirik->setGeometry(QRect(280, 280, 40, 25));
        pushButton_KembaliKeLirik->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border:none;\n"
"border-radius:10px;\n"
"background-color:rgb(255,255,255);\n"
"color:rgb(52,51,64);\n"
"padding:0px;\n"
"}\n"
"QPushButton::Hover\n"
"{\n"
" background-color:rgb(140,140,140);\n"
"}"));
        stackedView = new QStackedWidget(centralwidget);
        stackedView->setObjectName("stackedView");
        stackedView->setGeometry(QRect(20, 320, 451, 241));
        stackedView->setStyleSheet(QString::fromUtf8("QStackedWidget\n"
"{\n"
" color:rgb(255,255,255);\n"
" border:2px solid rgb(255,255,255);\n"
"}"));
        page_Lyrics = new QWidget();
        page_Lyrics->setObjectName("page_Lyrics");
        lyricsDisplay = new QTextEdit(page_Lyrics);
        lyricsDisplay->setObjectName("lyricsDisplay");
        lyricsDisplay->setGeometry(QRect(10, 10, 431, 221));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        lyricsDisplay->setPalette(palette1);
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Georgia")});
        font3.setPointSize(11);
        lyricsDisplay->setFont(font3);
        lyricsDisplay->setFrameShape(QFrame::Shape::NoFrame);
        lyricsDisplay->setReadOnly(true);
        stackedView->addWidget(page_Lyrics);
        page_Search = new QWidget();
        page_Search->setObjectName("page_Search");
        listWidget_SearchResult = new QListWidget(page_Search);
        listWidget_SearchResult->setObjectName("listWidget_SearchResult");
        listWidget_SearchResult->setGeometry(QRect(10, 10, 431, 231));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        listWidget_SearchResult->setPalette(palette2);
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Gadugi")});
        font4.setPointSize(10);
        listWidget_SearchResult->setFont(font4);
        stackedView->addWidget(page_Search);
        page_DaftarLagu = new QWidget();
        page_DaftarLagu->setObjectName("page_DaftarLagu");
        listWidget = new QListWidget(page_DaftarLagu);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(10, 10, 431, 231));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        listWidget->setPalette(palette3);
        listWidget->setFont(font4);
        stackedView->addWidget(page_DaftarLagu);
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
        pushButton_Shuffle->raise();
        pushButton_Repeat->raise();
        searchLineEdit->raise();
        pushButton_DaftarLagu->raise();
        pushButton_KembaliKeLirik->raise();
        stackedView->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 491, 23));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        menubar->setPalette(palette4);
        QFont font5;
        font5.setPointSize(10);
        menubar->setFont(font5);
        menuOpen = new QMenu(menubar);
        menuOpen->setObjectName("menuOpen");
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuOpen->menuAction());
        menuOpen->addAction(actionOpen_Audio_File);

        retranslateUi(MainWindow);

        stackedView->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen_Audio_File->setText(QCoreApplication::translate("MainWindow", "Open Audio File", nullptr));
#if QT_CONFIG(tooltip)
        actionOpen_Audio_File->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; text-decoration: underline; color:#ffffff;\">Open Audio File</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        label_File_Name->setText(QCoreApplication::translate("MainWindow", "FILE NAME", nullptr));
        pushButton_Volume->setText(QString());
        previousButton->setText(QString());
        pushButton_Stop->setText(QString());
        pushButton_Play->setText(QString());
        pushButton_Pause->setText(QString());
        nextButton->setText(QString());
        label_Value_1->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        label_Value_2->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        setTimerButton->setText(QString());
        pushButton_Shuffle->setText(QString());
        pushButton_Repeat->setText(QString());
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Cari lagu...", nullptr));
        pushButton_DaftarLagu->setText(QString());
        pushButton_KembaliKeLirik->setText(QString());
        menuOpen->setTitle(QCoreApplication::translate("MainWindow", "Open", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
