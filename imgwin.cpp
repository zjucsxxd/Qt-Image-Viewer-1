#include "ui_imgwin.h"
#include "imgwin.h"

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
