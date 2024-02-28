#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "testing.h"

#include <QDebug>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // qDebug() << ui->volumeSlider->value();
    player->setVolume(ui->volumeSlider->value());
    ui->labelVolme->setNum(ui->volumeSlider->value());

    connectElements();

    testing::testPlaySoundURL(player);
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
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), ui->labelVolme, SLOT(setNum(int)));
    // buttons
    connect(ui->playBtn, SIGNAL(clicked()), player, SLOT(play()));
    connect(ui->stopBtn, SIGNAL(clicked()), player, SLOT(stop()));
    connect(ui->pauseBtn, SIGNAL(clicked()), player, SLOT(pause()));
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    int sec = ui->horizontalSlider->value() / 1000;
    int min = sec / 60;
    if (sec > 60) {
        sec -= min * 60;
    }
    ui->lcdNumber->display(min);
    ui->lcdNumber_2->display(sec);
}

