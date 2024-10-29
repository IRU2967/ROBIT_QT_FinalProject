#include "game.h"
#include "dino.h"
#include "obstacle.h"
#include "grounddots.h"
#include <QGraphicsScene>

#include <QKeyEvent>
#include <QGraphicsTextItem>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <algorithm>

Game::Game(QWidget *parent) : QGraphicsView(parent), gameOver(false), score(0)
{
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 400);

    // 배경색을 회색으로 설정
    scene->setBackgroundBrush(QBrush(QColor(240, 240, 240)));
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(800, 400);

    // 공룡 생성 및 추가 - 땅 다음에 생성
    dino = new Dino();
    scene->addItem(dino);

    // 움직이는 점들 추가
    groundDots = new GroundDots();
    scene->addItem(groundDots);

    // 게임 타이머 설정 - 이 부분 추가
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &Game::update);
    gameTimer->start(16);  //약 60fps

    // 장애물 생성 타이머 설정
    obstacleTimer = new QTimer(this);
    connect(obstacleTimer, &QTimer::timeout, this, &Game::spawnObstacle);

    // 초기 장애물 생성 간격을 랜덤하게 설정
    int initialDelay = QRandomGenerator::global()->bounded(1500, 3000);
    obstacleTimer->start(initialDelay);

    loadScores();  // 게임 시작시 이전 점수들을 로드
}

void Game::restartGame()
{
    // 기존 공룡 제거
    if(dino) {
        scene->removeItem(dino);
        delete dino;
    }

    if(groundDots) {
        delete groundDots;
        groundDots = new GroundDots();
        scene->addItem(groundDots);
        groundDots->setSpeed(INITIAL_SPEED);  // 초기 속도 설정
    }

    // 모든 장애물 제거
    QList<QGraphicsItem*> items = scene->items();
    for (auto item : items) {
        Obstacle* obstacle = dynamic_cast<Obstacle*>(item);
        if (obstacle) {
            scene->removeItem(obstacle);
            delete obstacle;
        }
    }

    // 텍스트 아이템 제거
    for (auto item : scene->items()) {
        QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);
        if (textItem) {
            scene->removeItem(textItem);
            delete textItem;
        }
    }

    // 새 공룡 생성
    dino = new Dino();
    scene->addItem(dino);

    // 게임 상태 초기화
    gameOver = false;
    score = 0;
    currentSpeed = INITIAL_SPEED;

    loadScores();  // 최신 점수 다시 로드

    // 타이머 재시작
    gameTimer->start(16);
    int initialDelay = QRandomGenerator::global()->bounded(1500, 3000);
    obstacleTimer->start(initialDelay);

    if(groundDots) {
        delete groundDots;
        groundDots = new GroundDots();
        scene->addItem(groundDots);
        groundDots->startAnimation();  // 애니메이션 시작
    }
}

void Game::update()
{
    if (gameOver) return;

    dino->update();
    score++;
    emit scoreChanged(score);  // 점수가 변경될 때마다 시그널 발생

    // 점수에 따른 속도 증가
    currentSpeed = qMin(INITIAL_SPEED + (score / 500) * 0.5, MAX_SPEED);

    // 지면 속도 업데이트
    groundDots->setSpeed(currentSpeed);

    // 장애물 업데이트
    QList<QGraphicsItem*> itemsToRemove;  // 제거할 아이템들을 저장할 리스트

    for (auto item : scene->items()) {
        Obstacle* obstacle = dynamic_cast<Obstacle*>(item);
        if (obstacle) {
            // 현재 속도로 장애물 이동
            obstacle->setX(obstacle->x() - currentSpeed);

            // 충돌 감지
            if (dino->collidesWithItem(obstacle)) {
                gameOver = true;
                gameTimer->stop();
                obstacleTimer->stop();
                groundDots->stopAnimation();  // 바닥 애니메이션 정지
                dino->die();  // 공룡 죽음 처리

                saveScore();  // 점수 저장

                QGraphicsTextItem* gameOverText = scene->addText("Game Over - Press Space to Restart");
                gameOverText->setDefaultTextColor(Qt::black);
                QFont font = gameOverText->font();
                font.setPointSize(20);
                gameOverText->setFont(font);
                gameOverText->setPos(250, 150);

                QString scoreText = QString("Score: %1").arg(score);
                QGraphicsTextItem* scoreItem = scene->addText(scoreText);
                scoreItem->setDefaultTextColor(Qt::black);
                scoreItem->setPos(350, 200);

                showRanking();  // 랭킹 표시
                return;
            }

            // 화면 밖으로 나간 장애물 체크
            if (obstacle->x() < -50) {
                itemsToRemove.append(obstacle);
            }
        }
    }

    // 화면 밖으로 나간 장애물 제거
    for (auto item : itemsToRemove) {
        scene->removeItem(item);
        delete item;
    }
}

