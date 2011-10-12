#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_imgresizedialog.h"
#include "zoomdialog.h"
#include "imgwin.h"
#include "imgabout.h"
#include "sliderdialog.h"
#include <QMdiArea>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QMdiSubWindow>
#include <QMessageBox>

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
 * QPixmap* getCurrent(): Get the current image window
 * Private function.
 * Finds the current image window and returns it.
 *****************************************************************************/
ImgWin* MainWindow::getCurrent()
{
    QMdiSubWindow *child = ui->mdiArea->activeSubWindow();
    return (ImgWin*)(child->widget());
}

/******************************************************************************
 * QPixmap* getPixmap(): Get the current image window's image
 * Private function.
 * Finds the current image window and returns the image.
 *****************************************************************************/
const QPixmap* MainWindow::getPixmap()
{
    return getCurrent()->getPixmap();
}

/******************************************************************************
 * setPixmap(QPixmap): Set the current image window's image
 * Private function.
 * Finds the current image window and sets the image.
 *****************************************************************************/
void MainWindow::setPixmap(QPixmap p)
{
    getCurrent()->setPixmap(p);
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
    QString file = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp *.gif);;Any Files (*.*)");

    if (file != "")
    {
        QPixmap img(file);
        if (!img.isNull())
        {
            ImgWin* win = new ImgWin;
            win->setPixmap(img);
            win->setWindowTitle(file);
            win->setReader(file);

            QMdiSubWindow* subwin = ui->mdiArea->addSubWindow(win);
            subwin->showMaximized();

            // Add a item to the window menu
            win->setMenuItem(ui->menuWindow->addAction(file, subwin, SLOT(setFocus())));
            connect(win, SIGNAL(closing(QAction*)), this, SLOT(removeWindowListItem(QAction*)));

            // Mouse info
            connect(win, SIGNAL(mouseOverInfo(QPoint)), SLOT(imgMouseInfo(QPoint)), Qt::UniqueConnection);
        }
    }
}

/******************************************************************************
 * doSave(void): Save a document.
 * Slot function.
 * Signalers: actionSave
 * Save the current image to the name provided.
 *****************************************************************************/
void MainWindow::doSave()
{
    QString file = QFileDialog::getSaveFileName(this, "Save Image", "", "Image Files (*.png *.jpg *.bmp) ");

    if (file != "")
    {
        getPixmap()->save(file);
    }
}

/******************************************************************************
 * doRevert(void): Change back to original file contents.
 * Slot function.
 * Signalers: actionRevert
 * Revert the current image to the original.
 *****************************************************************************/
