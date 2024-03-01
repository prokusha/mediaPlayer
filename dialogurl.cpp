#include "dialogurl.h"
#include "ui_dialogurl.h"

#include <QFileInfo>
#include <QMessageBox>

DialogUrl::DialogUrl(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogUrl)
{
    ui->setupUi(this);
}

DialogUrl::~DialogUrl()
{
    delete ui;
}

QString DialogUrl::getUrl() {
    return ui->lineEdit->text();
}

bool DialogUrl::ytdlCheck() {
    return ui->checkBox->isChecked();
}

