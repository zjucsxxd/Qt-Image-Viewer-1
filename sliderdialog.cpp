#include "sliderdialog.h"
#include "ui_sliderdialog.h"
#include <math.h>

SliderDialog::SliderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SliderDialog)
{
    ui->setupUi(this);
    bright_index = 0;
}

SliderDialog::~SliderDialog()
{
    delete ui;
}

void SliderDialog::setPixmap(const QPixmap *pic)
{
    ui->PreviewLabel->setPixmap(*pic);
}

const QPixmap * SliderDialog::getPixmap()
{
    return ui->PreviewLabel->pixmap();
}

void SliderDialog::setBrightness()
{
    QImage pic = ui->PreviewLabel->pixmap()->toImage();
    for(int i = 1; i < pic.width(); ++i)
        for(int j = 1; j < pic.height(); ++j)
        {
            QRgb color = pic.pixel(i,j);
            int red = qRed(color) + ui->brightness->value() - bright_index;
            int blue = qBlue(color) + ui->brightness->value() - bright_index;
            int green = qGreen(color) + ui->brightness->value() - bright_index;
            if(red > 0xFF) red = 0xFF;
            if(blue > 0xFF) blue = 0xFF;
            if(green > 0xFF) green = 0xFF;
            if(red < 0) red = 0;
            if(blue < 0) blue = 0;
            if(green < 0) green = 0;
            color = qRgb(red, green, blue);
            pic.setPixel(i,j,color);
        }
    ui->PreviewLabel->setPixmap(QPixmap::fromImage(pic));
    ui->PreviewLabel->show();
    bright_index = ui->brightness->value();
}
void SliderDialog::setContrast()
{    QImage pic = ui->PreviewLabel->pixmap()->toImage();
    for(int i = 1; i < pic.width(); ++i)
        for(int j = 1; j < pic.height(); ++j)
        {
            QRgb color = pic.pixel(i,j);
            int red = pow(qRed(color),ui->Contrast->value() / 100.0);
            int blue = pow(qBlue(color),ui->Contrast->value() / 100.0);
            int green = pow(qGreen(color),ui->Contrast->value() / 100.0);
            if(red > 0xFF) red = 0xFF;
            if(blue > 0xFF) blue = 0xFF;
            if(green > 0xFF) green = 0xFF;
            if(red < 0) red = 0;
            if(blue < 0) blue = 0;
            if(green < 0) green = 0;
            color = qRgb(red, green, blue);
            pic.setPixel(i,j,color);
        }
    ui->PreviewLabel->setPixmap(QPixmap::fromImage(pic));
    ui->PreviewLabel->show();
    bright_index = ui->brightness->value();
}
void SliderDialog::setRed()
{
    QImage pic = ui->PreviewLabel->pixmap()->toImage();
    for(int i = 1; i < pic.width(); ++i)
        for(int j = 1; j < pic.height(); ++j)
        {
            QRgb color = pic.pixel(i,j);
            int red = qRed(color) + ui->Red->value();
            int blue = qBlue(color);
            int green = qGreen(color);
            if(red > 0xFF) red = 0xFF;
            if(red < 0) red = 0;
            color = qRgb(red, green, blue);
            pic.setPixel(i,j,color);
        }
    ui->PreviewLabel->setPixmap(QPixmap::fromImage(pic));
    ui->PreviewLabel->show();
    bright_index = ui->brightness->value();
}
void SliderDialog::setBlue()
{
    QImage pic = ui->PreviewLabel->pixmap()->toImage();
    for(int i = 1; i < pic.width(); ++i)
        for(int j = 1; j < pic.height(); ++j)
        {
            QRgb color = pic.pixel(i,j);
            int red = qRed(color);
            int blue = qBlue(color) + ui->Blue->value();
            int green = qGreen(color);
            if(blue > 0xFF) blue = 0xFF;
            if(blue < 0) blue = 0;
            color = qRgb(red, green, blue);
            pic.setPixel(i,j,color);
        }
    ui->PreviewLabel->setPixmap(QPixmap::fromImage(pic));
    ui->PreviewLabel->show();
    bright_index = ui->brightness->value();
}
void SliderDialog::setGreen()
{
    QImage pic = ui->PreviewLabel->pixmap()->toImage();
    for(int i = 1; i < pic.width(); ++i)
        for(int j = 1; j < pic.height(); ++j)
        {
            QRgb color = pic.pixel(i,j);
            int red = qRed(color);
            int blue = qBlue(color);
            int green = qGreen(color) + ui->Green->value();
            if(green > 0xFF) green = 0xFF;
            if(green < 0) green = 0;
            color = qRgb(red, green, blue);
            pic.setPixel(i,j,color);
        }
    ui->PreviewLabel->setPixmap(QPixmap::fromImage(pic));
    ui->PreviewLabel->show();
    bright_index = ui->brightness->value();
}

void SliderDialog::commit()
{
    const QPixmap *pic = ui->PreviewLabel->pixmap();
    target->setPixmap(*pic);
    accept();
}
