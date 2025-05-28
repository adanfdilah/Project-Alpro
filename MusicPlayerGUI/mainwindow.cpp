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
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMessageBox>
#include <QFileInfo>
#include <QRandomGenerator>
#include <algorithm> // Untuk std::sort (meskipun akan dihapus dari urutkanDaftarLagu()
#include <QUrl>
#include <QTime>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>
#include <QTextBlock>
#include <QFile>
#include <QTextCursor>
#include <QTextDocument>
// #include <QVBoxLayout> // Dihapus karena ui->listWidget sudah dari Designer
// #include <QWidget>     // Dihapus karena ui->listWidget sudah dari Designer

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

    // --- BAGIAN INI DIHAPUS KARENA DUPLIKASI QListWidget ---
    // Kode ini membuat QListWidget baru yang tidak terhubung ke UI Designer Anda.
    // QWidget *centralWidget = new QWidget(this);
    // setCentralWidget(centralWidget);
    // QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    // QListWidget *playlistWidget = new QListWidget(this);
    // mainLayout->addWidget(playlistWidget);
    // centralWidget->setLayout(mainLayout);
    // --- AKHIR BAGIAN YANG DIHAPUS ---

    // --- PENTING: Pengaturan Drag and Drop untuk ui->listWidget ---
    // ui->listWidget adalah QListWidget yang Anda definisikan di UI Designer.
    ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove); // Mengizinkan drag-and-drop di dalam list
    ui->listWidget->setDragEnabled(true); // Memungkinkan item untuk diseret
    // ui->listWidget->setDropEnabled(true); // BARIS INI MENYEBABKAN ERROR, DIHAPUS
    ui->listWidget->setDefaultDropAction(Qt::MoveAction); // Mengindikasikan aksi pemindahan

    // --- PENTING: Koneksi untuk sync daftarLagu saat playlist di-drag-drop di UI ---
    // Ketika baris dipindahkan di model QListWidget, kita sync daftarLagu internal.
    // QListWidget menggunakan QAbstractListModel sebagai modelnya.
    connect(ui->listWidget->model(), &QAbstractItemModel::rowsMoved,
            this, &MainWindow::syncDaftarLaguWithListWidget);

    // Koneksi untuk update lirik (menggunakan lyricsTimer yang sudah ada)
    lyricsTimer = new QTimer(this);
    lyricsTimer->setInterval(100); // Cek tiap 100ms posisi lagu untuk update lirik
    connect(lyricsTimer, &QTimer::timeout, this, [this]() {
        qint64 pos = MPlayer->position();
        updateLyricDisplay(pos);
    });

    // Koneksi untuk status media player
    connect(MPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::handleMediaStatusChanged);

    // Koneksi klik item di listWidget untuk putar lagu
    // Gunakan itemClicked daripada currentRowChanged untuk trigger putarLaguPadaIndeks
    connect(ui->listWidget, &QListWidget::itemClicked,
            this, &MainWindow::on_listWidget_itemClicked);

    // Setup timer untuk sleep function
    stopTimer = new QTimer(this);
    stopTimer->setSingleShot(true);
    connect(stopTimer, &QTimer::timeout, this, &MainWindow::handleTimerTimeout);

    // Aktifkan drag and drop untuk jendela utama (untuk file eksternal)
    setAcceptDrops(true); // Ini mengizinkan drop file dari File Explorer ke jendela utama

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

    // Inisialisasi QNetworkAccessManager untuk fetch lirik
    networkManager = new QNetworkAccessManager(this);

    // Setup lyricsDisplay (QPlainTextEdit)
    ui->lyricsDisplay->setReadOnly(true);
    ui->lyricsDisplay->setFrameShape(QFrame::NoFrame);
    ui->lyricsDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
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

    bool newSongsAdded = false;
    for (const QString &file : fileNames) {
        if (!daftarLagu.contains(file)) { // Hindari duplikasi di daftarLagu internal
            daftarLagu.append(file); // Tambahkan ke daftarLagu internal
            QListWidgetItem *item = new QListWidgetItem(QFileInfo(file).fileName());
            item->setData(Qt::UserRole, file); // PENTING: Simpan path lengkap di UserRole
            ui->listWidget->addItem(item); // Tambahkan ke tampilan UI
            newSongsAdded = true;
        }
    }

    if (newSongsAdded) {
        // TIDAK PERLU urutkanDaftarLagu() lagi di sini jika Anda ingin mempertahankan urutan drag and drop
        // Lagu baru akan ditambahkan di akhir playlist dan itu urutan yang diinginkan.
    }

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
    if (daftarLagu.isEmpty()) return;

    int nextIndex;

    if (isShuffle && daftarLagu.size() > 1) {
        do {
            nextIndex = QRandomGenerator::global()->bounded(daftarLagu.size());
        } while (nextIndex == indeksLaguSaatIni); // Hindari pengulangan lagu sama
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
    // Hanya set posisi jika slider tidak sedang ditekan oleh pengguna
    if (!ui->horizontalSlider_Audio_File_Duration->isSliderDown())
    {
        MPlayer->setPosition(value * 1000);
    }
}


