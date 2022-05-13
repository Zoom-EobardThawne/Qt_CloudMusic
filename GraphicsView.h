#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QImage>
#include <QGraphicsScene>
#include "DiskItem.h"
#include "NeedleItem.h"

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = nullptr);
    ~GraphicsView();

    void setImage(const QImage &musicImage);

    QImage makeARGB32Image(QImage musicImage);
    QImage makeCircleImage(QImage argb32Image);
    QImage makeDiskImage(QImage circleImage);

    void startAnimation();
    void pauseAnimation();
    void stopAnimation();

private:
    void paintEvent(QPaintEvent *pe);

    void resizeEvent(QResizeEvent *re);
signals:

public slots:

private:
    QImage image;
    QGraphicsScene *scene; //场景
    DiskItem *diskItem; //disk图元
    NeedleItem *needleItem; //needle图元
};

#endif // GRAPHICSVIEW_H
