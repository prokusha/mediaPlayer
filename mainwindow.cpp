#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "helper.h"
#include "dialogurl.h"

#include <QDebug>
#include <QLabel>
#include <QFileDialog>
#include <QProcess>
#include <QFileInfo>
#include <QMessageBox>
#include <QMediaMetaData>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // qDebug() << ui->volumeSlider->value();
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(ui->volumeSlider->value() / qreal(100.0));
    ui->labelVolme->setNum(ui->volumeSlider->value());

    connectElements();

    // testing::testPlaySoundURL(player);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectElements() {
    // play slider
    connect(player, &QMediaPlayer::positionChanged, ui->horizontalSlider, &QSlider::setValue);
    connect(player, SIGNAL(metaDataChanged()), this, SLOT(setMetaData()));
    // sliders
    connect(ui->horizontalSlider, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);
    connect(ui->horizontalSlider, &QSlider::sliderPressed, player, &QMediaPlayer::pause);
    connect(ui->horizontalSlider, &QSlider::sliderReleased, player, &QMediaPlayer::play);
    // connect(ui->volumeSlider, &QSlider::valueChanged, audioOutput, &QAudioOutput::setVolume);
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), ui->labelVolme, SLOT(setNum(int)));
    // buttons
    connect(ui->playBtn, SIGNAL(clicked()), player, SLOT(play()));
    connect(ui->stopBtn, SIGNAL(clicked()), player, SLOT(stop()));
    connect(ui->pauseBtn, SIGNAL(clicked()), player, SLOT(pause()));
}

void MainWindow::setPlay(QUrl& url) {
    player->setSource(url);
    player->play();
}

void MainWindow::setMetaData() {
    QMediaMetaData value = player->metaData();

    int duraction = value[QMediaMetaData::Duration].toInt();

    helper::Time time(duraction);
    QString timeText = "/ ";
    timeText += time.min < 10 ? '0' + QString::number(time.min) + ':' : QString::number(time.min) + ':';
    timeText += time.sec < 10 ? '0' + QString::number(time.sec) : QString::number(time.sec);

    qDebug() << duraction;

    ui->mediaTime_2->setText(timeText);
    ui->horizontalSlider->setMaximum(duraction);

    QList keys = value.keys();

    for (auto key : keys) {
        qDebug() << key << value[key];
    }
    QList author = value[QMediaMetaData::ContributingArtist].toList();
    QString title = value[QMediaMetaData::Title].toString();
    // qDebug() << author[0].toString() << title;

    QString info = "";
    if (!author.empty()) {
        bool first = true;
        for (int i = 0; i < author.size(); ++i) {
            if (first) {
                first = false;
            } else {
                info += ", ";
            }
            info += author[i].toString();
        }
        info += " - ";
        info += title;
    } else {
        info += player->source().fileName();
    }

    ui->labelMedia->setText(info);
    ui->labelMedia->adjustSize();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    helper::Time time(value);
    QString timeText;
    timeText += time.min < 10 ? '0' + QString::number(time.min) + ':' : QString::number(time.min) + ':';
    timeText += time.sec < 10 ? '0' + QString::number(time.sec) : QString::number(time.sec);
    ui->mediaTime->clear();
    ui->mediaTime->setText(timeText);
}


void MainWindow::on_openFileBtn_clicked()
{
    auto file = QUrl::fromLocalFile(QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Audio (*.mp3 *.flac)")));
    if (file.isEmpty()) {
        return;
    }
    setPlay(file);
}


void MainWindow::on_volumeSlider_valueChanged(int value)
{

    audioOutput->setVolume(value / qreal(100.0));
    // qDebug() << audioOutput->volume();
}


void MainWindow::on_openURLBtn_clicked()
{
    DialogUrl * w = new DialogUrl();
    if (w->exec() == QDialog::Accepted) {
        QString urlPath = w->getUrl();

        if (QUrl url = QUrl::fromUserInput(urlPath); !url.isLocalFile()) {
            qDebug() << url.toString();
            if (w->ytdlCheck()) {
                if (!QFileInfo::exists("./yt-dlp")) {
                    QMessageBox::information(this, "Warning", "yt-dlp not found");
                    return on_openURLBtn_clicked();
                }
                QString urlStr = url.toString();
                QProcess ytdl;
                QStringList args {"--get-url", "-f 140", urlStr};
                ytdl.start("./yt-dlp", args);
                ytdl.waitForFinished();
                urlStr = ytdl.readAllStandardOutput();
                qDebug() << urlStr;
                url = QUrl(urlStr);
            }
            setPlay(url);
        }
    }
}