void MainWindow::on_horizontalSlider_Audio_Volume_valueChanged(int value)
{
    audioOutput->setVolume(value / 100.0);
}

// Fungsi untuk menerima file yang di-drop dari luar aplikasi
void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> droppedUrls = event->mimeData()->urls();
    if (droppedUrls.isEmpty())
        return;

    bool newSongsAdded = false;
    QString lastDroppedFilePath; // Untuk menyimpan path dari lagu terakhir yang di-drop

    for (const QUrl &url : droppedUrls) {
        QString filePath = url.toLocalFile();

        // Cek ekstensi file dan hindari duplikasi
        if ((filePath.endsWith(".mp3", Qt::CaseInsensitive) ||
             filePath.endsWith(".wav", Qt::CaseInsensitive) ||
             filePath.endsWith(".flac", Qt::CaseInsensitive))
            && !daftarLagu.contains(filePath))
        {
            daftarLagu.append(filePath); // Tambahkan ke daftarLagu internal
            QListWidgetItem *item = new QListWidgetItem(QFileInfo(filePath).fileName());
            item->setData(Qt::UserRole, filePath); // PENTING: Simpan path lengkap di UserRole
            ui->listWidget->addItem(item); // Tambahkan ke tampilan UI
            newSongsAdded = true;
            lastDroppedFilePath = filePath;
        }
    }

    if (newSongsAdded) {
        // TIDAK PERLU urutkanDaftarLagu() di sini untuk drag-and-drop eksternal.
        // Lagu baru akan ditambahkan di akhir playlist.

        // Putar lagu terakhir ditambahkan jika daftar lagu kosong sebelumnya atau tidak ada lagu aktif
        int indexToPlay = daftarLagu.indexOf(lastDroppedFilePath); // Cari indeksnya di daftarLagu
        if (indexToPlay != -1 && MPlayer->mediaStatus() == QMediaPlayer::NoMedia) {
            putarLaguPadaIndeks(indexToPlay);
        } else if (indexToPlay != -1 && !MPlayer->isPlaying()) {
            putarLaguPadaIndeks(indexToPlay);
        }
    } else {
        QMessageBox::information(this, "Info", "Tidak ada file audio valid yang ditambahkan atau file sudah ada di daftar.");
    }
    event->acceptProposedAction(); // Penting untuk menerima drop event
}

// Fungsi untuk menandakan bahwa sesuatu diseret ke dalam jendela
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

// Slot untuk menangani klik pada item di listWidget
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    // Mengambil path lengkap dari data item, ini lebih robust
    QString path = item->data(Qt::UserRole).toString();
    if (!path.isEmpty()) {
        int index = daftarLagu.indexOf(path); // Temukan indeks di daftarLagu utama
        if (index != -1) {
            putarLaguPadaIndeks(index); // Langsung panggil fungsi putar
        } else {
            qDebug() << "Error: Clicked item path not found in daftarLagu:" << path;
        }
    } else {
        qDebug() << "Error: Clicked item has no path in UserRole:" << item->text();
    }
}

