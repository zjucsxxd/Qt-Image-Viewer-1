#include "zoomdialog.h"
#include "ui_zoomdialog.h"
#include "imgwin.h"

ZoomDialog::ZoomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ZoomDialog)
{
    ui->setupUi(this);
    target = NULL;
}

ZoomDialog::~ZoomDialog()
{
    delete ui;
}

void ZoomDialog::setTarget(ImgWin *newTarget)
{
    target = newTarget;
    ui->zoomSlider->setValue(target->getScale());
    updateBoxes(target->getScale());
}

void ZoomDialog::commit()
{
    if(target != NULL)
    {
        target->setScale(ui->zoomSlider->value());
    }
}

void ZoomDialog::updateBoxes(int s)
{
    ui->xBox->setValue(target->getPixmap()->width() * s / 100);
    ui->yBox->setValue(target->getPixmap()->height() * s / 100);
}

