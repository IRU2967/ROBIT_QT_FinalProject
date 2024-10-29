#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 게임 생성
    game = new Game(this);

    // 게임을 중앙 위젯으로 설정
    setCentralWidget(game);

    // 점수 레이블을 게임 위에 표시
    ui->scoreLabel->setParent(this);
    ui->scoreLabel->raise();  // 레이블을 최상위로 올림
    ui->scoreLabel->setGeometry(10, 10, 100, 30);  // 위치와 크기 설정

    // 선 추가
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Plain);
    line->setLineWidth(2);
    line->setGeometry(0, 250, 800, 2);
    line->setStyleSheet("background-color: black;");
    line->raise();

    // 시그널/슬롯 연결SLOT
    connect(game, SIGNAL(scoreChanged(int)), this, SLOT(updateScore(int)));

    // 윈도우 크기 설정
    setFixedSize(800, 450);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateScore(int score)
{
    ui->scoreLabel->setText(QString("Score: %1").arg(score));
}
