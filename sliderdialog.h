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
    void setBrightness();
    void setContrast();
    void setRed();
    void setBlue();
    void setGreen();
    void commit();

private:
    Ui::SliderDialog *ui;
    const QPixmap *getPixmap();
    ImgWin* target;
    int bright_index;
};

#endif // SLIDERDIALOG_H
