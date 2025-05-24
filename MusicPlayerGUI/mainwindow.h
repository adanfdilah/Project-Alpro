#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include <QListWidgetItem>
#include <QTextEdit>
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

    void on_pushButton_DaftarLagu_clicked();

    void on_pushButton_KembaliKeLirik_clicked();

    void on_searchLineEdit_textChanged(const QString &arg1);

    void on_listWidget_SearchResult_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;

    QMediaPlayer *MPlayer;
    QAudioOutput *audioOutput;
    QTimer *stopTimer;
    QTextEdit *lyricsDisplay;
    QNetworkAccessManager *networkManager;

    QStringList daftarLagu;
    int indeksLaguSaatIni = 0;
    int Mduration = 0;
    bool IS_Muted = false;
    bool isShuffle = false;
    bool isRepeat = false;
    int currentSongIndex = -1;
    void putarLaguDariIndeks(int index);
    QVector<QString> hasilPencarian; // berisi hasil dari pencarian saat ini

    void updateduration(qint64 duration);
    void putarLaguPadaIndeks(int indeks);
    void fetchLyrics(const QString &title);
};

#endif // MAINWINDOW_H
