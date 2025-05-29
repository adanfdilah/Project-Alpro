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
#include <QFileDialog>
#include <QMimeData> // Untuk drag and drop
#include <QDragEnterEvent> // Untuk drag and drop
#include <QDropEvent> // Untuk drag and drop
#include <QMessageBox> // Untuk pesan informasi
#include <QFileInfo> // Untuk informasi file
#include <QRandomGenerator> // Untuk shuffle
#include <algorithm> // Untuk std::sort
#include <QUrl> // Untuk QUrl::fromLocalFile
#include <QTime> // Untuk format waktu
#include <QRegularExpression> // Ganti QRegExp dengan QRegularExpression
#include <QRegularExpressionMatch> // Tambahkan ini juga untuk QRegularExpressionMatch
#include <QDebug> // Untuk debugging output
#include <QTextBlock>
#include <QFile>
#include <QTextCursor>
#include <QTextDocument>
#include <QToolTip> // untuk styleSheet QToolTip
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Inisialisasi QMediaPlayer dan QAudioOutput
    MPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    MPlayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);

    // Koneksi untuk update lirik (menggunakan lyricsTimer yang sudah ada)
    lyricsTimer = new QTimer(this);
    lyricsTimer->setInterval(250); // Cek tiap 100ms posisi lagu untuk update lirik
    connect(lyricsTimer, &QTimer::timeout, this, [this]() {
        qint64 pos = MPlayer->position();
        updateLyricDisplay(pos);
    });

    // Koneksi untuk status media player
    connect(MPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::handleMediaStatusChanged);

    connect(ui->listWidget_Queue->model(), &QAbstractItemModel::rowsMoved, this, &MainWindow::updateDaftarLaguSetelahDrag);

    // Setup timer untuk sleep function
    stopTimer = new QTimer(this);
    stopTimer->setSingleShot(true);
    connect(stopTimer, &QTimer::timeout, this, &MainWindow::handleTimerTimeout);

    // Aktifkan drag and drop
    setAcceptDrops(true);

    ui->listWidget_Queue->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listWidget_Queue->setDragDropMode(QAbstractItemView::InternalMove);
    ui->listWidget_Queue->setDefaultDropAction(Qt::MoveAction);
    ui->listWidget_Queue->setDragEnabled(true);
    ui->listWidget_Queue->setAcceptDrops(true);
    ui->listWidget_Queue->setDropIndicatorShown(true);

    // styleSheet
    qApp->setStyleSheet("QToolTip { "
                            "color: white; "               // Warna teks
                            "background-color: #323232; "  // Warna latar
                            "border: 1px solid white; "
                            "padding: 5px; "
                            "}");
    // tool tip keterengan
    ui->previousButton->setToolTip("Previous");
    ui->pushButton_Stop->setToolTip("Reset");
    ui->pushButton_Play->setToolTip("Play");
    ui->pushButton_Pause->setToolTip("Pause");
    ui->nextButton->setToolTip("Next");
    ui->setTimerButton->setToolTip("Sleep Timer");
    ui->pushButton_KembaliKeLirik->setToolTip("Lirik");
    ui->pushButton_DaftarLagu->setToolTip("Playlist");
    ui->pushButton_Shuffle->setToolTip("Shuffle");
    ui->pushButton_Repeat->setToolTip("Repeat");
    ui->pushButton_Volume->setToolTip("Sound");
    ui->pushButton_ListWidgetQueue->setToolTip("Antrian");


    // Setup ikon tombol dan volume slider
    ui->pushButton_Play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButton_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->pushButton_Stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    ui->pushButton_Shuffle->setIcon(QIcon(":/new/icon/shuffle.svg"));
    ui->pushButton_Repeat->setIcon(QIcon(":/new/icon/repeat.svg"));
    ui->pushButton_KembaliKeLirik->setIcon(QIcon(":/new/icon/lyric.svg"));
    ui->pushButton_DaftarLagu->setIcon(QIcon(":/new/icon/playlist.svg"));
    ui->setTimerButton->setIcon(QIcon(":/new/icon/timer.svg"));
    ui->pushButton_ListWidgetQueue->setIcon(QIcon(":/new/icon/queue.svg"));

    // Inisialisasi QNetworkAccessManager untuk fetch lirik
    networkManager = new QNetworkAccessManager(this);

    // Setup lyricsDisplay (QPlainTextEdit)
    ui->lyricsDisplay->setReadOnly(true);
    ui->lyricsDisplay->setFrameShape(QFrame::NoFrame);
    ui->lyricsDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QFont font("Segoe UI", 12);
    ui->lyricsDisplay->setFont(font);
    ui->lyricsDisplay->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    // Setup volume slider
    ui->horizontalSlider_Audio_Volume->setMinimum(1);
    ui->horizontalSlider_Audio_Volume->setMaximum(100);
    ui->horizontalSlider_Audio_Volume->setValue(30);
    audioOutput->setVolume(ui->horizontalSlider_Audio_Volume->value() / 100.0);

    // Koneksi untuk durasi dan posisi lagu
    connect(MPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(MPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);

    // Inisialisasi slider durasi
    ui->horizontalSlider_Audio_File_Duration->setRange(0, MPlayer->duration() / 1000);

    // Default tampilan di halaman lirik
    ui->stackedView->setCurrentWidget(ui->page_Lyrics);

    // Inisialisasi status shuffle dan repeat
    isShuffle = false; // Default: shuffle off
    isRepeat = false;  // Default: repeat off
    indeksLaguSaatIni = -1; // Inisialisasi indeks lagu saat ini
    IS_Muted = false;

    connect(ui->lyricsDisplay, &ClickableTextEdit::clickedAt,
            this, &MainWindow::on_lyricsDisplay_clicked);

    // Tangkap event scroll manual dari lirik
    connect(ui->lyricsDisplay, &ClickableTextEdit::userScrolled, this, [=]() {
        userScrollingLyrics = true;
        resumeAutoScrollTimer->start(3000); // kembali auto-scroll setelah 3 detik
    });

    // Timer untuk mengaktifkan kembali auto-scroll
    resumeAutoScrollTimer = new QTimer(this);
    resumeAutoScrollTimer->setSingleShot(true);
    connect(resumeAutoScrollTimer, &QTimer::timeout, this, [=]() {
        userScrollingLyrics = false;
    });

    // Timer untuk auto-keluar dari full lyric mode
    fullLyricAutoExitTimer = new QTimer(this);
    fullLyricAutoExitTimer->setSingleShot(true);
    connect(fullLyricAutoExitTimer, &QTimer::timeout, this, [=]() {
        fullLyricMode = false;
    });

    connect(ui->listWidget_Queue, &QListWidget::itemClicked,
            this, &MainWindow::on_listWidget_Queue_itemClicked);

    ui->listWidget->installEventFilter(this);


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
            format = "hh:mm:ss"; // Format untuk durasi lebih dari 1 jam

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
        tr("Audio Files (*.mp3 *.wav *.flac)")
        );

    if (fileNames.isEmpty())
        return;

    // Tambahkan lagu-lagu baru (hindari duplikasi)
    for (const QString &file : fileNames) {
        if (!daftarLagu.contains(file)) {
            daftarLagu << file;
        }
    }

    // Urutkan dan tampilkan ulang daftar
    urutkanDaftarLagu();

    // Putar lagu terakhir yang ditambahkan jika tidak ada lagu yang diputar
    // atau jika pengguna ingin lagu baru langsung diputar
    QString last = fileNames.last();
    int indeks = daftarLagu.indexOf(last);
    if (indeks != -1 && MPlayer->mediaStatus() == QMediaPlayer::NoMedia) {
        putarLaguPadaIndeks(indeks);
    } else if (indeks != -1 && !MPlayer->isPlaying()) {
        // Jika player sedang pause/stop tapi ada lagu baru ditambahkan
        putarLaguPadaIndeks(indeks);
    }
}

