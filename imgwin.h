#ifndef IMGWIN_H
#define IMGWIN_H

#include <QScrollArea>
#include <QImageReader>

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

    void setScale(int);
    int scale()
    {
        return scaleFactor * 100;
    }

    void setMenuItem(QAction* a)
    {
        menu_item = a;
    }

    void setReader(QString inreader)
    {
        reader.setFileName(inreader);
    }
    QImageReader* getReader()
    {
        return &reader;
    }

    QRect getSelection();

signals:
    void closing(QAction*);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::ImageWindow *ui;
    QAction* menu_item;
    QImageReader reader;
    float scaleFactor;
};

#endif // IMGWIN_H
