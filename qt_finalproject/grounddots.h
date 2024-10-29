#ifndef GROUNDDOTS_H
#define GROUNDDOTS_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QTimer>
#include <QVector>

class GroundDots : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    explicit GroundDots(QObject* parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setSpeed(qreal newSpeed);  // 속도 설정 함수 추가


public slots:
    void updatePosition();
    void stopAnimation();
    void startAnimation();

private:
    QTimer* moveTimer;
    QVector<QPointF> dots;  // 점들의 위치를 저장
    const int DOT_SPACING = 20;  // 점들 사이의 간격
    qreal speed;  // SPEED 상수를 speed 변수로 변경
};

#endif