void MainWindow::on_pushButton_Play_clicked()
{
    if (MPlayer->playbackState() == QMediaPlayer::PausedState) {
        MPlayer->play(); // Lanjut dari posisi terakhir
    } else if (MPlayer->playbackState() != QMediaPlayer::PlayingState) {
        if (MPlayer->mediaStatus() == QMediaPlayer::EndOfMedia)
            MPlayer->setPosition(0); // Ulangi jika sudah selesai

        // Jika tidak ada lagu yang dipilih tapi daftar lagu ada, putar yang pertama
        if (MPlayer->mediaStatus() == QMediaPlayer::NoMedia && !daftarLagu.isEmpty()) {
            putarLaguPadaIndeks(0);
        } else {
            MPlayer->play();
        }
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
    catatDurasiDengar();
    MPlayer->stop();
    // Berhenti menampilkan lirik juga
    lyricsTimer->stop();
    ui->lyricsDisplay->setPlainText("");
    ui->label_File_Name->setText("Tidak ada lagu diputar");
    ui->horizontalSlider_Audio_File_Duration->setValue(0);
    updateduration(0);
}


void MainWindow::on_nextButton_clicked()
{
    if (!laguQueue.isEmpty()) {
        putarLaguDariQueue();
        return;
    }

    if (daftarLagu.isEmpty()) return;

    int nextIndex;

    if (isShuffle && daftarLagu.size() > 1) {
        do {
            nextIndex = QRandomGenerator::global()->bounded(daftarLagu.size());
        } while (nextIndex == indeksLaguSaatIni);
    } else {
        nextIndex = (indeksLaguSaatIni + 1) % daftarLagu.size();
    }

    putarLaguPadaIndeks(nextIndex);
}


void MainWindow::on_previousButton_clicked()
{
    if (daftarLagu.isEmpty()) return;

    int prevIndex;

    if (isShuffle && daftarLagu.size() > 1) {
        do {
            prevIndex = QRandomGenerator::global()->bounded(daftarLagu.size());
        } while (prevIndex == indeksLaguSaatIni);
    } else {
        prevIndex = (indeksLaguSaatIni - 1 + daftarLagu.size()) % daftarLagu.size();
    }

    putarLaguPadaIndeks(prevIndex);
}


void MainWindow::on_horizontalSlider_Audio_File_Duration_valueChanged(int value)
{
    MPlayer->setPosition(value * 1000);
}


void MainWindow::on_horizontalSlider_Audio_Volume_valueChanged(int value)
{
    audioOutput->setVolume(value / 100.0);
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> droppedUrls = event->mimeData()->urls();
    if (droppedUrls.isEmpty())
        return;

    QStringList laguBaru;

    for (const QUrl &url : droppedUrls) {
        QString filePath = url.toLocalFile();

        // Cek ekstensi file dan hindari duplikasi
        if ((filePath.endsWith(".mp3", Qt::CaseInsensitive) ||
             filePath.endsWith(".wav", Qt::CaseInsensitive) ||
             filePath.endsWith(".flac", Qt::CaseInsensitive))
            && !daftarLagu.contains(filePath))
        {
            daftarLagu.append(filePath);
            laguBaru.append(filePath);
        }
    }

    if (!laguBaru.isEmpty()) {
        urutkanDaftarLagu(); // Urutkan dan refresh listWidget

        // Putar lagu terakhir ditambahkan jika daftar lagu kosong sebelumnya atau tidak ada lagu aktif
        QFileInfo lastFile(laguBaru.last());
        int indexToPlay = -1;
        for (int i = 0; i < daftarLagu.size(); ++i) {
            if (QFileInfo(daftarLagu[i]).fileName() == lastFile.fileName()) {
                indexToPlay = i;
                break;
            }
        }

        if (indexToPlay != -1 && MPlayer->mediaStatus() == QMediaPlayer::NoMedia) {
            putarLaguPadaIndeks(indexToPlay);
        }
    } else {
        QMessageBox::information(this, "Info", "Tidak ada file audio valid yang ditambahkan atau file sudah ada di daftar.");
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        // Cek apakah ada file audio yang diseret
        bool hasAudio = false;
        for (const QUrl &url : event->mimeData()->urls()) {
            QString filePath = url.toLocalFile();
            if (filePath.endsWith(".mp3", Qt::CaseInsensitive) ||
                filePath.endsWith(".wav", Qt::CaseInsensitive) ||
                filePath.endsWith(".flac", Qt::CaseInsensitive)) {
                hasAudio = true;
                break;
            }
        }
        if (hasAudio) {
            event->acceptProposedAction();
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
    if (index < 0 || index >= daftarLagu.size()) return;

    QString path = daftarLagu[index];

    QMenu menu(this);
    QAction *putarLangsung = menu.addAction("Putar Sekarang");
    QAction *tambahkanKeQueueAct = menu.addAction("Tambahkan ke Queue");

    QAction *chosen = menu.exec(QCursor::pos());

    if (chosen == putarLangsung) {
        putarLaguPadaIndeks(index);
    } else if (chosen == tambahkanKeQueueAct) {
        tambahkanKeQueue(path);
        QMessageBox::information(this, "Queue", "Lagu telah ditambahkan ke antrian.");
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
        int index = daftarLagu.indexOf(path); // Temukan indeks di daftarLagu utama
        if (index != -1) {
            putarLaguPadaIndeks(index);
        }
    }
}


void MainWindow::putarLaguPadaIndeks(int indeks)
{
    catatDurasiDengar(); // Catat durasi lagu sebelumnya sebelum ganti

    if (daftarLagu.isEmpty()) return;

    if (indeks < 0 || indeks >= daftarLagu.size()) {
        // Atur kembali ke awal daftar jika melebihi batas (untuk kasus next/prev di akhir/awal)
        if (indeks >= daftarLagu.size()) {
            indeks = 0;
        } else if (indeks < 0) {
            indeks = daftarLagu.size() - 1;
        }
    }

    // Handle shuffle
    if (isShuffle && daftarLagu.size() > 1) {
        int randomIndex;
        do {
            randomIndex = QRandomGenerator::global()->bounded(daftarLagu.size());
        } while (randomIndex == indeksLaguSaatIni); // Hindari pengulangan lagu sama persis
        indeks = randomIndex;
    }

    indeksLaguSaatIni = indeks;
    QString path = daftarLagu[indeks];
    MPlayer->setSource(QUrl::fromLocalFile(path));
    MPlayer->play();

    waktuMulaiDiputar = QDateTime::currentDateTime();
    judulLaguDiputar = QFileInfo(path).fileName();

    // Tambahkan logika play count
    QString fileName = QFileInfo(path).fileName();
    qint64 currentDurationMs = MPlayer->duration(); // durasi lagu (ms)

    playCountMap[fileName]++;
    playDurationMap[fileName] += currentDurationMs;
    lastPlayedMap[fileName] = QDateTime::currentDateTime();


    QFileInfo fileInfo(path);
    ui->label_File_Name->setText(fileInfo.fileName());
    ui->listWidget->setCurrentRow(indeks); // Highlight lagu yang sedang diputar di playlist

    // Ambil dan tampilkan lirik lagu
    QString judulLagu = fileInfo.baseName();
    fetchLyrics(judulLagu);
}

void MainWindow::on_pushButton_Shuffle_toggled(bool checked)
{
    isShuffle = checked;
    if (isShuffle) {
        ui->pushButton_Shuffle->setStyleSheet("background-color: #FFA500; color: white;"); // Contoh highlight orange
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
        catatDurasiDengar();

        if (isRepeat) {
            MPlayer->setPosition(0);
            MPlayer->play();

            // Tambah statistik play dan reset waktu mulai
            QString fileName = QFileInfo(daftarLagu[indeksLaguSaatIni]).fileName();
            playCountMap[fileName]++;
            waktuMulaiDiputar = QDateTime::currentDateTime();
            judulLaguDiputar = fileName;
        }
        else {
            if (!laguQueue.isEmpty()) {
                putarLaguDariQueue(); // putar queue dulu
            } else {
                int nextIndex = indeksLaguSaatIni + 1;

                if (isShuffle && daftarLagu.size() > 1) {
                    do {
                        nextIndex = QRandomGenerator::global()->bounded(daftarLagu.size());
                    } while (nextIndex == indeksLaguSaatIni);
                }

                if (nextIndex < daftarLagu.size()) {
                    putarLaguPadaIndeks(nextIndex);
                } else {
                    MPlayer->stop();
                    lyricsTimer->stop();
                    ui->lyricsDisplay->setPlainText("");
                    ui->label_File_Name->setText("Tidak ada lagu diputar");
                }
            }
        }
    }

    if (MPlayer->playbackState() == QMediaPlayer::PlayingState) {
        if (!lyricsTimer->isActive() && !syncedLyricsData.isEmpty()) {
            lyricsTimer->start();
        }
    } else if (MPlayer->playbackState() == QMediaPlayer::StoppedState ||
               MPlayer->playbackState() == QMediaPlayer::PausedState ||
               status == QMediaPlayer::NoMedia) {
        lyricsTimer->stop();
    }
}


void MainWindow::on_searchLineEdit_textChanged(const QString &query) {
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
            ui->stackedView->setCurrentWidget(ui->page_Lyrics);
            ui->lyricsDisplay->setPlainText("Tidak ada hasil untuk pencarian Anda.");
        }
    } else {
        ui->stackedView->setCurrentWidget(ui->page_Lyrics);  // Kosongkan pencarian, kembali ke lirik
        // Bersihkan lirik jika lagu tidak diputar (atau sedang dijeda/berhenti)
        if (MPlayer->playbackState() != QMediaPlayer::PlayingState) {
            ui->lyricsDisplay->setPlainText("");
        }
    }
}



void MainWindow::fetchLyrics(const QString &fullTitle) {
    // 1. Coba cari dan muat file .lrc lokal terlebih dahulu
    // Pastikan indeksLaguSaatIni valid sebelum mengakses daftarLagu
    if (indeksLaguSaatIni < 0 || indeksLaguSaatIni >= daftarLagu.size()) {
        qDebug() << "fetchLyrics: No song selected or invalid index, cannot check for local LRC.";
        ui->lyricsDisplay->setPlainText("Silakan pilih lagu untuk menampilkan lirik.");
        syncedLyricsData.clear(); // Bersihkan lirik lama
        currentLyricIndex = 0;
        lyricsTimer->stop();
        return;
    }

    QFileInfo currentSongInfo(daftarLagu[indeksLaguSaatIni]); // Ambil info lagu yang sedang diputar
    QString lrcFilePath = currentSongInfo.absolutePath() + "/" + currentSongInfo.baseName() + ".lrc";

    qDebug() << "Checking for LRC file at:" << lrcFilePath;

    QFile lrcFile(lrcFilePath);
    if (lrcFile.exists() && lrcFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString lrcContent = lrcFile.readAll();
        lrcFile.close();
        qDebug() << "LRC file found and loaded. Content length:" << lrcContent.length();
        // Untuk melihat sebagian isi, tapi hati-hati dengan lirik yang sangat panjang
        qDebug() << "First 200 chars of LRC content:" << lrcContent.left(200) << "...";

        // Bersihkan lirik lama dan tampilkan pesan loading dari LRC
        ui->lyricsDisplay->setPlainText("Memuat lirik sinkron dari file lokal...");
        syncedLyricsData.clear();
        currentLyricIndex = 0;
        lyricsTimer->stop();

        loadSyncedLyrics(lrcContent); // Langsung muat lirik dari file LRC

        // Efek fade-in setelah teks muncul (aplikasikan ke ui->lyricsDisplay)
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
        ui->lyricsDisplay->setGraphicsEffect(effect);
        QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
        animation->setDuration(600);
        animation->setStartValue(0);
        animation->setEndValue(1);
        animation->start(QAbstractAnimation::DeleteWhenStopped);

        qDebug() << "Successfully processed local LRC file. Returning.";
        return; // Jika LRC ditemukan dan dimuat, jangan lanjutkan ke API
    } else {
        qDebug() << "LRC file not found or could not be opened for:" << lrcFilePath << ". Falling back to API.";
    }

    // 2. Jika file .lrc lokal tidak ditemukan, baru ambil dari API (logic yang sudah ada)
    QString cleaned = fullTitle;
    cleaned.remove("(Lyrics)", Qt::CaseInsensitive).trimmed();

    QString artist, title;
    QStringList parts = cleaned.split(" - ");
    if (parts.size() >= 2) {
        artist = parts[0].trimmed();
        title  = parts[1].trimmed();
    } else {
        artist = "Unknown";
        title  = cleaned;
    }

    QString apiUrl = QString("https://api.lyrics.ovh/v1/%1/%2")
                         .arg(QUrl::toPercentEncoding(artist))
                         .arg(QUrl::toPercentEncoding(title));

    ui->lyricsDisplay->setPlainText("Mencari lirik online..."); // Pesan loading untuk API
    syncedLyricsData.clear();
    currentLyricIndex = 0;
    lyricsTimer->stop();

    qDebug() << "Fetching lyrics from API:" << apiUrl;

    QNetworkRequest request{ QUrl(apiUrl) };
    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QJsonObject obj = doc.object();
            QString lyricsText = obj.value("lyrics").toString();

            qDebug() << "API Response Lyrics (first 200 chars):" << lyricsText.left(200) << "...";

            if (!lyricsText.isEmpty()) {
                ui->lyricsDisplay->setPlainText(lyricsText); // Tampilkan plain text dari API
                loadSyncedLyrics(lyricsText); // Coba parsing lirik sinkron (ini akan gagal jika API hanya plain text)

                // Efek fade-in setelah teks muncul
                QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
                ui->lyricsDisplay->setGraphicsEffect(effect);
                QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
                animation->setDuration(600);
                animation->setStartValue(0);
                animation->setEndValue(1);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                qDebug() << "Successfully processed API lyrics. Fade-in animation started.";

            } else {
                ui->lyricsDisplay->setPlainText("Lirik tidak ditemukan untuk lagu ini.");
                qDebug() << "Lyrics not found for:" << fullTitle << " from API.";
            }
        } else {
            ui->lyricsDisplay->setPlainText("Gagal mengambil lirik: " + reply->errorString());
            qDebug() << "Failed to fetch lyrics from API:" << reply->errorString();
        }
        reply->deleteLater();
    });
}


