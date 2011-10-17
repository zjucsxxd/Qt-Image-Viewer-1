/******************************************************************************
 * mainwindow.cpp
 * Creates the outer wrapper UI and performs some basic image manipulations.
 * This is where the bulk of operations go; Unless something needs a special
 * dialog, it's probably stuffed here.
 * Author: Mostly Sam Harrington, but some of the functions were Christopher
 *  Jensen's fault.
 * Date of first commit: 28 September, 2011
 * Knonw Issues: None.
 *****************************************************************************/
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
#include <QComboBox>
#include <QLineEdit>


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

    // Add a box in the status bar to show the color under the cursor
    status_color_swatch = new QLabel();
    QPalette pal = status_color_swatch->palette();
    pal.setColor(status_color_swatch->backgroundRole(), Qt::black);
    status_color_swatch->setPalette(pal);
    status_color_swatch->setAutoFillBackground(true);
    status_color_swatch->setMinimumSize(50,5);
    ui->statusBar->addPermanentWidget(status_color_swatch);

    // Add a drop-down box to select common zoom sizes or to type in your own size
    zoom_box = new QComboBox();
    zoom_box->addItem("300");
    zoom_box->addItem("200");
    zoom_box->addItem("150");
    zoom_box->addItem("100");
    zoom_box->addItem("75");
    zoom_box->addItem("50");
    zoom_box->addItem("25");
    zoom_box->addItem("10");
    zoom_box->setInsertPolicy(QComboBox::NoInsert);
    zoom_box->setValidator(new QIntValidator(10,300,zoom_box));
    zoom_box->setEditable(true);
    zoom_box->setEnabled(false);

    ui->toolBar->addWidget(zoom_box);

    // Add a spacer on the toolbar
    QWidget *w = new QWidget();
    w->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    w->setMinimumSize(4, 4);
    ui->toolBar->addWidget(w);

    // Add a slider to control zoom as well
    zoom_slider = new QSlider(Qt::Vertical);
    zoom_slider->setRange(10, 300);
    zoom_slider->setSingleStep(10);
    zoom_slider->setValue(100);
    zoom_slider->setTickInterval(40);
    zoom_slider->setTickPosition(QSlider::TicksRight);
    zoom_slider->setEnabled(false);
    zoom_slider->setMinimumWidth(40);
    zoom_slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // Note, if you move the toolbar, the slider changes direction :)
    zoom_slider->connect(ui->toolBar, SIGNAL(orientationChanged(Qt::Orientation)), SLOT(setOrientation(Qt::Orientation)));

    ui->toolBar->addWidget(zoom_slider);

    // Also, you can resize to fit the window
    ui->toolBar->addAction(ui->actionFit_window);

    // Sync the combo box and slider
    connect(zoom_box, SIGNAL(editTextChanged(QString)), SLOT(zoomBoxChanged(QString)));
    connect(zoom_box, SIGNAL(activated(QString)), SLOT(zoomBoxChanged(QString)));
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
    if (!child)
        return 0;
    return (ImgWin*)(child->widget());
}

/******************************************************************************
 * QImage getImage(): Get the current image window's image
 * Private function.
 * Finds the current image window and returns the image.
 *****************************************************************************/
QImage MainWindow::getImage()
{
    ImgWin *cur = getCurrent();
    if (cur)
        return cur->getImage();
    return QImage();
}

/******************************************************************************
 * setImage(QImage): Set the current image window's image
 * Private function.
 * Finds the current image window and sets the image.
 *****************************************************************************/
void MainWindow::setImage(QImage p)
{
    getCurrent()->setImage(p);
}

/******************************************************************************
 * doOpen(void): Open a document.
 * Slot function.
 * Signalers: actionOpen
 * Attempts to open a document using Qt, and creates a child window for the
 * image. If the user cancels or they choose an invalid file, no window is
 * created.
 *****************************************************************************/
