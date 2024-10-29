#include "grounddots.h"

// 생성자 정의
GroundDots::GroundDots(QObject* parent)
    : QObject(parent), QGraphicsItem(), speed(5.0)
{
    // 초기 점들 생성
    for(int x = 0; x < 800; x += DOT_SPACING) {
        dots.append(QPointF(x, 252));
    }

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &GroundDots::updatePosition);
    moveTimer->start(16);
}

// boundingRect 함수 정의
QRectF GroundDots::boundingRect() const
{
    return QRectF(0, 250, 800, 10);  // 점들이 그려질 영역
}

// paint 함수 정의
void GroundDots::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(Qt::black);
    painter->setBrush(Qt::black);

    // 모든 점 그리기
    for(const QPointF& dot : dots) {
        painter->drawRect(dot.x(), dot.y(), 2, 2);  // 2x2 크기의 작은 사각형으로 점 표현
    }
}

// updatePosition 함수 정의
void GroundDots::updatePosition()
{
    // 모든 점을 왼쪽으로 이동
    for(QPointF& dot : dots) {
        dot.setX(dot.x() - speed);

        // 화면 왼쪽을 벗어난 점을 오른쪽으로 이동
        if(dot.x() < -2) {
            dot.setX(800);
        }
    }
    update();  // 화면 갱신
}

// stopAnimation 함수 정의
void GroundDots::stopAnimation()
{
    if (moveTimer) {
        moveTimer->stop();
    }
}

// startAnimation 함수 정의
void GroundDots::startAnimation()
{
    if (moveTimer) {
        moveTimer->start(16);
    }
}

// setSpeed 함수 정의
void GroundDots::setSpeed(qreal newSpeed)
{
    speed = newSpeed;
}