void MainWindow::on_pushButton_DaftarLagu_clicked()
{
    ui->stackedView->setCurrentWidget(ui->page_DaftarLagu); // Tampilkan daftar lagu utama
}

void MainWindow::on_pushButton_ListWidgetQueue_clicked()
{
    ui->stackedView->setCurrentWidget(ui->page_ListQueue); // Tampilkan daftar antrian
}

void MainWindow::on_pushButton_KembaliKeLirik_clicked()
{
    ui->stackedView->setCurrentWidget(ui->page_Lyrics); // Kembali ke halaman lirik
}

void MainWindow::urutkanDaftarLagu()
{
    // Menggunakan std::sort pada daftarLagu langsung
    // Dengan lambda untuk membandingkan nama file tanpa path
    std::sort(daftarLagu.begin(), daftarLagu.end(), [](const QString &a, const QString &b) {
        return QFileInfo(a).fileName().toLower() < QFileInfo(b).fileName().toLower();
    });

    // Tampilkan di listWidget
    ui->listWidget->clear();
    for (const QString &path : daftarLagu) {
        ui->listWidget->addItem(QFileInfo(path).fileName());
    }
}

// mainwindow.cpp - di dalam loadSyncedLyrics
void MainWindow::loadSyncedLyrics(const QString& lyricsContent)
{
    syncedLyricsData.clear();
    currentLyricIndex = 0;

    QRegularExpression rx("\\[(\\d+):(\\d+)\\.?(\\d*)\\]"); // Regex untuk [mm:ss.xx] atau [mm:ss]
    QRegularExpression tagRx("\\[(ti|ar|al|by|offset):.*\\]", QRegularExpression::CaseInsensitiveOption); // Regex untuk tag seperti [ti:title], [ar:artist]

    QStringList lines = lyricsContent.split('\n');
    bool timestampsFound = false;

    qDebug() << "loadSyncedLyrics: Starting parsing for lyrics content (length:" << lyricsContent.length() << ").";
    qDebug() << "loadSyncedLyrics: Number of lines to process:" << lines.size();

    for (const QString &line : lines) {
        QString trimmedLine = line.trimmed();
        if (tagRx.match(trimmedLine).hasMatch()) {
            qDebug() << "loadSyncedLyrics: Skipping tag line:" << trimmedLine;
            continue;
        }

        QRegularExpressionMatchIterator i = rx.globalMatch(line);
        QVector<qint64> lineTimestamps; // Untuk menyimpan semua timestamps yang ditemukan di BARIS ini

        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            timestampsFound = true; // Set flag jika timestamp ditemukan

            int min = match.captured(1).toInt();
            int sec = match.captured(2).toInt();
            int msec = match.captured(3).toInt();
            // Handle milidetik: .x -> .x00, .xx -> .xx0, .xxx -> .xxx
            if (match.captured(3).length() == 1) msec *= 100;
            else if (match.captured(3).length() == 2) msec *= 10;
            // Jika .lrc menggunakan 3 digit milidetik, tidak perlu dikalikan.

            qint64 timeMs = min * 60000 + sec * 1000 + msec;
            lineTimestamps.append(timeMs);
            qDebug() << "  Found timestamp:" << match.captured(0) << " -> " << timeMs << "ms";
        }

        // Ambil teks lirik setelah menghapus semua timestamp dari baris
        QString lyricTextOnly = line;
        lyricTextOnly.remove(QRegularExpression("\\[\\d+:\\d+\\.?\\d*\\]")); // Hapus semua timestamps
        lyricTextOnly = lyricTextOnly.trimmed();

        if (!lyricTextOnly.isEmpty()) {
            // Jika ada timestamps di baris ini, buat entri untuk setiap timestamp
            if (!lineTimestamps.isEmpty()) {
                for(qint64 ts : lineTimestamps) {
                    syncedLyricsData.append({ts, lyricTextOnly});
                    qDebug() << "  Added synced lyric data: [" << ts << "] " << lyricTextOnly;
                }
            } else {
                // Ini adalah baris lirik tanpa timestamp. Kita bisa abaikan untuk sinkronisasi,
                // atau menambahkannya dengan timestamp 0 jika ingin tampil di awal.
                // Untuk animasi, kita hanya fokus pada yang ada timestamp.
                qDebug() << "  Skipping non-synced lyric line (no timestamp):" << lyricTextOnly;
            }
        }
    }

    // Urutkan seluruh data berdasarkan timestamp
    std::sort(syncedLyricsData.begin(), syncedLyricsData.end(), [](const LyricLine &a, const LyricLine &b) {
        return a.timestamp < b.timestamp;
    });

    qDebug() << "loadSyncedLyrics: Parsing complete.";
    qDebug() << "loadSyncedLyrics: Timestamps found overall:" << timestampsFound;
    qDebug() << "loadSyncedLyrics: Number of synced lines in syncedLyricsData:" << syncedLyricsData.size();

    if (timestampsFound && !syncedLyricsData.isEmpty()) {
        lyricsTimer->start();
        updateLyricDisplay(MPlayer->position()); // Panggil sekali untuk menampilkan baris pertama
        qDebug() << "Lyrics timer started successfully.";
    } else {
        lyricsTimer->stop();
        // Jangan setPlainText di sini, biarkan lirik plain text dari fetchLyrics tetap tampil.
        qDebug() << "No synced lyrics found or parsed. Lyrics timer stopped.";
    }
}


