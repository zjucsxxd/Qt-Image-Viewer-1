/******************************************************************************
 * zoomdialog.cpp
 * Create a dialog to adjust the zoom.
 * This allows you to access zoom from the menus, in addition to the box &
 * slider on the toolbar.
 * Author: Sam Harrington
 * Date of first commit: 10 October, 2011
 * Known bugs: none.
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

/******************************************************************************
 * setTarget(): Set the ImgWin that this dialog will resize. Reads the current
 * zoom, updates the sliders, and stores the window.
 *****************************************************************************/
void ZoomDialog::setTarget(ImgWin *newTarget)
{
    target = newTarget;
    ui->zoomSlider->setValue(target->getScale());
    updateBoxes(target->getScale());
}

/******************************************************************************
 * commit(): Apply the zoom.
 * This takes the current value of the zoom slider and applies it to the
 * target.
 *****************************************************************************/
void ZoomDialog::commit()
{
    if(target != NULL)
    {
        target->setScale(ui->zoomSlider->value());
    }
}

/******************************************************************************
 * updateBoxes(int): Show what size the image would appear if commited.
 * Shows the size of the resulting view in the label.
 *****************************************************************************/
void ZoomDialog::updateBoxes(int s)
{
    ui->xBox->setText(QString().setNum(target->getImage().width() * s / 100));
    ui->yBox->setText(QString().setNum(target->getImage().height() * s / 100));
}

