#include "ui_imgwin.h"
#include "imgwin.h"
#include <QCloseEvent>

/******************************************************************************
 * ImgWin(QWidget*): Creates the ImgWin class
 * Creates the image window, passing the parent to QScrollArea, and
 * initializing the ui.
 *****************************************************************************/
ImgWin::ImgWin(QWidget *parent) :
    QScrollArea(parent), ui(new Ui::ImageWindow), scaleFactor(1.0)
{
    ui->setupUi(this);
    connect(ui->pictureLabel, SIGNAL(mouseOverInfo(QPoint)), SLOT(labelMouseInfo(QPoint)));
}

/******************************************************************************
 * ~ImgWin(): Destroys the ImgWin class
 * Destroys the image window, deleting the ui.
 *****************************************************************************/
ImgWin::~ImgWin()
{
    delete ui;
}

/******************************************************************************
 * QPixmap* getPixmap(): Get the image
 * Public function.
 * Gets the current image from the pictureLabel.
 *****************************************************************************/
const QPixmap* ImgWin::getPixmap()
{
    QLabel *label = ui->pictureLabel;
    return label->pixmap();
}

/******************************************************************************
 * setPixmap(QPixmap): Set the image
 * Public function.
 * Sets the current image in the pictureLabel.
 *****************************************************************************/
void ImgWin::setPixmap(QPixmap image)
{
    ui->pictureLabel->setPixmap(image);
    ui->scrollAreaWidgetContents->resize(image.size() * scaleFactor);
    ui->pictureLabel->setImgSelection(QRect(0,0,0,0));
}

/******************************************************************************
 * QRect setSelection(QPixmap): Get the mouse selection of the image
 * Public function.
 * Get the selection of the user on the pictureLabel.
 *****************************************************************************/
QRect ImgWin::getSelection()
{
    QRect sel = ui->pictureLabel->getImgSelection();
    return QRect(sel.topLeft() / scaleFactor, sel.size() / scaleFactor);
}

/******************************************************************************
 * closeEvent(QCloseEvent*): Event handler when the window is closed.
 * Overridden protected function.
 * When the window is closed, let the world know that the menu_item is no
 * longer valid.
 *****************************************************************************/
void ImgWin::closeEvent(QCloseEvent *event)
{
    emit closing(menu_item);
    event->accept();
}

/******************************************************************************
 * setScale(int): Change the image zoom.
 * Public function.
 * Set the scale factor for the zoom, and resize the view and selection.
 *****************************************************************************/
void ImgWin::setScale(int factor)
{
    QRect sel = getSelection();
    scaleFactor = factor / 100.0;
    ui->scrollAreaWidgetContents->resize(ui->pictureLabel->pixmap()->size() * scaleFactor);
    ui->pictureLabel->setImgSelection(QRect(sel.topLeft() * scaleFactor, sel.size() * scaleFactor));
    emit scaleChanged(scaleFactor * 100);
}
