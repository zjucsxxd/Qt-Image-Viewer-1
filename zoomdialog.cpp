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
}

void ZoomDialog::commit()
{
    if(target != NULL)
    {
        target->scale(ui->zoomSlider->value());
    }
    close();
}
