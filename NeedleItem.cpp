#include "NeedleItem.h"
#include <QPainter>

NeedleItem::NeedleItem(QObject *parent) : QObject(parent),
    needleImage(":/image/cp_z.png"),
    animation(new QPropertyAnimation(this,"anagle"))
{
    /*动画属性参数*/
    animation->setDuration(100); //执行一次时间
    animation->setLoopCount(1); //循环次数 1

    setTransformOriginPoint(200,-100);
}

QRectF NeedleItem::boundingRect() const
{
     return QRectF(needleImage.width()* -0.5,needleImage.height()* -0.5, needleImage.width(), needleImage.height());
}

void NeedleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    needleImage = needleImage.scaled(160, 160, Qt::KeepAspectRatio);
    painter->drawImage(boundingRect(), needleImage);
}

void NeedleItem::setNeedleImage(const QImage &image)
{
    needleImage = image;
    update();

}

void NeedleItem::needleStart()
{
    animation->setStartValue(0); //起始角度
    animation->setEndValue(10); //结束角度
    animation->start();
}

void NeedleItem::needleStop()
{
    animation->setStartValue(10); //起始角度
    animation->setEndValue(0); //结束角度
    animation->start();
}
