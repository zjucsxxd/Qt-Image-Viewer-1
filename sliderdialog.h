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

public slots:
    void setBrightness();
    void setTarget(ImgWin* win);
    void setContrast();
    void setRed();
    void setBlue();
    void setGreen();
    void setPixmap(QPixmap*);

private:
    Ui::SliderDialog *ui;
    const QPixmap* getPixmap();
    ImgWin* target;
};

#endif // SLIDERDIALOG_H
