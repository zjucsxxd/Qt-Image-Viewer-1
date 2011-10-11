#ifndef ZOOMDIALOG_H
#define ZOOMDIALOG_H

#include "imgwin.h"
#include <QDialog>

namespace Ui {
    class ZoomDialog;
}

class ZoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ZoomDialog(QWidget *parent = 0);
    ~ZoomDialog();
    void setTarget(ImgWin*);
public slots:
    void commit();

private:
    Ui::ZoomDialog *ui;
    ImgWin* target;
};

#endif // ZOOMDIALOG_H
