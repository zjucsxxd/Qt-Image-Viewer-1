#include "ui_imgwin.h"
#include "imgwin.h"
#include <QCloseEvent>

/******************************************************************************
 * ImgWin(QWidget*): Creates the ImgWin class
 * Creates the image window, passing the parent to QScrollArea, and
 * initializing the ui.
 *****************************************************************************/
ImgWin::ImgWin(QWidget *parent) :
    QScrollArea(parent), ui(new Ui::ImageWindow)
{
    ui->setupUi(this);
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
    ui->scrollAreaWidgetContents->adjustSize();
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

void ImgWin::scale(int factor)
{
    ui->scrollAreaWidgetContents->resize(ui->pictureLabel->pixmap()->size() * (factor/100.0));
}
