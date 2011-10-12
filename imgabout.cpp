#include "imgabout.h"
#include "ui_imgabout.h"
#include <QImageReader>

imgAbout::imgAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::imgAbout)
{
    ui->setupUi(this);
}

imgAbout::~imgAbout()
{
    delete ui;
}

void imgAbout::setup(ImgWin *target)
{
    QImageReader* reader = target->getReader();
    ui->fname->setText(reader->fileName());
    ui->ftype->setText(reader->format());

    QImage pix = target->getImage();
    int width = pix.size().width();
    int height = pix.size().height();

    QString dim = "";
    dim += QString::number(width);
    dim += " x ";
    dim += QString::number(height);
    ui->fsize->setText(dim);
}