void MainWindow::doOpen()
{
    QString file = QFileDialog::getOpenFileName(this, "Open Image", "",
        "Image Files (*.png *.jpg *.bmp *.gif);;Any Files (*.*)");

    if (file != "")
    {
        QImageReader *reader = new QImageReader(file);
        QImage img = reader->read();
        if (!img.isNull())
        {
            ImgWin* win = new ImgWin;
            win->setImage(img);
            win->setWindowTitle(file);
            win->setReader(reader);

            // Display the image window
            QMdiSubWindow* subwin = ui->mdiArea->addSubWindow(win);
            subwin->showMaximized();

            // Add a item to the window menu, and give it to the imgwin to keep track of
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
        getImage().save(file);
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
    // Must happen before the message box, because it deactivates the subwindow
    ImgWin *win = getCurrent();
    if (QMessageBox::Ok == QMessageBox::question(this, "Revert file",
        "Do you want to revert? You will lose all your changes.",
        QMessageBox::Ok | QMessageBox::Cancel))
    {
        win->setImage(win->getReader()->read());
        win->getReader()->setFileName(win->getReader()->fileName());
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
    QImage img = getImage();
    for(int i = 0; i < img.width(); ++i)
    {
        for(int j = 0; j < img.height(); ++j)
        {
            QRgb old_color = img.pixel(i, j);
            old_color = qRgb(0xFF - qRed(old_color), 0xFF - qGreen(old_color), 0xFF - qBlue(old_color));
            img.setPixel(i, j, old_color);
        }
    }
    setImage(img);
}

/******************************************************************************
 * doSharpen(void): Apply a sharpen filter to an image.
 * Slot function.
 * Signalers: actionSharpen()
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
    QImage img = getImage();
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
    setImage(out_img);
}

/******************************************************************************
 * doSmooth(void): Apply a smooth filter to an image.
 * Slot function.
 * Signalers: actionSmooth()
 * Attempts to sharpen an image by applying a
 * [1  2  1]
 * [2  4  2]
 * [1  2  1]
 * filter to each pixel in succession.
 * Each color has the filter applied seperately, and if a color would exceed
 * 0xFF, it is clipped to 0xFF. If a color would go below 0, it is clipped to
 * 0.
 *****************************************************************************/
void MainWindow::doSmooth()
{
    QImage img = getImage();
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
    setImage(out_img);
}

/******************************************************************************
 * doZoom(): Change the view zoom.
 * Slot function.
 * Signalers: actionZoom()
 * Creates a new zoom dialog targeted at the active window.
 *****************************************************************************/
void MainWindow::doZoom()
{
    ZoomDialog *zoom = new ZoomDialog(this);
    zoom->setTarget(getCurrent());
    zoom->show();
}

/******************************************************************************
 * doFillWindow(): Change the zoom to hold the whole image.
 * Slot function.
 * Signalers: actionFillWindow()
 * Zoom the image to fit it inside the image window.
 *****************************************************************************/
void MainWindow::doFillWindow()
{
    ImgWin *c = getCurrent();
    c->setScale(std::min(c->contentsRect().width() * 100.0 / getImage().width(),
                         c->contentsRect().height() * 100.0 / getImage().height()));
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

    QRect sel = win->getSelection();
    if (sel.isEmpty())
    {
        QMessageBox::warning(this, "Crop", "Press select a region to crop.", QMessageBox::Ok);
        return;
    }

    setImage(getImage().copy(win->getSelection()));
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
    resize->widthSpin->setValue(getImage().size().width());
    resize->heightSpin->setValue(getImage().size().height());
    resize->widthSpin->setRange(1,1000000);
    resize->heightSpin->setRange(1,1000000);

    if (d->exec() == QDialog::Accepted)
    {
        setImage(getImage().scaled(QSize(resize->widthSpin->value(),resize->heightSpin->value())));
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

    zoom_box->setDisabled(win == 0);
    zoom_slider->setDisabled(win == 0);
    zoom_slider->disconnect();
    connect(zoom_slider, SIGNAL(valueChanged(int)), SLOT(zoomChanged(int)));
    if (win)
    {
        ImgWin *c = getCurrent();

        zoom_slider->setValue(c->getScale());
        zoom_box->setEditText(QString::number(c->getScale()));
        c->connect(zoom_slider, SIGNAL(valueChanged(int)), SLOT(setScale(int)));
        zoom_slider->connect(c, SIGNAL(scaleChanged(int)), SLOT(setValue(int)));
    }

    emit imageEditable(win != 0);
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

/******************************************************************************
 * doSliders(): Show the image color adjustment dialog.
 * Slot function.
 * Signalers: actionColorAdjust
 * Display the color adjustment dialog and wait for it to complete.
 *****************************************************************************/
void MainWindow::doSliders()
{
    SliderDialog *d = new SliderDialog(ui->mdiArea->activeSubWindow());
    QMdiSubWindow *child = ui->mdiArea->activeSubWindow();
    ImgWin *win = (ImgWin*)(child->widget());
    d->setTarget(win);
    d->setImage(win->getImage());
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
    QImage i = getImage();
    if (i.isNull())
        return;
    QColor c(i.pixel(p));
    QString msg = QString("%1,%2 (%3,%4) [h%5 s%6 v%7]")
            .arg(p.x()).arg(p.y())
            .arg(i.width()).arg(i.height())
            .arg(c.hue()).arg(c.saturation()).arg(c.value());
    ui->statusBar->showMessage(msg);
    QPalette pal = status_color_swatch->palette();
    pal.setColor(status_color_swatch->backgroundRole(), c);
    status_color_swatch->setPalette(pal);
}

/******************************************************************************
 * zoomChanged(int): Change the zoom combobox to the current zoom.
 * Slot function.
 * Signalers: zoom_slider->valueChanged(int)
 * Get the zoom value from the slider and set the box to it.
 *****************************************************************************/
void MainWindow::zoomChanged(int s)
{
    zoom_box->setEditText(QString::number(s));
}

/******************************************************************************
 * zoomBoxChanged(int): Change the zoom to the zoom combobox's value.
 * Slot function.
 * Signalers: zoom_box->editTextChanged(int)
 * Get the zoom value from the box and set the window to it.
 *****************************************************************************/
void MainWindow::zoomBoxChanged(QString str)
{
    int n = str.toInt();
    zoom_slider->setValue(n);
}
