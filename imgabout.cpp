#include "imgabout.h"
#include "ui_imgabout.h"

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
