#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_imgwin.h"
#include "ui_aboutdialog.h"
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
    QScrollArea* area = new QScrollArea;
    Ui::ImageWindow* win = new Ui::ImageWindow;
    win->setupUi(area);
    win->pictureLabel->setPixmap(img);
    area->setWindowTitle(file);

    ui->mdiArea->addSubWindow(area)->showMaximized();
}

void MainWindow::doAbout()
{
    QDialog* d = new QDialog;
    Ui::AboutDialog* about = new Ui::AboutDialog;
    about->setupUi(d);
    d->show();
}
