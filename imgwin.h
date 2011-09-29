#ifndef IMGWIN_H
#define IMGWIN_H

#include <QScrollArea>

namespace Ui {
    class ImageWindow;
}

class ImgWin : public QScrollArea
{
    Q_OBJECT
public:
    explicit ImgWin(QWidget *parent = 0);
    ~ImgWin();

    const QPixmap* getPixmap();
    void setPixmap(QPixmap image);

private:
    Ui::ImageWindow *ui;
};

#endif // IMGWIN_H
