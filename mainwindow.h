#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>

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

private:
    Ui::MainWindow *ui;
    QMediaPlayer * player = new QMediaPlayer;
    QAudioOutput * audioOutput = new QAudioOutput;


    void connectElements();
};
#endif // MAINWINDOW_H
