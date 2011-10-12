#include "imageeditlabel.h"
#include <QRubberBand>

ImageEditLabel::ImageEditLabel(QWidget *parent) :
    QLabel(parent), rubber(new QRubberBand(QRubberBand::Rectangle, this))
{
}

void ImageEditLabel::mousePressEvent(QMouseEvent *ev)
{
    dragStart = ev->pos();
    rubber->setGeometry(QRect(dragStart, QSize()));
    rubber->show();
}

void ImageEditLabel::mouseMoveEvent(QMouseEvent *ev)
{
    if (ev->buttons() & Qt::LeftButton)
        rubber->setGeometry(QRect(dragStart,ev->pos()).normalized());
    emit mouseOverInfo(ev->pos());
}
