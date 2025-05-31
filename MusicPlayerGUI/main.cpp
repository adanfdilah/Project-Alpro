#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    try {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    } catch (const std::exception &e) {
        QMessageBox::critical(nullptr, "Exception", e.what());
    } catch (...) {
        QMessageBox::critical(nullptr, "Unknown Error", "Terjadi kesalahan tidak dikenal.");
    }
    return -1;
}
