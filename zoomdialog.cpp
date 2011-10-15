/******************************************************************************
 * zoomdialog.cpp
 * Create a dialog to adjust the zoom.
 * This was implemented before we noticed that the zoom had to be set by a
 * drop-down box, so it's really more redundant than anything, but still neat.
 * There is also a slider in the main window's toolbar now that further makes
 * this useless, but it was a prototype for our color adjust window.
 * Author: Sam Harrington
 * Date of first commit: 10 October, 2011
 * Known bugs: If the target window is not set before the dialog is created,
 * it becomes impossible to set the target in Linux because the QMdiArea will
 * not have a valid sub-window. This does not affect Windows users.
 *****************************************************************************/
#include "zoomdialog.h"
#include "ui_zoomdialog.h"
#include "imgwin.h"
// Default constructor with a minor change to initialize the target.
ZoomDialog::ZoomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ZoomDialog)
{
    ui->setupUi(this);
    target = NULL;
}
// Default deconstructor.
ZoomDialog::~ZoomDialog()
{
    delete ui;
}
// Sam, it looks like you made some modifications to this since I last looked.
// Would you care to write the comment blocks for this file?
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
    ui->xBox->setText(QString().setNum(target->getImage().width() * s / 100));
    ui->yBox->setText(QString().setNum(target->getImage().height() * s / 100));
}

