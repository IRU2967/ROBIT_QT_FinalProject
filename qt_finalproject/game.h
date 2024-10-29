
#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include <QRandomGenerator>  // 랜덤 숫자 생성을 위해 추가
#include <QObject>

#include "dino.h"
#include "obstacle.h"
#include "grounddots.h"

class Game : public QGraphicsView
{
    Q_OBJECT

public:
    Game(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;
    void saveScore();  // 점수 저장
    void loadScores(); // 점수 불러오기
    void showRanking(); // 랭킹 표시

signals:  // signals 섹션 추가
    void scoreChanged(int score);  // 시그널 선언 추가


private slots:
    void update();
    void spawnObstacle();
    void restartGame();
private:
    QGraphicsScene *scene;
    GroundDots* groundDots;
    Dino *dino;
    QTimer *gameTimer;
    QTimer *obstacleTimer;
    bool gameOver;
    int score;
    const qreal INITIAL_SPEED = 5.0;
    const qreal MAX_SPEED = 12.0;      // 최대 속도
    qreal currentSpeed = INITIAL_SPEED; // 현재 속도
    QVector<int> highScores;  // 상위 점수들을 저장할 벡터
    static const int MAX_RANKINGS = 5;  // 최대 랭킹 수
};

#endif // GAME_H
