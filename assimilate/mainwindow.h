#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPen>
#include <QWidget>
#include <QPushButton>
#include <QBrush>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *mouseEvent);

    int xPressed;
    int yPressed;
    int first;
    int numCounts;
    bool turn;
    int checkBoard[7][7];

    QFont font;
    QPushButton *newGame, *abouts, *recall, *place, *readFile, *pass;

public slots:
    void clear();
    void checkWin();
    void winShow(int stand);
};

#endif // MAINWINDOW_H
