#ifndef SLIDERDIALOG_H
#define SLIDERDIALOG_H

#include <QDialog>

namespace Ui {
    class SliderDialog;
}

class SliderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SliderDialog(QWidget *parent = 0);
    ~SliderDialog();

private:
    Ui::SliderDialog *ui;
};

#endif // SLIDERDIALOG_H
