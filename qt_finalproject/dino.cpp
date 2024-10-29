#include "dino.h"

Dino::Dino(QObject* parent)
    : QObject(parent), QGraphicsItem(), jumping(false), velocity(0),
    isFirstFrame(true), animationState(0), isDead(false)  // isDead 초기화 추가
{
    // 이미지 로드
    dinoPixmap.load(":/images/dino.png");
    dinoPixmap1.load(":/images/dino1.png");
    dinoPixmap2.load(":/images/dino2.png");
     dinoPixmap3.load(":/images/dino3.png");

    // 이미지 크기 설정
    frameWidth = 40;
    frameHeight = 43;
    dinoPixmap = dinoPixmap.scaled(frameWidth, frameHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    dinoPixmap1 = dinoPixmap1.scaled(frameWidth, frameHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    dinoPixmap2 = dinoPixmap2.scaled(frameWidth, frameHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    dinoPixmap3 = dinoPixmap3.scaled(frameWidth, frameHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 픽셀화된 이미지 품질 유지
    dinoPixmap.setDevicePixelRatio(1);
    dinoPixmap1.setDevicePixelRatio(1);
    dinoPixmap2.setDevicePixelRatio(1);
     dinoPixmap3.setDevicePixelRatio(1);

    setPos(50, groundY);

    // 애니메이션 타이머 설정
    animationTimer = new QTimer(this);  // this를 parent로 설정
    connect(animationTimer, SIGNAL(timeout()), this, SLOT(switchFrame()));
    animationTimer->start(150);
}

Dino::~Dino()
{
    if (animationTimer) {
        animationTimer->stop();
        delete animationTimer;
        animationTimer = nullptr;
    }
}

QRectF Dino::boundingRect() const
{
    return QRectF(0, 0, frameWidth, frameHeight);
}

void Dino::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
    painter->setRenderHint(QPainter::Antialiasing, false);


    if (isDead) {
        painter->drawPixmap(0, 0, dinoPixmap3);  // 죽었을 때
    }else if (jumping) {
        painter->drawPixmap(0, 0, dinoPixmap2);
    } else {
        // 달리기 애니메이션
        if (isFirstFrame) {
            painter->drawPixmap(0, 0, dinoPixmap);
        } else {
            painter->drawPixmap(0, 0, dinoPixmap1);
        }
    }
}

void Dino::switchFrame()
{
    // 점프 중이 아닐 때만 애니메이션 실행
    if (!jumping) {
        isFirstFrame = !isFirstFrame;
        this->update();  // QGraphicsItem::update() 호출
    }
}

void Dino::update()
{
    if (jumping) {
        velocity += gravity;
        setY(y() + velocity);

        if (y() >= groundY) {
            setY(groundY);
            jumping = false;
            velocity = 0;
        }
    }
    QGraphicsItem::update();  // 부모 클래스의 update() 호출
}

void Dino::jump()
{
    if (!jumping) {
        jumping = true;
        velocity = jumpForce;
    }
}

void Dino::die()
{
    isDead = true;
    if (animationTimer) {
        animationTimer->stop();  // 죽으면 애니메이션 정지
    }
    update();  // 화면 갱신
}

void Dino::reset()
{
    isDead = false;
    jumping = false;
    velocity = 0;
    isFirstFrame = true;
    setPos(50, groundY);

    // 애니메이션 타이머 재시작
    if (animationTimer) {
        animationTimer->start(150);
    }
}
