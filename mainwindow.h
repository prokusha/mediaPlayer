#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaMetaData>

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

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_openFileBtn_clicked();

    void on_volumeSlider_valueChanged(int value);

    void on_openURLBtn_clicked();

    void setMetaData();

private:
    Ui::MainWindow *ui;
    QMediaPlayer * player = new QMediaPlayer;
    QAudioOutput * audioOutput = new QAudioOutput;

    void connectElements();
    void setPlay(QUrl& url);

};
#endif // MAINWINDOW_H