// mainwindow.cpp - di dalam updateLyricDisplay
void MainWindow::updateLyricDisplay(qint64 currentPosition)
{
    if (fullLyricMode) return; // Kalau mode full aktif, jangan update otomatis
    if (userScrollingLyrics) return; // Pause auto-scroll jika user sedang scroll manual

    qDebug() << "updateLyricDisplay called. userScrollingLyrics =" << userScrollingLyrics;

    if (syncedLyricsData.isEmpty()) {
        qDebug() << "updateLyricDisplay: syncedLyricsData is empty, returning. No synced lyrics to display.";
        return;
    }

    int newLyricIndex = -1;
    // Cari indeks baris lirik yang paling sesuai menggunakan binary search
    // std::upper_bound mengembalikan iterator ke elemen pertama yang > value
    auto it = std::upper_bound(syncedLyricsData.begin(), syncedLyricsData.end(), currentPosition + lyricOffsetMs,
                               [](qint64 pos, const LyricLine &line) {
                                   return pos < line.timestamp;
                               });

    if (it != syncedLyricsData.begin()) {
        --it; // Mundur satu untuk mendapatkan baris yang sedang aktif atau baris sebelumnya
        newLyricIndex = std::distance(syncedLyricsData.begin(), it);
    } else {
        newLyricIndex = 0; // Jika posisi lagu di awal sekali, anggap baris pertama aktif
    }

    qDebug() << "updateLyricDisplay: Calculated newLyricIndex =" << newLyricIndex << ", currentLyricIndex (previous) =" << currentLyricIndex;

    if (newLyricIndex != -1 && newLyricIndex != currentLyricIndex) {
        currentLyricIndex = newLyricIndex;
        qDebug() << "updateLyricDisplay: Lyric index changed to:" << currentLyricIndex;

        QStringList displayLines;
        int linesToShowBefore = 2;
        int linesToShowAfter = 2;

        int startIndex = qMax(0, currentLyricIndex - linesToShowBefore);
        int endIndex = qMin(syncedLyricsData.size() - 1, currentLyricIndex + linesToShowAfter);

        qDebug() << "updateLyricDisplay: Display range from line" << startIndex << "to" << endIndex;

        for (int i = startIndex; i <= endIndex; ++i) {
            const LyricLine &line = syncedLyricsData[i]; // Akses langsung dari QVector
            QString lyricText = line.text;

            if (i == currentLyricIndex) {
                displayLines.append(QString("<font color='#00cc66'><b>%1</b></font>").arg(lyricText));
            } else {
                displayLines.append(QString("<font color='gray'>%1</font>").arg(lyricText));
            }
        }

        QString fullHtmlContent = displayLines.join("<br>");
        ui->lyricsDisplay->setHtml(fullHtmlContent);
        qDebug() << "updateLyricDisplay: UI updated with new HTML content (first 100 chars):" << fullHtmlContent.left(100) << "...";

        // Logic untuk menggulir ke baris yang aktif
        QTextCursor cursor = ui->lyricsDisplay->textCursor();
        cursor.clearSelection();
        cursor.movePosition(QTextCursor::Start); // Pindahkan kursor ke awal dokumen
        ui->lyricsDisplay->setTextCursor(cursor);

        QTextDocument *doc = ui->lyricsDisplay->document();
        // Baris aktif dalam konteks yang ditampilkan, bukan indeks global
        // Misalnya, jika startIndex = 0 dan currentLyricIndex = 2, maka baris aktif adalah baris ke-2 (indeks 2)
        // dari teks yang sedang ditampilkan.
        QTextBlock block = doc->findBlockByLineNumber(currentLyricIndex - startIndex);

        if (block.isValid()) {
            QTextCursor scrollCursor(block);
            ui->lyricsDisplay->setTextCursor(scrollCursor);
            ui->lyricsDisplay->ensureCursorVisible(); // Gulir agar kursor terlihat
            qDebug() << "updateLyricDisplay: Scrolled to active lyric block.";
        } else {
            qDebug() << "updateLyricDisplay: Block for scrolling is not valid.";
        }

        qDebug() << "Displaying synced lyric (full lines):" << displayLines.join(" | ");
    } else {
        qDebug() << "updateLyricDisplay: Lyric index did not change or no valid new index. No UI update needed.";
    }
}

