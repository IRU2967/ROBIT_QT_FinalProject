#include "obstacle.h"

Obstacle::Obstacle(Type type, QObject* parent)
    : QObject(parent), QGraphicsItem(), obstacleType(type), isFirstFrame(true)
{
    // 이미지 로드
    cactusPixmap.load(":/images/cactus.png");
    bigCactusPixmap.load(":/images/big_cactus.png");
    birdPixmap.load(":/images/bird.png");
    birdPixmap1.load(":/images/bird1.png");

    // 이미지 크기 설정
    frameWidth = 20;
    frameHeight = 40;
    int bigCactusWidth = 30;    // 큰 선인장 크기
    int bigCactusHeight = 60;
    int birdWidth = 30;    // 새의 너비
    int birdHeight = 35;   // 새의 높이

    // 각 이미지 크기 조정
    cactusPixmap = cactusPixmap.scaled(frameWidth, frameHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    bigCactusPixmap = bigCactusPixmap.scaled(bigCactusWidth, bigCactusHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    birdPixmap = birdPixmap.scaled(birdWidth, birdHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    birdPixmap1 = birdPixmap1.scaled(birdWidth, birdHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 픽셀화된 이미지 품질 유지
    cactusPixmap.setDevicePixelRatio(1);
    bigCactusPixmap.setDevicePixelRatio(1);  // 추가
    birdPixmap.setDevicePixelRatio(1);
    birdPixmap1.setDevicePixelRatio(1);

    // 시작 위치 설정
    if (type == BIRD) {
        setPos(800, 150);
    } else if (type == BIG_CACTUS) {
        setPos(800, 180);
    } else {
        setPos(800, 200);
    }

    // 타이머 설정 (BIRD 타입일 경우)
    if (type == BIRD) {
        animationTimer = new QTimer(this);
        connect(animationTimer, &QTimer::timeout, this, &Obstacle::switchFrame);
        animationTimer->start(200);
    }
}

QRectF Obstacle::boundingRect() const
{
    if (obstacleType == BIRD) {
        return QRectF(0, 0, 30, 35);  // 새의 크기
    } else if (obstacleType == BIG_CACTUS) {
        return QRectF(0, 0, 30, 60);  // 큰 선인장의 크기
    }
    return QRectF(0, 0, frameWidth, frameHeight);  // 일반 선인장 크기
}

void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
    painter->setRenderHint(QPainter::Antialiasing, false);

    if (obstacleType == CACTUS) {
        painter->drawPixmap(0, 0, cactusPixmap);
    } else if (obstacleType == BIG_CACTUS) {  // 큰 선인장 추가
        painter->drawPixmap(0, 0, bigCactusPixmap);
    } else {
        // BIRD 타입
        if (isFirstFrame) {
            painter->drawPixmap(0, 0, birdPixmap);
        } else {
            painter->drawPixmap(0, 0, birdPixmap1);
        }
    }
}

void Obstacle::switchFrame()
{
    isFirstFrame = !isFirstFrame;
    update();  // 화면 갱신
}
