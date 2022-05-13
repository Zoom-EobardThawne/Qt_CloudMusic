#include "DiskItem.h"
#include <QPainter>

DiskItem::DiskItem(QObject *parent) : QObject(parent),
    diskImage(":/image/disk.png"),
    animation(new QPropertyAnimation(this,"anagle"))
{
    /*动画属性参数*/
    animation->setDuration(10000); //执行一次时间
    animation->setStartValue(0); //起始角度
    animation->setEndValue(360); //结束角度
    animation->setLoopCount(-1); //循环次数，无限
}

QRectF DiskItem::boundingRect() const
{
    //return diskImage.rect();
    //图元的坐标系原点在中心
    return QRectF(diskImage.width()* -0.5,diskImage.height()* -0.5, diskImage.width(), diskImage.height());
}

void DiskItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawImage(boundingRect(), diskImage);
}

void DiskItem::setDiskImage(const QImage &image)
{
    diskImage = image;
    update();

}

void DiskItem::diskStart()
{
    animation->start();
}

void DiskItem::diskPause()
{
    animation->pause();
}

void DiskItem::diskStop()
{
    animation->stop();
}
