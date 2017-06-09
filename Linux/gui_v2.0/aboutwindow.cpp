#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QDesktopServices>
#include <QUrl>

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::on_label_9_linkActivated()
{
    QDesktopServices::openUrl(QUrl("https://zzitcyzdb.wixsite.com/chapp"));
}
