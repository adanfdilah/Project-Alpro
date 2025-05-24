#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Buat dulu player dan audioOutput
    MPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    MPlayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);

    connect(MPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::handleMediaStatusChanged);

    networkManager = new QNetworkAccessManager(this);
    lyricsDisplay = new QTextEdit(this);
    lyricsDisplay->setReadOnly(true);
    lyricsDisplay->setGeometry(20, 400, 400, 150);


    // Koneksi list widget item klik
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::on_listWidget_itemClicked);

    stopTimer = new QTimer(this);
    stopTimer->setSingleShot(true);
    connect(stopTimer, &QTimer::timeout, this, &MainWindow::handleTimerTimeout);

    setAcceptDrops(true);

    // Setup ikon tombol dan volume slider
    ui->pushButton_Play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButton_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->pushButton_Stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    ui->pushButton_Shuffle->setIcon(QIcon(":/new/icon/shuffle.svg"));
    ui->pushButton_Repeat->setIcon(QIcon(":/new/icon/repeat.svg"));


    ui->lyricsDisplay->setReadOnly(true);
    ui->lyricsDisplay->setFrameShape(QFrame::NoFrame);
    ui->lyricsDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QFont font("Segoe UI", 12);
    ui->lyricsDisplay->setFont(font);
    ui->lyricsDisplay->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    ui->horizontalSlider_Audio_Volume->setMinimum(1);
    ui->horizontalSlider_Audio_Volume->setMaximum(100);
    ui->horizontalSlider_Audio_Volume->setValue(30);
    audioOutput->setVolume(ui->horizontalSlider_Audio_Volume->value() / 100.0);

    connect(MPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(MPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);

    ui->horizontalSlider_Audio_File_Duration->setRange(0, MPlayer->duration() / 1000);

    ui->stackedView->setCurrentWidget(ui->page_Lyrics); //Default menampilkan lirik di awal
    ui->pushButton_Repeat->setIcon(QIcon(":/new/icon/repeat.svg"));
    ui->pushButton_Shuffle->setIcon(QIcon(":/new/icon/shuffle.svg"));

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateduration(qint64 duration)
{
    QString timestr;
    if (duration || Mduration)
    {
        QTime CurrentTime((duration / 3600) % 60, (duration / 60) % 60, duration % 60,(duration * 1000) % 1000);
        QTime totalTime((Mduration / 3600) % 60, (Mduration / 60) % 60, Mduration % 60,(Mduration * 1000) % 1000);
        QString format = "mm:ss";
        if (Mduration > 3600)
            format = "mm:ss";
        ui->label_Value_1->setText(CurrentTime.toString(format));
        ui->label_Value_2->setText(totalTime.toString(format));
    }
}

void MainWindow::durationChanged(qint64 duration)
{
    Mduration = duration / 1000;
    ui->horizontalSlider_Audio_File_Duration->setMaximum(Mduration);
}

void MainWindow::positionChanged(qint64 progress)
{
    if (!ui->horizontalSlider_Audio_File_Duration->isSliderDown())
    {
        ui->horizontalSlider_Audio_File_Duration->setValue(progress / 1000);
    }

    updateduration(progress / 1000);
}

void MainWindow::on_pushButton_Volume_clicked()
{
    if (IS_Muted == false)
    {
        ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        IS_Muted = true;
        audioOutput->setMuted(true);
    }
    else
    {
        ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        IS_Muted = false;
        audioOutput->setMuted(false);
    }
}


void MainWindow::on_actionOpen_Audio_File_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(
        this,
        tr("Select Audio Files"),
        "",
        tr("Audio Files (*.mp3 *.wav)")
        );

    if (fileNames.isEmpty())
        return;

    daftarLagu = fileNames; // <- simpan ke anggota class untuk referensi nanti

    ui->listWidget->clear(); // kosongkan list sebelumnya
    for (const QString &file : daftarLagu) {
        ui->listWidget->addItem(QFileInfo(file).fileName());
    }

    // Set lagu pertama sebagai default dan tampilkan namanya
    MPlayer->setSource(QUrl::fromLocalFile(daftarLagu[0]));
    QFileInfo fileInfo(daftarLagu[0]);
    ui->label_File_Name->setText(fileInfo.fileName());

    putarLaguPadaIndeks(0);
}



void MainWindow::on_pushButton_Play_clicked()
{
    if (MPlayer->playbackState() == QMediaPlayer::PausedState) {
        MPlayer->play(); // Lanjut dari posisi terakhir
    } else if (MPlayer->playbackState() != QMediaPlayer::PlayingState) {
        if (MPlayer->mediaStatus() == QMediaPlayer::EndOfMedia)
            MPlayer->setPosition(0); // Ulangi jika sudah selesai
        MPlayer->play();
    }
}

