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
    QRect getSelection()
    {
        if (!rubber)
            return QRect(0,0,0,0);
        return rubber->geometry();
    }
    void setPixmap(const QPixmap &p)
    {
        if (rubber)
            rubber->setGeometry(0,0,0,0);
        QLabel::setPixmap(p);
    }

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

private:
    QRubberBand* rubber;
    QPoint dragStart;
};

#endif // IMAGEEDITLABEL_H
