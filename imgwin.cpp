#include "ui_imgwin.h"
#include "imgwin.h"
#include <QCloseEvent>

ImgWin::ImgWin(QWidget *parent) :
    QScrollArea(parent), ui(new Ui::ImageWindow)
{
    ui->setupUi(this);
}

ImgWin::~ImgWin()
{
    delete ui;
}

const QPixmap* ImgWin::getPixmap()
{
    return ui->pictureLabel->pixmap();
}

void ImgWin::setPixmap(QPixmap image)
{
    ui->pictureLabel->setPixmap(image);
}

void ImgWin::closeEvent(QCloseEvent *event)
{
    emit closing(menu_item);
    event->accept();
}