void MainWindow::on_pushButton_Pause_clicked()
{
    if (MPlayer->playbackState() == QMediaPlayer::PlayingState) {
        MPlayer->pause();
    }
}



void MainWindow::on_pushButton_Stop_clicked()
{
    MPlayer->stop();
}


void MainWindow::on_nextButton_clicked()
{
    if (daftarLagu.isEmpty()) return;

    currentSongIndex++;
    if (currentSongIndex >= daftarLagu.size())
        currentSongIndex = 0; // kembali ke awal

    putarLaguDariIndeks(currentSongIndex);
}

void MainWindow::on_previousButton_clicked()
{
    if (daftarLagu.isEmpty()) return;

    currentSongIndex--;
    if (currentSongIndex < 0)
        currentSongIndex = daftarLagu.size() - 1; // ke lagu terakhir

    putarLaguDariIndeks(currentSongIndex);
}



void MainWindow::on_horizontalSlider_Audio_File_Duration_valueChanged(int value)
{
    MPlayer->setPosition(value * 1000);
}


void MainWindow::on_horizontalSlider_Audio_Volume_valueChanged(int value)
{
    audioOutput->setVolume(value / 100.0);
}


void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> droppedUrls = event->mimeData()->urls();
    if (!droppedUrls.isEmpty()) {
        QString filePath = droppedUrls.first().toLocalFile();

        // Optional: Cek format
        if (filePath.endsWith(".mp3") || filePath.endsWith(".wav")) {
            MPlayer->setSource(QUrl::fromLocalFile(filePath));
            MPlayer->play();

            QFileInfo info(filePath);
            ui->label_File_Name->setText(info.fileName());
        } else {
            QMessageBox::warning(this, "Format tidak didukung", "Hanya file .mp3 dan .wav yang didukung.");
        }
    }
}

void MainWindow::handleTimerTimeout()
{
    if (MPlayer->playbackState() == QMediaPlayer::PlayingState) {
        MPlayer->pause(); // Bisa juga pakai stop()
        QMessageBox::information(this, "Timer", "Musik dihentikan otomatis.");
    }
}



void MainWindow::on_setTimerButton_clicked()
{
    int minutes = ui->timerSpinBox->value(); // Ambil nilai input

    if (minutes > 0) {
        stopTimer->start(minutes * 60 * 1000); // Konversi menit ke milidetik
        QMessageBox::information(this, "Timer", QString("Musik akan berhenti dalam %1 menit.").arg(minutes));
    } else {
        QMessageBox::warning(this, "Timer", "Masukkan waktu lebih dari 0 menit.");
    }
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    int index = ui->listWidget->row(item);
    if (index >= 0 && index < daftarLagu.size()) {
        QString path = daftarLagu[index];
        MPlayer->setSource(QUrl::fromLocalFile(path));
        MPlayer->play();

        QFileInfo fileInfo(path);
        ui->label_File_Name->setText(fileInfo.fileName());
    }
}

void MainWindow::on_listWidget_SearchResult_itemClicked(QListWidgetItem *item)
{
    QString fileName = item->text();
    QString path;

    for (const QString &p : hasilPencarian) {
        if (QFileInfo(p).fileName() == fileName) {
            path = p;
            break;
        }
    }

    if (!path.isEmpty()) {
        MPlayer->setSource(QUrl::fromLocalFile(path));
        MPlayer->play();
        ui->label_File_Name->setText(fileName);
    }
}




void MainWindow::putarLaguPadaIndeks(int indeks)
{
    if (daftarLagu.isEmpty()) return;

    if (isShuffle && daftarLagu.size() > 1) {
        int randomIndex;
        do {
            randomIndex = QRandomGenerator::global()->bounded(daftarLagu.size());
        } while (randomIndex == indeksLaguSaatIni);
        indeks = randomIndex;
    }

    if (indeks < 0 || indeks >= daftarLagu.size()) return;

    indeksLaguSaatIni = indeks;
    ui->listWidget->setCurrentRow(indeks);
    MPlayer->setSource(QUrl::fromLocalFile(daftarLagu[indeks]));
    MPlayer->play();

    QFileInfo info(daftarLagu[indeks]);
    QString judulLagu = info.baseName();
    fetchLyrics(judulLagu);
}

