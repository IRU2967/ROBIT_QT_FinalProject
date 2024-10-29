#ifndef DINO_H
#define DINO_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QObject>

class Dino : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    explicit Dino(QObject* parent = nullptr);  // parent 매개변수 추가
    virtual ~Dino() override;  // virtual 키워드 추가
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void jump();
    void update();
    bool isJumping() const { return jumping; }
    void die();            // 죽음 함수 추가
    void reset();  // 상태 초기화 함수 추가

private:
    bool jumping;
    qreal velocity;
    const qreal gravity = 0.8;
    const qreal jumpForce = -15.0;
    const qreal groundY = 200.0;
    QPixmap dinoPixmap;    // 달리기 이미지1
    QPixmap dinoPixmap1;   // 달리기 이미지2
    QPixmap dinoPixmap2;   // 점프 이미지
    QPixmap dinoPixmap3;   // 죽었을 때 이미지로 이름 변경
    bool isDead;
    QTimer* animationTimer;
    bool isFirstFrame;
    int frameWidth;
    int frameHeight;
    int animationState;

private slots:
    void switchFrame();
};

#endif // DINO_H
