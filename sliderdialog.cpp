#include "sliderdialog.h"
#include "ui_sliderdialog.h"

SliderDialog::SliderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SliderDialog)
{
    ui->setupUi(this);
}

SliderDialog::~SliderDialog()
{
    delete ui;
}

void SliderDialog::setPixmap(QPixmap *pic)
{
    ui->PreviewLabel->setPixmap(pic->copy());
}

const QPixmap * SliderDialog::getPixmap()
{
    return ui->PreviewLabel->pixmap();
}
void SliderDialog::setBrightness()
{
}
void SliderDialog::setContrast()
{
}
void SliderDialog::setRed()
{
}
void SliderDialog::setBlue()
{
}
void SliderDialog::setGreen()
{
}
