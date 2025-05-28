// mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include <QListWidgetItem>
// #include <QTextEdit> // ui->lyricsDisplay sudah bertipe QTextEdit, tidak perlu deklarasi terpisah
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFileDialog>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QTime>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
// #include <QMap> // Tidak lagi diperlukan jika hanya untuk syncedLyrics
// #include <QPair> // Tidak lagi diperlukan secara eksplisit
#include <QVector> // Diperlukan untuk syncedLyricsData
#include <QRegularExpression> // Diperlukan untuk QRegularExpression
#include <QRegularExpressionMatch> // Diperlukan untuk QRegularExpressionMatch
#include <QFileInfo> // Diperlukan untuk QFileInfo

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void on_pushButton_Play_clicked();
    void on_pushButton_Pause_clicked();
    void on_pushButton_Stop_clicked();
    void on_nextButton_clicked();
    void on_previousButton_clicked();
    void on_pushButton_Volume_clicked();
    void on_horizontalSlider_Audio_File_Duration_valueChanged(int value);
    void on_horizontalSlider_Audio_Volume_valueChanged(int value);
    void on_actionOpen_Audio_File_triggered();
    void on_setTimerButton_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_pushButton_Shuffle_toggled(bool checked);
    void on_pushButton_Repeat_toggled(bool checked);

    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void handleTimerTimeout();
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
    // void onMediaStatusChanged(QMediaPlayer::MediaStatus status); // Dihapus karena redundant

    void on_pushButton_DaftarLagu_clicked();
    void on_pushButton_KembaliKeLirik_clicked();
    void on_searchLineEdit_textChanged(const QString &arg1);
    void on_listWidget_SearchResult_itemClicked(QListWidgetItem *item);

private: // Fungsi-fungsi helper dan member variables
    Ui::MainWindow *ui;

    QMediaPlayer *MPlayer;
    QAudioOutput *audioOutput;
    QTimer *lyricsTimer; // Timer untuk update lirik sinkron
    QTimer *stopTimer;   // Timer untuk sleep function

    QNetworkAccessManager *networkManager; // Untuk mengambil lirik online

    // --- Struktur data baru untuk lirik sinkron ---
    struct LyricLine {
        qint64 timestamp; // Waktu dalam milidetik
        QString text;     // Teks liriknya
    };
    QVector<LyricLine> syncedLyricsData; // Vektor untuk menyimpan lirik sinkron yang sudah di-parse
    int currentLyricIndex = 0; // Indeks baris lirik yang sedang aktif

    // --- Variabel kontrol pemutar musik ---
    QStringList daftarLagu; // Daftar path file audio
    int indeksLaguSaatIni = -1; // Indeks lagu yang sedang diputar di daftarLagu (-1 berarti tidak ada)
    qint64 Mduration = 0; // Total durasi lagu dalam detik (diubah menjadi qint64 untuk konsisten dengan MPlayer)
    bool IS_Muted = false;
    bool isShuffle = false;
    bool isRepeat = false;

    QVector<QString> hasilPencarian; // Berisi path lengkap hasil dari pencarian saat ini

    // --- Fungsi-fungsi helper yang dipanggil secara internal ---
    void updateduration(qint64 duration); // Update tampilan durasi
    void putarLaguPadaIndeks(int indeks); // Memutar lagu berdasarkan indeks di daftarLagu
    void urutkanDaftarLagu(); // Mengurutkan daftar lagu
    void fetchLyrics(const QString &fullTitle); // Mengambil lirik dari API
    void loadSyncedLyrics(const QString& lyricsContent); // Memproses dan memuat lirik sinkron
    void updateLyricDisplay(qint64 currentPosition); // Memperbarui tampilan lirik yang sedang aktif
    // void tampilkanBarisLirik(); // Dihapus, karena sudah tidak diperlukan dengan updateLyricDisplay yang baru
};

#endif // MAINWINDOW_H
