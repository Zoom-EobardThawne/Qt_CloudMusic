#include "GraphicsView.h"
#include <QPainter>
#include <QResizeEvent>

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent),
    scene(new QGraphicsScene),
    diskItem(new DiskItem),
    needleItem(new NeedleItem)
{
    //image.load(":/image/disk.png");

    setScene(scene);
    scene->addItem(diskItem);
    scene->addItem(needleItem);

    //needleItem->hide();
}

GraphicsView::~GraphicsView()
{
    if (diskItem != NULL) {
        delete diskItem;
        diskItem = NULL;
    }
    if (scene != NULL) {
        delete scene;
        scene =NULL;
    }
    if (needleItem != NULL) {
        delete needleItem;
        needleItem = NULL;
    }

}

void GraphicsView::setImage(const QImage &musicImage)
{
    //image = musicImage;
    update();
    QImage circle= makeCircleImage( makeARGB32Image(musicImage));
    QImage disk = makeDiskImage(circle);
    diskItem->setDiskImage(disk);
}

QImage GraphicsView::makeARGB32Image(QImage musicImage)
{ //等比缩小，和":/image/circleMask.png" 一样大，且是ARGB32格式的图片
    QImage circle(":/image/circleMask.png");
    QSize circleSize = circle.size();
    musicImage = musicImage.scaled(circleSize, Qt::KeepAspectRatio);

    QImage fixedImage(circleSize, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&fixedImage);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(fixedImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(fixedImage.rect(), musicImage);
    painter.end();

    return fixedImage;

}

QImage GraphicsView::makeCircleImage(QImage argb32Image)
{ //将缩小后的专辑图片，裁剪成圆形

    QImage circle(":/image/circleMask.png");

    QPainter painter(&circle);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawImage(circle.rect(), argb32Image);
    painter.end();

    return circle;
}

QImage GraphicsView::makeDiskImage(QImage circleImage)
{ //将圆形的专辑图片和disk.png做组合，拼接成带光盘的专辑图片
    QImage diskImage(":/image/disk.png");

    QPainter painter(&diskImage);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    painter.drawImage(83, 83, circleImage); //手动计算

    /*方法二：获取专辑图片的矩形和disk图片中心点，将矩形移动到中心点
    QRect rect = circleImage.rect();
    QPoint point = diskImage.rect().center();
    rect.moveCenter(point);
    painter.drawImage(rect, circleImage);
    */
    painter.end();

    return diskImage;
}

void GraphicsView::startAnimation()
{
    diskItem->diskStart();
    needleItem->needleStart();
}

void GraphicsView::pauseAnimation()
{
    diskItem->diskPause();
    needleItem->needleStop();
}

void GraphicsView::stopAnimation()
{
    diskItem->diskStop();
}


void GraphicsView::paintEvent(QPaintEvent *pe)
{
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawImage(rect(), image);

    QGraphicsView::paintEvent(pe); //让父类绘制自己的东西
}

void GraphicsView::resizeEvent(QResizeEvent *re)
{
    int x = re->size().width();
    int y = re->size().height();
    scene->setSceneRect(-x/2, -y/2, x, y);
    diskItem->setPos(0, 0);
    needleItem->setPos(175,90);

    QGraphicsView::resizeEvent(re);
}