void MainWindow::doRevert()
{
    if (QMessageBox::Ok == QMessageBox::question(this, "Revert file", "Do you want to revert? You will lose all your changes.", QMessageBox::Ok, QMessageBox::Cancel))
    {
        ImgWin *win = getCurrent();
        win->setPixmap(QPixmap::fromImageReader(win->getReader()));
        win->setReader(win->getReader()->fileName());
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
void MainWindow::doSmooth()
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
            red = (red * 4 + qRed(img.pixel(i-1,j)) * 2 + qRed(img.pixel(i+1,j)) * 2
                  + qRed(img.pixel(i,j-1)) * 2 + qRed(img.pixel(i,j+1)) * 2
                  + qRed(img.pixel(i-1,j-1)) + qRed(img.pixel(i+1,j+1))
                  + qRed(img.pixel(i-1,j+1)) + qRed(img.pixel(i+1, j-1)) ) / 16;
            blue = (blue * 4 + qBlue(img.pixel(i-1,j)) * 2 + qBlue(img.pixel(i+1,j)) * 2
                  + qBlue(img.pixel(i,j-1)) * 2 + qBlue(img.pixel(i,j+1)) * 2
                  + qBlue(img.pixel(i-1,j-1)) + qBlue(img.pixel(i+1,j+1))
                  + qBlue(img.pixel(i-1,j+1)) + qBlue(img.pixel(i+1, j-1)) ) / 16;
            green = (green * 4 + qGreen(img.pixel(i-1,j)) * 2 + qGreen(img.pixel(i+1,j)) * 2
                  + qGreen(img.pixel(i,j-1)) * 2 + qGreen(img.pixel(i,j+1)) * 2
                  + qGreen(img.pixel(i-1,j-1)) + qGreen(img.pixel(i+1,j+1))
                  + qGreen(img.pixel(i-1,j+1)) + qGreen(img.pixel(i+1, j-1)) ) / 16;
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

/******************************************************************************
 * doZoom(): Change the view zoom.
 * Slot function.
 * Signalers: actionZoom()
 * Zoom the image to the factor the user specifies.
 *****************************************************************************/
void MainWindow::doZoom()
{
    ZoomDialog *zoom = new ZoomDialog(this);
    zoom->setTarget(getCurrent());
    zoom->show();
}

/******************************************************************************
 * doCrop(): Crop the image.
 * Slot function.
 * Signalers: actionCrop()
 * Crop the image to the selection.
 *****************************************************************************/
void MainWindow::doCrop()
{
    ImgWin* win = getCurrent();
    QImage img = getPixmap()->toImage();

    QRect sel = win->getSelection();
    if (sel.isEmpty())
    {
        QMessageBox::warning(this, "Crop", "Press select a region to crop.", QMessageBox::Ok);
        return;
    }

    img = img.copy(win->getSelection());
    setPixmap(QPixmap::fromImage(img));
}

/******************************************************************************
 * doResize(): Resize the image.
 * Slot function.
 * Signalers: actionResize()
 * Launch a dialog box and resize the image to the specified size.
 *****************************************************************************/
void MainWindow::doResize()
{
    QDialog* d = new QDialog(this);
    Ui::ImgResizeDialog* resize = new Ui::ImgResizeDialog;
    resize->setupUi(d);
    resize->widthSpin->setValue(getPixmap()->size().width());
    resize->heightSpin->setValue(getPixmap()->size().height());
    resize->widthSpin->setRange(1,1000000);
    resize->heightSpin->setRange(1,1000000);

    if (d->exec() == QDialog::Accepted)
    {
        setPixmap(getPixmap()->scaled(QSize(resize->widthSpin->value(),resize->heightSpin->value())));
    }
}

/******************************************************************************
 * doAbout(): Show about dialog.
 * Slot function.
 * Signalers: actionAbout()
 * Shows the About dialog from aboutdialog.ui.
 *****************************************************************************/
void MainWindow::doAbout()
{
    QMessageBox::information(this, "About",
        "A very simple image viewer, created for GUI/OOP.\n"
        "Copyright 2011 Samuel Harrington & Chris Jensen.");
}

/*******************************************************************************
 * doInfo(): Show a dialog containing basic image info.
 * Slot function.
 * Signalers: actionInfo()
 * Shows the Image Info dialog from imageInfo.ui and sets the fields.
 ******************************************************************************/
void MainWindow::doInfo()
{
    imgAbout *d = new imgAbout();
    QMdiSubWindow *child = ui->mdiArea->activeSubWindow();
    ImgWin *win = (ImgWin*)(child->widget());
    d->setup(win);
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
    ui->statusBar->clearMessage();

    ui->menuEdit->setDisabled(win == 0);
    ui->actionSave->setDisabled(win == 0);
    ui->actionRevert->setDisabled(win == 0);
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

void MainWindow::doSliders()
{
    SliderDialog *d = new SliderDialog(ui->mdiArea->activeSubWindow());
    QMdiSubWindow *child = ui->mdiArea->activeSubWindow();
    ImgWin *win = (ImgWin*)(child->widget());
    d->setTarget(win);
    d->setPixmap(win->getPixmap());
    d->exec();
}

/******************************************************************************
 * imgMouseInfo(QPoint): Display mouse info.
 * Slot function.
 * Signalers: ImgWin->mouseOverInfo(QPoint)
 * Display that the mouse is at the given point on the image (not screen).
 *****************************************************************************/
void MainWindow::imgMouseInfo(QPoint p)
{
    QString msg = QString("%1, %2").arg(p.x()).arg(p.y());
    ui->statusBar->showMessage(msg);
}
