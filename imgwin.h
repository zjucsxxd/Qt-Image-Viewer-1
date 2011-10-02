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

    void setMenuItem(QAction* a) { menu_item = a; }

signals:
    void closing(QAction*);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::ImageWindow *ui;
    QAction* menu_item;
};

#endif // IMGWIN_H
