#ifndef IMGABOUT_H
#define IMGABOUT_H

#include <QDialog>

namespace Ui {
    class imgAbout;
}

class imgAbout : public QDialog
{
    Q_OBJECT

public:
    explicit imgAbout(QWidget *parent = 0);
    ~imgAbout();

private:
    Ui::imgAbout *ui;
};

#endif // IMGABOUT_H
