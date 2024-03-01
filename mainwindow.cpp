#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "testing.h"
#include "dialogurl.h"

#include <QDebug>
#include <QLabel>
#include <QFileDialog>
#include <QProcess>
#include <QFileInfo>
#include <QMessageBox>

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
    connect(player, &QMediaPlayer::durationChanged, ui->horizontalSlider, &QSlider::setMaximum);
    connect(ui->horizontalSlider, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);
    // sliders
    // connect(ui->volumeSlider, &QSlider::valueChanged, audioOutput, &QAudioOutput::setVolume);
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), ui->labelVolme, SLOT(setNum(int)));
    // buttons
    connect(ui->playBtn, SIGNAL(clicked()), player, SLOT(play()));
    connect(ui->stopBtn, SIGNAL(clicked()), player, SLOT(stop()));
    connect(ui->pauseBtn, SIGNAL(clicked()), player, SLOT(pause()));
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    int sec = value / 1000;
    int min = sec / 60;
    if (sec > 60) {
        sec -= min * 60;
    }
    ui->lcdNumber->display(min);
    ui->lcdNumber_2->display(sec);
}


void MainWindow::on_openFileBtn_clicked()
{
    auto file = QUrl::fromLocalFile(QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Audio (*.mp3 *.flac)")));
    if (file.isEmpty()) {
        return;
    }
    player->setSource(file);
    player->play();
}


void MainWindow::on_volumeSlider_valueChanged(int value)
{

    audioOutput->setVolume(value / qreal(100.0));
    qDebug() << audioOutput->volume();
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
            player->setSource(url);
            player->play();
        }
    }
}

