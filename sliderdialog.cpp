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