void Game::spawnObstacle()
{
    if (!gameOver) {
        // 랜덤하게 장애물 타입 선택
        int random = QRandomGenerator::global()->bounded(100);
        Obstacle::Type type;

        // 점수가 높아질수록 BIRD와 BIG_CACTUS의 출현 확률 증가
        // 출현 확률 조정
        int birdProb = qMin(20 + (score / 300), 40);  // 더 빨리, 더 높은 40%확률로 증가
        int bigCactusProb = qMin(20 + (score / 300), 40);

        if (random < birdProb) {
            type = Obstacle::BIRD;
        } else if (random < (birdProb + bigCactusProb)) {
            type = Obstacle::BIG_CACTUS;
        } else {
            type = Obstacle::CACTUS;
        }

        Obstacle* obstacle = new Obstacle(type);
        scene->addItem(obstacle);

        // 점수에 따라 생성 간격 감소
        int baseMinInterval = 1500;  // 기본 최소 간격
        int baseMaxInterval = 3000;  // 기본 최대 간격

        // 점수가 300점 증가할 때마다 간격 감소
        int reduction = (score / 300) * 150;  // 더 빠르게 간격 감소

        // 최소/최대 간격 제한
        int minInterval = qMax(600, baseMinInterval - reduction);   // 최소 600ms까지만 감소
        int maxInterval = qMax(1500, baseMaxInterval - reduction); // 최소 1500ms까지만 감소

        int randomInterval = QRandomGenerator::global()->bounded(minInterval, maxInterval);
        obstacleTimer->setInterval(randomInterval);
    }
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        if (!gameOver) {
            dino->jump();
        } else {
            restartGame();
        }
    }
}


void Game::saveScore()
{
    // 현재 점수를 highScores에 추가
    highScores.append(score);

    // 내림차순으로 정렬
    std::sort(highScores.begin(), highScores.end(), std::greater<int>());

    // 상위 5개만 유지
    while (highScores.size() > MAX_RANKINGS) {
        highScores.removeLast();
    }

    // 파일에 저장
    QFile file("scores.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int score : highScores) {
            out << score << "\n";
        }
        file.close();
    }
}

void Game::loadScores()
{
    highScores.clear();
    QFile file("scores.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd() && highScores.size() < MAX_RANKINGS) {
            QString line = in.readLine();
            bool ok;
            int score = line.toInt(&ok);
            if (ok) {
                highScores.append(score);
            }
        }
        file.close();
    }
}

void Game::showRanking()
{
    // 기존 랭킹 텍스트 제거
    for (auto item : scene->items()) {
        QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);
        if (textItem) {
            scene->removeItem(textItem);
            delete textItem;
        }
    }

    // 랭킹 표시
    QGraphicsTextItem* titleText = scene->addText("High Scores");
    titleText->setDefaultTextColor(Qt::black);
    QFont titleFont = titleText->font();
    titleFont.setPointSize(20);
    titleText->setFont(titleFont);
    titleText->setPos(350, 100);

    for (int i = 0; i < highScores.size(); ++i) {
        QString rankText = QString("%1. %2").arg(i + 1).arg(highScores[i]);
        QGraphicsTextItem* rankItem = scene->addText(rankText);
        rankItem->setDefaultTextColor(Qt::black);
        rankItem->setPos(350, 150 + i * 30);
    }
}
