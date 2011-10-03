#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_aboutdialog.h"
#include "imgwin.h"
#include <QMdiArea>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QMdiSubWindow>

/******************************************************************************
 * MainWindow(QWidget*): Creates the MainWindow class
 * Creates the main window, passing the parent to QMainWindow, and initializing
 * the ui.
 *****************************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

/******************************************************************************
 * ~MainWindow(): Destructor
 * Deletes the ui.
 *****************************************************************************/
MainWindow::~MainWindow()
{
    delete ui;
}

/******************************************************************************
 * QPixmap* getPixmap(): Get the current image window's image
 * Private function.
 * Finds the current image window and returns the image.
 *****************************************************************************/
const QPixmap* MainWindow::getPixmap()
{
    QMdiSubWindow *child = ui->mdiArea->activeSubWindow();
    ImgWin *win = (ImgWin*)(child->widget());
    return win->getPixmap();
}

/******************************************************************************
 * setPixmap(QPixmap): Set the current image window's image
 * Private function.
 * Finds the current image window and sets the image.
 *****************************************************************************/
void MainWindow::setPixmap(QPixmap p)
{
    QMdiSubWindow *child = ui->mdiArea->activeSubWindow();
    ImgWin *win = (ImgWin*)(child->widget());
    win->setPixmap(p);
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

    if (file != "")
    {
        QPixmap img(file);
        if (!img.isNull())
        {
            ImgWin* win = new ImgWin;
            win->setPixmap(img);
            win->setWindowTitle(file);

            QMdiSubWindow* subwin = ui->mdiArea->addSubWindow(win);
            subwin->showMaximized();

            win->setMenuItem(ui->menuWindow->addAction(file,subwin,SLOT(setFocus())));
            connect(win,SIGNAL(closing(QAction*)),this,SLOT(removeWindowListItem(QAction*)));
        }
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
    QImage img = getPixmap()->toImage();
    for(int i = 0; i < img.width(); ++i)
    {
        for(int j = 0; j < img.height(); ++j)
        {
            QRgb old_color = img.pixel(i, j);
            old_color = qRgb(0xFF - qRed(old_color), 0xFF - qGreen(old_color), 0xFF - qBlue(old_color));
            img.setPixel(i, j, old_color);
        }
    }
    setPixmap(QPixmap::fromImage(img));
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
{
    QImage img = getPixmap()->toImage();
    QImage out_img = img;
    /* We only want the inner pixels. Don't worry about the furthest edge.*/
    for(int i = 1; i < img.width() - 1; ++i)
    {
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
    }
    setPixmap(QPixmap::fromImage(out_img));
}

void MainWindow::doCrop()
{
    QImage img = getPixmap()->toImage();
    setPixmap(QPixmap::fromImage(img));
}

/******************************************************************************
 * doAbout(): Show about dialog.
 * Slot function.
 * Signalers: actionAbout()
 * Shows the About dialog from aboutdialog.ui.
 *****************************************************************************/
void MainWindow::doAbout()
{
    QDialog* d = new QDialog;
    Ui::AboutDialog* about = new Ui::AboutDialog;
    about->setupUi(d);
    d->show();
}

/******************************************************************************
 * doChangeImage(QMdiSubWindow*): Handles window switching.
 * Slot function.
 * Signalers: QMdiArea->SubWindowActivated(QMdiSubWindow*)
 * Changes the enabled state of the edit menu when the last window is closed.
 *****************************************************************************/
void MainWindow::doChangeImage(QMdiSubWindow* win)
{
    ui->menuEdit->setDisabled(win == 0);
}

/******************************************************************************
 * removeWindowListItem(QAction*): Remove the QAction from the window list.
 * Slot function.
 * Signalers: ImgWin->closing(QAction*)
 * Remove the QAction from the window menu when the window closes.
 *****************************************************************************/
void MainWindow::removeWindowListItem(QAction* act)
{
    ui->menuWindow->removeAction(act);
}