void MainWindow::on_lyricsDisplay_clicked(QMouseEvent *event)
{
    QTextCursor cursor = ui->lyricsDisplay->cursorForPosition(event->pos());
    QTextBlock block = cursor.block();
    QString html = block.text();

    if (fullLyricMode) {
        fullLyricAutoExitTimer->start(3000); // Restart timer tiap klik

        QTextDocument *doc = ui->lyricsDisplay->document();
        QTextBlock clickedBlock = doc->findBlock(cursor.position());

        if (clickedBlock.isValid()) {
            QString clickedLine = clickedBlock.text();
            for (const LyricLine &line : syncedLyricsData) {
                if (line.text.trimmed() == clickedLine.trimmed()) {
                    MPlayer->setPosition(line.timestamp);
                    break;
                }
            }

            // Jika klik kosong, keluar manual
            if (clickedLine.trimmed().isEmpty()) {
                fullLyricMode = false;
                fullLyricAutoExitTimer->stop(); // stop biar tidak aktif dobel
            }
        }
    } else {
        // MASUK KE MODE FULL
        fullLyricMode = true;
        tampilkanSemuaLirikDenganKlik();
    }
}


void MainWindow::tampilkanSemuaLirikDenganKlik()
{
    ui->lyricsDisplay->clear();

    QString htmlContent;
    for (const LyricLine &line : syncedLyricsData) {
        htmlContent += QString("<p data-timestamp='%1'>%2</p>")
        .arg(line.timestamp)
            .arg(line.text.toHtmlEscaped());
    }

    ui->lyricsDisplay->setHtml(htmlContent);
}

