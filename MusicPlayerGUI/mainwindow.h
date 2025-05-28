#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QListWidgetItem> // Penting: Pastikan ini disertakan

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Struktur untuk menyimpan data lirik sinkron
struct LyricLine {
    qint64 timestamp; // Waktu dalam milidetik
    QString text;     // Teks lirik
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected: // Ubah aksesibilitas dropEvent dan dragEnterEvent ke protected
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;

private slots:
    void updateduration(qint64 duration);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void on_pushButton_Volume_clicked();
    void on_actionOpen_Audio_File_triggered();
    void on_pushButton_Play_clicked();
    void on_pushButton_Pause_clicked();
    void on_pushButton_Stop_clicked();
    void on_nextButton_clicked();
    void on_previousButton_clicked();
    void on_horizontalSlider_Audio_File_Duration_valueChanged(int value);
    void on_horizontalSlider_Audio_Volume_valueChanged(int value);
    void handleTimerTimeout();
    void on_setTimerButton_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_listWidget_SearchResult_itemClicked(QListWidgetItem *item);
    void putarLaguPadaIndeks(int indeks);
    void on_pushButton_Shuffle_toggled(bool checked);
    void on_pushButton_Repeat_toggled(bool checked);
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void on_searchLineEdit_textChanged(const QString &query);
    void fetchLyrics(const QString &fullTitle);
    void on_pushButton_DaftarLagu_clicked();
    void on_pushButton_KembaliKeLirik_clicked();

    // Fungsi baru untuk memperbarui daftarLagu internal setelah drag and drop UI
    void syncDaftarLaguWithListWidget();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *MPlayer;
    QAudioOutput *audioOutput;
    qint64 Mduration;
    bool IS_Muted;
    QTimer *stopTimer; // Timer untuk sleep function
    QNetworkAccessManager *networkManager; // Untuk fetch lirik
    QTimer *lyricsTimer; // Timer untuk update lirik
    QVector<LyricLine> syncedLyricsData; // Data lirik yang sudah di-parse
    int currentLyricIndex; // Indeks baris lirik yang sedang aktif

    QStringList daftarLagu;        // Daftar lagu utama (full path)
    QStringList hasilPencarian;    // Hasil pencarian (full path)
    int indeksLaguSaatIni;         // Indeks lagu yang sedang diputar di daftarLagu
    bool isShuffle;                // Status shuffle
    bool isRepeat;                 // Status repeat

    void urutkanDaftarLagu(); // Fungsi ini akan diubah untuk hanya merefresh tampilan
    void updateLyricDisplay(qint64 currentPosition);
    void loadSyncedLyrics(const QString& lyricsContent);
};
#endif // MAINWINDOW_H