void MainWindow::on_listWidget_SearchResult_itemClicked(QListWidgetItem *item)
{
    QString fileName = item->text();
    QString path;

    // Cari path lengkap dari hasilPencarian berdasarkan nama file
    for (const QString &p : qAsConst(hasilPencarian)) {
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
    if (daftarLagu.isEmpty()) {
        qDebug() << "putarLaguPadaIndeks: daftarLagu is empty.";
        return;
    }

    // Pastikan indeks berada dalam batas yang valid setelah potensi shuffle
    if (indeks < 0 || indeks >= daftarLagu.size()) {
        // Ini lebih ke fallback untuk kasus next/previous, shuffle akan handle indeksnya sendiri
        if (indeks >= daftarLagu.size()) {
            indeks = 0; // Kembali ke awal
        } else if (indeks < 0) {
            indeks = daftarLagu.size() - 1; // Kembali ke akhir
        }
        qDebug() << "putarLaguPadaIndeks: Index out of bounds, adjusted to" << indeks;
    }

    // Handle shuffle (ini akan mengubah 'indeks' jika shuffle aktif)
    if (isShuffle && daftarLagu.size() > 1) {
        int randomIndex;
        do {
            randomIndex = QRandomGenerator::global()->bounded(daftarLagu.size());
        } while (randomIndex == indeksLaguSaatIni); // Hindari pengulangan lagu sama persis
        indeks = randomIndex; // Gunakan indeks acak yang baru
        qDebug() << "Shuffle is ON. Playing random song at index:" << indeks;
    }

    indeksLaguSaatIni = indeks;
    QString path = daftarLagu[indeksLaguSaatIni]; // Ambil path dari daftarLagu internal yang sudah disinkronkan
    MPlayer->setSource(QUrl::fromLocalFile(path));
    MPlayer->play();

    QFileInfo fileInfo(path);
    ui->label_File_Name->setText(fileInfo.fileName());
    ui->listWidget->setCurrentRow(indeksLaguSaatIni); // Highlight lagu yang sedang diputar di playlist

    // Ambil dan tampilkan lirik lagu
    QString judulLagu = fileInfo.baseName();
    fetchLyrics(judulLagu);
    qDebug() << "Playing song:" << fileInfo.fileName() << "at index:" << indeksLaguSaatIni;
}

void MainWindow::on_pushButton_Shuffle_toggled(bool checked)
{
    isShuffle = checked;
    if (isShuffle) {
        ui->pushButton_Shuffle->setStyleSheet("background-color: #FFA500; color: white;"); // Contoh highlight orange
        qDebug() << "Shuffle ON";
    } else {
        ui->pushButton_Shuffle->setStyleSheet(""); // Reset style
        qDebug() << "Shuffle OFF";
    }
}

void MainWindow::on_pushButton_Repeat_toggled(bool checked)
{
    isRepeat = checked;

    if (checked) {
        ui->pushButton_Repeat->setStyleSheet("background-color: #00cc66; color: white;");
        qDebug() << "Repeat ON";
    } else {
        ui->pushButton_Repeat->setStyleSheet("");
        qDebug() << "Repeat OFF";
    }
}

void MainWindow::handleMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qDebug() << "MediaStatusChanged: " << status;
    if (status == QMediaPlayer::EndOfMedia)
    {
        if (isRepeat) {
            qDebug() << "End of media, Repeat is ON. Replaying current song.";
            MPlayer->setPosition(0);
            MPlayer->play();
        }
        else {
            qDebug() << "End of media, Repeat is OFF. Moving to next song.";
            int nextIndex = indeksLaguSaatIni + 1;

            if (isShuffle && daftarLagu.size() > 1) {
                do {
                    nextIndex = QRandomGenerator::global()->bounded(daftarLagu.size());
                } while (nextIndex == indeksLaguSaatIni); // Hindari pengulangan lagu sama
                qDebug() << "Shuffle active, next random index:" << nextIndex;
            }

            if (nextIndex < daftarLagu.size()) {
                putarLaguPadaIndeks(nextIndex);
            } else {
                // Jika sudah lagu terakhir, berhenti saja
                qDebug() << "End of playlist. Stopping playback.";
                MPlayer->stop();
                lyricsTimer->stop(); // Berhenti menampilkan lirik
                ui->lyricsDisplay->setPlainText("");
                ui->label_File_Name->setText("Tidak ada lagu diputar");
                ui->horizontalSlider_Audio_File_Duration->setValue(0);
            }
        }
    }

    // Menggunakan MPlayer->playbackState() untuk mengecek apakah sedang diputar, dijeda, atau berhenti
    if (MPlayer->playbackState() == QMediaPlayer::PlayingState) {
        // Start lirik timer ketika lagu mulai diputar
        if (!lyricsTimer->isActive() && !syncedLyricsData.isEmpty()) {
            lyricsTimer->start();
            qDebug() << "Lyrics timer started.";
        }
    } else if (MPlayer->playbackState() == QMediaPlayer::StoppedState ||
               MPlayer->playbackState() == QMediaPlayer::PausedState ||
               status == QMediaPlayer::NoMedia) {
        // Stop lirik timer jika media berhenti, dijeda, atau tidak ada
        lyricsTimer->stop();
        qDebug() << "Lyrics timer stopped.";
    }
}


