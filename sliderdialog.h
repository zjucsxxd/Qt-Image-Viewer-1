/******************************************************************************
 * sliderdialog.h
 * Class prototypes and methods for the slider dialog (Color Adjust window).
 * Details about the file can be found in sliderdialog.cpp to avoid redundancy.
 * Author: Christopher Jensen
 *****************************************************************************/

#ifndef SLIDERDIALOG_H
#define SLIDERDIALOG_H

#include <QDialog>
#include "imgwin.h"

namespace Ui {
    class SliderDialog;
}

class SliderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SliderDialog(QWidget *parent = 0);
    ~SliderDialog();
    void setTarget(ImgWin* win){target = win;}
    void setImage(QImage);

public slots:
    /* To be implemented if time permits
    void setBrightness();
    void setContrast();
    */
    void setColor();
    void commit();

private:
    Ui::SliderDialog *ui;
    const QPixmap *getPixmap();
    /* The window we will commit any changes to */
    ImgWin* target;
    /* To be used if we later decide to implement brightness adjusts.
    int bright_index;
    */
};

#endif // SLIDERDIALOG_H
