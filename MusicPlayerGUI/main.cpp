#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    try {
        QApplication a(argc, argv);
        MainWindow w;

        w.show(); // TAMPILKAN DULU

        if (argc > 1) {
            QString fullPath;

            // Gabungkan semua argumen (misal: "C:\My Music\lagu bagus.wav")
            for (int i = 1; i < argc; ++i) {
                fullPath += QString::fromLocal8Bit(argv[i]) + " ";
            }

            fullPath = fullPath.trimmed().remove("\""); // Hapus spasi akhir & kutipan

            if (QFileInfo::exists(fullPath)) {
                QTimer::singleShot(100, [&w, fullPath]() {
                    w.openAudioFileFromArg(fullPath);
                });
            } else {
                QMessageBox::warning(nullptr, "File Tidak Ditemukan", "File tidak ditemukan:\n" + fullPath);
            }
        }

        return a.exec();

    } catch (const std::exception &e) {
        QMessageBox::critical(nullptr, "Exception", e.what());
    } catch (...) {
        QMessageBox::critical(nullptr, "Unknown Error", "Terjadi kesalahan tidak dikenal.");
    }
    return -1;
}

