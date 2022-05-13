#ifndef DISKITEM_H
#define DISKITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QImage>
#include <QPropertyAnimation>

class DiskItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(qreal anagle  READ rotation WRITE setRotation)
public:
    explicit DiskItem(QObject *parent = nullptr);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

    void setDiskImage(const QImage &image);

    /*控制唱片动画效果*/
    void diskStart();
    void diskPause();
    void diskStop();

signals:

public slots:

private:
    QImage diskImage;
    QPropertyAnimation *animation; //动画指针
};

#endif // DISKITEM_H