void MainWindow::perbaruiTampilanQueue()
{
    ui->listWidget_Queue->clear();
    for (const QString &lagu : laguQueue) {
        ui->listWidget_Queue->addItem(QFileInfo(lagu).fileName());
    }
}


void MainWindow::putarLaguDariQueue()
{
    if (laguQueue.isEmpty()) return;

    QString path = laguQueue.dequeue(); // Ambil lagu pertama
    int index = daftarLagu.indexOf(path);
    if (index != -1) {
        putarLaguPadaIndeks(index);
    }
    perbaruiTampilanQueue(); // kalau ada list tampilan queue
}


void MainWindow::tambahkanKeQueue(const QString &path)
{
    if (!laguQueue.contains(path)) {
        laguQueue.enqueue(path);
        perbaruiTampilanQueue(); // Jika ingin update list tampilan antrian
    }
}

void MainWindow::hapusLaguDariQueue(const QString &path)
{
    laguQueue.removeAll(path);
    perbaruiTampilanQueue();
}

void MainWindow::clearQueue()
{
    laguQueue.clear();
    perbaruiTampilanQueue();
    QMessageBox::information(this, "Queue", "Antrian lagu telah dikosongkan.");
}

void MainWindow::on_listWidget_Queue_itemClicked(QListWidgetItem *item)
{
    QString fileName = item->text();

    // Temukan path berdasarkan nama file
    QString pathToRemove;
    for (const QString &path : laguQueue) {
        if (QFileInfo(path).fileName() == fileName) {
            pathToRemove = path;
            break;
        }
    }

    if (pathToRemove.isEmpty()) return;

    QMenu menu(this);
    QAction *hapus = menu.addAction("Hapus dari Queue");
    QAction *clearAll = menu.addAction("Kosongkan Semua");

    QAction *chosen = menu.exec(QCursor::pos());

    if (chosen == hapus) {
        hapusLaguDariQueue(pathToRemove);
    } else if (chosen == clearAll) {
        clearQueue();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->listWidget && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            int currentRow = ui->listWidget->currentRow();
            if (currentRow >= 0)
                putarLaguPadaIndeks(currentRow);
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::updateDaftarLaguSetelahDrag() {
    QStringList urutanBaru;
    for (int i = 0; i < ui->listWidget_Queue->count(); ++i) {
        QString namaFile = ui->listWidget_Queue->item(i)->text();
        for (const QString &path : daftarLagu) {
            if (QFileInfo(path).fileName() == namaFile) {
                urutanBaru << path;
                break;
            }
        }
    }

    if (!urutanBaru.isEmpty()) {
        QString currentPath = daftarLagu[indeksLaguSaatIni];
        daftarLagu = urutanBaru;

        indeksLaguSaatIni = daftarLagu.indexOf(currentPath);
        if (indeksLaguSaatIni == -1) {
            // Lagu saat ini tidak ditemukan di daftar baru
            // Pilih lagu default, misalnya lagu pertama, atau set ke 0 agar tidak error
            indeksLaguSaatIni = 0;
            qWarning() << "Lagu saat ini tidak ditemukan setelah drag, set indeks ke 0";
        }
        qDebug() << "Daftar lagu dan indeks lagu saat ini telah diperbarui setelah drag.";
    }
}

void MainWindow::on_pushButton_DownloadReport_clicked()
{
    if (playCountMap.empty()) {
        QMessageBox::information(this, "Info", "Belum ada lagu yang diputar.");
        return;
    }

    QString dirPath = QFileDialog::getExistingDirectory(this, "Pilih Folder Untuk Menyimpan Laporan");
    if (dirPath.isEmpty()) return;

    QVector<QString> sortedKeys;
    for (const auto &pair : playCountMap) {
        sortedKeys.append(pair.first);
    }

    std::sort(sortedKeys.begin(), sortedKeys.end(), [=](const QString &a, const QString &b) {
        return playCountMap[b] < playCountMap[a]; // descending
    });

    // === CSV ===
    QFile csvFile(dirPath + "/laporan_pemutaran.csv");
    if (csvFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&csvFile);
        out << "Peringkat,Judul Lagu,Jumlah Diputar,Total Durasi,Terakhir Diputar\n";
        for (int i = 0; i < sortedKeys.size(); ++i) {
            QString judul = sortedKeys[i];
            int playCount = playCountMap[judul];
            qint64 dur = playDurationMap[judul];
            QDateTime lastPlayed = lastPlayedMap[judul];

            QTime timeDur = QTime(0, 0).addMSecs(dur);
            QString durStr = timeDur.toString("hh:mm:ss");

            out << i + 1 << ",\"" << judul << "\"," << playCount << "," << durStr << "," << lastPlayed.toString("dd-MM-yyyy hh:mm") << "\n";
        }
        csvFile.close();
    }

    // === TXT ===
    QFile txtFile(dirPath + "/laporan_pemutaran.txt");
    if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&txtFile);
        out << "📊 Laporan Statistik Lagu Diputar\n";
        out << "Tanggal Dibuat: " << QDate::currentDate().toString("dd MMMM yyyy") << "\n\n";
        out << "Berikut adalah daftar lagu yang paling sering diputar:\n\n";

        for (int i = 0; i < sortedKeys.size(); ++i) {
            QString judul = sortedKeys[i];
            int count = playCountMap[judul];
            qint64 dur = playDurationMap[judul];
            QDateTime lastPlayed = lastPlayedMap[judul];

            QTime timeDur = QTime(0, 0).addMSecs(dur);
            QString durStr = timeDur.toString("hh:mm:ss");
            QString lastPlayedStr = lastPlayed.toString("dd-MM-yyyy hh:mm");

            QString dots = QString(30 - judul.length(), '.');
            out << i + 1 << ". " << judul << " " << dots << " " << count << " kali\n";
            out << "    Total durasi didengar: " << durStr << "\n";
            out << "    Terakhir diputar     : " << lastPlayedStr << "\n\n";
        }

        out << "Terima kasih telah menggunakan MusicPlayer!\n";
        txtFile.close();
    }

    QMessageBox::information(this, "Sukses", "Laporan berhasil disimpan dalam format CSV dan TXT!");
}

void MainWindow::catatDurasiDengar()
{
    if (judulLaguDiputar.isEmpty()) return;

    qint64 durasi = waktuMulaiDiputar.msecsTo(QDateTime::currentDateTime());
    playDurationMap[judulLaguDiputar] += durasi;
    lastPlayedMap[judulLaguDiputar] = QDateTime::currentDateTime();

    // Reset
    judulLaguDiputar.clear();
    waktuMulaiDiputar = QDateTime();
}