void MainWindow::on_searchLineEdit_textChanged(const QString &query)
{
    ui->listWidget_SearchResult->clear();
    hasilPencarian.clear(); // Pastikan ini membersihkan full path, bukan hanya nama file

    if (!query.isEmpty()) {
        for (const QString &path : qAsConst(daftarLagu)) { // Cari di daftarLagu utama
            if (QFileInfo(path).fileName().contains(query, Qt::CaseInsensitive)) {
                hasilPencarian.append(path); // Tambahkan full path ke hasilPencarian
                ui->listWidget_SearchResult->addItem(QFileInfo(path).fileName()); // Tampilkan nama file
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

void MainWindow::fetchLyrics(const QString &fullTitle)
{
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
    // Opsional: Panggil urutkanDaftarLagu() di sini jika Anda ingin tampilan direfresh
    // dan urutan tetap konsisten dengan daftarLagu internal setelah manual reorder.
    // Jika Anda hanya ingin menampilkan urutan yang sudah di-drag-drop, tidak perlu panggil.
    // urutkanDaftarLagu();
}

void MainWindow::on_pushButton_KembaliKeLirik_clicked()
{
    ui->stackedView->setCurrentWidget(ui->page_Lyrics); // Kembali ke halaman lirik
}

// Fungsi ini sekarang hanya untuk merefresh tampilan ui->listWidget
// Tidak lagi melakukan pengurutan alfabetis. Urutan dikelola oleh drag and drop UI.
void MainWindow::urutkanDaftarLagu()
{
    ui->listWidget->clear();
    for (const QString &path : qAsConst(daftarLagu)) {
        QListWidgetItem *item = new QListWidgetItem(QFileInfo(path).fileName());
        item->setData(Qt::UserRole, path); // PENTING: Simpan path lengkap di UserRole
        ui->listWidget->addItem(item);
    }
    // Pastikan jika ada lagu yang sedang diputar, itemnya tetap highlight
    if (indeksLaguSaatIni != -1 && indeksLaguSaatIni < ui->listWidget->count()) {
        ui->listWidget->setCurrentRow(indeksLaguSaatIni);
    }
    qDebug() << "urutkanDaftarLagu() called. ListWidget refreshed based on current daftarLagu.";
}

// Fungsi baru: Sinkronkan daftarLagu internal dengan urutan visual di ui->listWidget
void MainWindow::syncDaftarLaguWithListWidget()
{
    qDebug() << "syncDaftarLaguWithListWidget() called due to playlist reorder.";
    QStringList newDaftarLagu;
    QString currentPlayingPath = MPlayer->source().toLocalFile(); // Path lagu yang sedang diputar

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        QString fullPath = item->data(Qt::UserRole).toString(); // Ambil path lengkap dari UserRole
        if (!fullPath.isEmpty()) {
            newDaftarLagu.append(fullPath);
        } else {
            qDebug() << "Error: No full path found in UserRole for item:" << item->text();
        }
    }
    daftarLagu = newDaftarLagu; // Perbarui daftarLagu internal

    // Opsional: Periksa dan perbarui indeksLaguSaatIni jika lagu yang sedang diputar pindah posisi
    if (!currentPlayingPath.isEmpty() && !daftarLagu.isEmpty()) {
        int newIndex = daftarLagu.indexOf(currentPlayingPath);
        if (newIndex != -1 && newIndex != indeksLaguSaatIni) {
            indeksLaguSaatIni = newIndex;
            qDebug() << "Current playing song's index updated to:" << indeksLaguSaatIni << "after reorder.";
        }
    }
    qDebug() << "DaftarLagu internal updated. New size:" << daftarLagu.size();
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
                for(qint64 ts : qAsConst(lineTimestamps)) {
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

    if (syncedLyricsData.isEmpty()) {
        //qDebug() << "updateLyricDisplay: syncedLyricsData is empty, returning. No synced lyrics to display.";
        return;
    }

    int newLyricIndex = -1;
    // Cari indeks baris lirik yang paling sesuai menggunakan binary search
    // std::upper_bound mengembalikan iterator ke elemen pertama yang > value
    auto it = std::upper_bound(syncedLyricsData.begin(), syncedLyricsData.end(), currentPosition,
                               [](qint64 pos, const LyricLine &line) {
                                   return pos < line.timestamp;
                               });

    if (it != syncedLyricsData.begin()) {
        --it; // Mundur satu untuk mendapatkan baris yang sedang aktif atau baris sebelumnya
        newLyricIndex = std::distance(syncedLyricsData.begin(), it);
    } else {
        newLyricIndex = 0; // Jika posisi lagu di awal sekali, anggap baris pertama aktif
    }

    if (newLyricIndex != -1 && newLyricIndex != currentLyricIndex) {
        currentLyricIndex = newLyricIndex;
        //qDebug() << "updateLyricDisplay: Lyric index changed to:" << currentLyricIndex;

        QStringList displayLines;
        int linesToShowBefore = 2;
        int linesToShowAfter = 2;

        int startIndex = qMax(0, currentLyricIndex - linesToShowBefore);
        int endIndex = qMin(syncedLyricsData.size() - 1, currentLyricIndex + linesToShowAfter);

        //qDebug() << "updateLyricDisplay: Display range from line" << startIndex << "to" << endIndex;

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
        //qDebug() << "updateLyricDisplay: UI updated with new HTML content (first 100 chars):" << fullHtmlContent.left(100) << "...";

        // Logic untuk menggulir ke baris yang aktif
        QTextCursor cursor = ui->lyricsDisplay->textCursor();
        cursor.clearSelection();
        cursor.movePosition(QTextCursor::Start); // Pindahkan kursor ke awal dokumen
        ui->lyricsDisplay->setTextCursor(cursor);

        QTextDocument *doc = ui->lyricsDisplay->document();
        QTextBlock block = doc->findBlockByLineNumber(currentLyricIndex - startIndex);

        if (block.isValid()) {
            QTextCursor scrollCursor(block);
            ui->lyricsDisplay->setTextCursor(scrollCursor);
            ui->lyricsDisplay->ensureCursorVisible(); // Gulir agar kursor terlihat
            //qDebug() << "updateLyricDisplay: Scrolled to active lyric block.";
        } else {
            //qDebug() << "updateLyricDisplay: Block for scrolling is not valid.";
        }

        //qDebug() << "Displaying synced lyric (full lines):" << displayLines.join(" | ");
    } else {
        //qDebug() << "updateLyricDisplay: Lyric index did not change or no valid new index. No UI update needed.";
    }
}
