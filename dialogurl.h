#ifndef DIALOGURL_H
#define DIALOGURL_H

#include <QDialog>

namespace Ui {
class DialogUrl;
}

class DialogUrl : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUrl(QWidget *parent = nullptr);
    ~DialogUrl();

    QString getUrl();
    bool ytdlCheck();

private:
    Ui::DialogUrl *ui;
};

#endif // DIALOGURL_H
