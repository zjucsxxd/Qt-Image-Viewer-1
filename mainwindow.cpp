#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMdiArea>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QMdiSubWindow>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doOpen()
{
    QString file = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp)");

    QPixmap img(file);
    //QScrollArea* area = new QScrollArea();
    QLabel* label = new QLabel();//area);
    //label->setSizePolicy(QSizePolicy(Policy.Fixed,Policy.Fixed));
    //area->setWidget(label);
    label->setPixmap(img);

    ui->mdiArea->addSubWindow(label)->show();
}
