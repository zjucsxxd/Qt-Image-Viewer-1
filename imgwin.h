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
    QRect getSelection();

    int getScale() const
    {
        return scaleFactor * 100;
    }

    QImageReader* getReader()
    {
        return &reader;
    }
    void setReader(QString inreader)
    {
        reader.setFileName(inreader);
    }

signals:
    void closing(QAction*);
    void scaleChanged(int);
    void mouseOverInfo(QPoint);

public slots:
    void setScale(int);
    void setMenuItem(QAction* a)
    {
        menu_item = a;
    }

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void labelMouseInfo(QPoint p)
    {
        emit mouseOverInfo(p / scaleFactor);
    }

private:
    Ui::ImageWindow *ui;
    QAction* menu_item;
    QImageReader reader;
    float scaleFactor;
};

#endif // IMGWIN_H
