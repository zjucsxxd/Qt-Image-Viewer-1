#ifndef IMAGEEDITLABEL_H
#define IMAGEEDITLABEL_H

#include <QLabel>
#include <QRubberBand>
#include <QMouseEvent>

class ImageEditLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ImageEditLabel(QWidget *parent = 0);

    QRect getImgSelection()
    {
        return rubber->geometry();
    }

    void setImgSelection(QRect r)
    {
        rubber->setGeometry(r);
    }

signals:
    void mouseOverInfo(QPoint);

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

private:
    QRubberBand* rubber;
    QPoint dragStart;
};

#endif // IMAGEEDITLABEL_H
