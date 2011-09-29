#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMdiArea>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QMdiSubWindow>
#include <QScrollArea>

// Sam: Please put a header on the constructor and destructor. --Chris
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

/******************************************************************************
 * doOpen(void): Open a document.
 * Slot function.
 * Signalers: actionOpen
 * Attempts to open a document using Qt, and creates a child window for the
 * image. If the user cancells, no window is created.
 *****************************************************************************/
void MainWindow::doOpen()
{
    QString file = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp)");

    if(file != "")
    {
        QPixmap img(file);
        //QScrollArea* area = new QScrollArea();
        QLabel* label = new QLabel();//area);
        //label->setSizePolicy(QSizePolicy(Policy.Fixed,Policy.Fixed));
        //area->setWidget(label);
        label->setPixmap(img);
        ui->mdiArea->addSubWindow(label)->show();
    }
}

/******************************************************************************
 * doNegate(void): Negate an image.
 * Slot function.
 * Signalers: actionNegate()
 * Negates an image by subtracting each pixel's red, green and blue values from
 * the maximum value for a given color (0xFF).
 *****************************************************************************/
void MainWindow::doNegate()
{
    QMdiSubWindow *child = ui->mdiArea->activeSubWindow();
    QLabel *label = (QLabel*)(child->widget());
    const QPixmap *pic = label->pixmap();
    QImage img = pic->toImage();
    for(int i = 0; i < img.width(); ++i)
        for(int j = 0; j < img.height(); ++j)
        {
            QRgb old_color = img.pixel(i, j);
            old_color = qRgb(0xFF - qRed(old_color), 0xFF - qGreen(old_color), 0xFF - qBlue(old_color));
            img.setPixel(i, j, old_color);
         }
    label->setPixmap( pic->fromImage(img));
    child->show();
    return;
}

/******************************************************************************
 * doSharpen(void): Apply a sharpen filter to an image.
 * Slot function.
 * Signalers: actionNegate()
 * Attempts to sharpen an image by applying a
 * [0  -1  0]
 * [-1  5 -1]
 * [0  -1  0]
 * filter to each pixel in succession.
 * Each color has the filter applied seperately, and if a color would exceed
 * 0xFF, it is clipped to 0xFF. If a color would go below 0, it is clipped to
 * 0.
 *****************************************************************************/
void MainWindow::doSharpen()
{    QMdiSubWindow *child = ui->mdiArea->activeSubWindow();
    QLabel *label = (QLabel*)(child->widget());
    const QPixmap *pic = label->pixmap();
    QImage img = pic->toImage();
    QImage out_img = img;
    /* We only want the inner pixels. Don't worry about the furthest edge.*/
    for(int i = 1; i < img.width() - 1; ++i)
        for(int j = 1; j < img.height() - 1; ++j)
        {
            QRgb old_color = img.pixel(i, j);
            int red = qRed(old_color);
            int blue = qBlue(old_color);
            int green = qGreen(old_color);
            red = 5 * red - qRed(img.pixel(i-1,j)) -qRed(img.pixel(i+1,j)) -qRed(img.pixel(i,j-1)) -qRed(img.pixel(i,j+1));
            blue = 5 * blue - qBlue(img.pixel(i-1,j)) -qBlue(img.pixel(i+1,j)) -qBlue(img.pixel(i,j-1)) -qBlue(img.pixel(i,j+1));
            green = 5 * green - qGreen(img.pixel(i-1,j)) -qGreen(img.pixel(i+1,j)) -qGreen(img.pixel(i,j-1)) -qGreen(img.pixel(i,j+1));
            if(red > 0xFF) red = 0xEF;
            if(red < 0) red = 0;
            if(green > 0xFF) green = 0xEF;
            if(green < 0) green = 0;
            if(blue > 0xFF) blue = 0xEF;
            if(blue < 0 ) blue = 0;
            old_color = qRgb(red, green, blue);
            out_img.setPixel(i, j, old_color);
         }
    label->setPixmap( pic->fromImage(out_img));
    child->show();
    return;
}

void MainWindow::doCrop()
{/*
    QLabel *label = (QLabel*)(child->widget());
    const QPixmap *pic = label->pixmap();
    QImage img = pic->toImage();
    */
}
