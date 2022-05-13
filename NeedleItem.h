#ifndef NEEDLEITEM_H
#define NEEDLEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QImage>
#include <QPropertyAnimation>


class NeedleItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(qreal anagle  READ rotation WRITE setRotation)
public:
    explicit NeedleItem(QObject *parent = nullptr);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

    void setNeedleImage(const QImage &image);
    /*控制唱针动画效果*/
    void needleStart();
    void needleStop();

signals:

public slots:

private:
    QImage needleImage;
    QPropertyAnimation *animation; //动画指针
};

#endif // NEEDLEITEM_H
