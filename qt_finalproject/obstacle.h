// obstacle.h
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QObject>
#include <QTimer>

class Obstacle : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    enum Type { CACTUS, BIRD , BIG_CACTUS};
    explicit Obstacle(Type type = CACTUS, QObject* parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public slots:
    void switchFrame();

private:
    const qreal speed = 5.0;
    QPixmap cactusPixmap;
    QPixmap bigCactusPixmap;
    QPixmap birdPixmap;
    QPixmap birdPixmap1;
    Type obstacleType;
    bool isFirstFrame;
    QTimer* animationTimer;
    int frameWidth;
    int frameHeight;
};

#endif