void MainWindow::putarLaguDariIndeks(int index)
{
    if (index >= 0 && index < daftarLagu.size()) {
        QString path = daftarLagu[index];
        MPlayer->setSource(QUrl::fromLocalFile(path));
        MPlayer->play();

        QFileInfo fileInfo(path);
        ui->label_File_Name->setText(fileInfo.fileName());

        // Tandai di list widget
        ui->listWidget->setCurrentRow(index);
    }
}




void MainWindow::on_pushButton_Shuffle_toggled(bool checked)
{
    isShuffle = checked;
    if (isShuffle) {
        ui->pushButton_Shuffle->setStyleSheet("background-color: lightgreen"); // Contoh highlight
    } else {
        ui->pushButton_Shuffle->setStyleSheet(""); // Reset style
    }
}



void MainWindow::on_pushButton_Repeat_toggled(bool checked)
{
    isRepeat = checked;

    if (checked) {
        ui->pushButton_Repeat->setStyleSheet("background-color: #00cc66; color: white;");
    } else {
        ui->pushButton_Repeat->setStyleSheet("");
    }
}

void MainWindow::handleMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia)
    {
        if (isRepeat) {
            // Jika repeat aktif, ulangi lagu yang sedang diputar
            MPlayer->setPosition(0);
            MPlayer->play();
        }
        else {
            // Jika tidak repeat, lanjut ke lagu berikutnya (atau shuffle)
            int nextIndex = indeksLaguSaatIni + 1;

            if (isShuffle && daftarLagu.size() > 1) {
                do {
                    nextIndex = QRandomGenerator::global()->bounded(daftarLagu.size());
                } while (nextIndex == indeksLaguSaatIni);
            }

            if (nextIndex < daftarLagu.size()) {
                putarLaguPadaIndeks(nextIndex);
            } else {
                // Jika sudah lagu terakhir, berhenti saja
                MPlayer->stop();
            }
        }
    }
}


void MainWindow::on_searchLineEdit_textChanged(const QString &query)
{
    ui->listWidget_SearchResult->clear();
    hasilPencarian.clear();

    if (!query.isEmpty()) {
        for (const QString &path : daftarLagu) {
            if (QFileInfo(path).fileName().contains(query, Qt::CaseInsensitive)) {
                hasilPencarian.append(path);
                ui->listWidget_SearchResult->addItem(QFileInfo(path).fileName());
            }
        }

        if (!hasilPencarian.isEmpty()) {
            ui->stackedView->setCurrentWidget(ui->page_Search);  // Tampilkan hasil pencarian
        } else {
            ui->stackedView->setCurrentWidget(ui->page_Lyrics);  // Tidak ada hasil, tampilkan lirik
        }
    } else {
        ui->stackedView->setCurrentWidget(ui->page_Lyrics);  // Kosongkan pencarian, kembali ke lirik
    }
}



void MainWindow::fetchLyrics(const QString &fullTitle)
{
    // Bersihkan dan pisah artist / title
    QString cleaned = fullTitle;
    cleaned.remove("(Lyrics)", Qt::CaseInsensitive).trimmed();

    QStringList parts = cleaned.split(" - ");
    QString artist, title;
    if (parts.size() >= 2) {
        artist = parts[0].trimmed();
        title  = parts[1].trimmed();
    } else {
        artist = "Unknown";
        title  = cleaned;
    }

    // Rangkai URL dengan percent-encoding
    QString apiUrl = QString("https://api.lyrics.ovh/v1/%1/%2")
                         .arg(QUrl::toPercentEncoding(artist))
                         .arg(QUrl::toPercentEncoding(title));

    // Kirim request ke API
    QNetworkRequest request{ QUrl(apiUrl) };
    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QJsonObject obj = doc.object();
            QString lyricsText = obj.value("lyrics").toString("Lirik tidak ditemukan.");

            // Set teks lirik di UI
            ui->lyricsDisplay->setPlainText(lyricsText);

            // Tambahkan efek fade-in setelah teks muncul
            QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
            ui->lyricsDisplay->setGraphicsEffect(effect);

            QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
            animation->setDuration(600);
            animation->setStartValue(0);
            animation->setEndValue(1);
            animation->start(QAbstractAnimation::DeleteWhenStopped);

        } else {
            ui->lyricsDisplay->setPlainText("Gagal mengambil lirik: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

void MainWindow::on_pushButton_DaftarLagu_clicked()
{
    ui->stackedView->setCurrentWidget(ui->page_DaftarLagu); // Tampilkan daftar lagu utama
}

void MainWindow::on_pushButton_KembaliKeLirik_clicked()
{
    ui->stackedView->setCurrentWidget(ui->page_Lyrics); // Kembali ke halaman lirik
}
