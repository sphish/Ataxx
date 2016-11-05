#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *mouseEvent);
    void checkWin();
    void winShow(int stand);
    void clear();

    int xPressed;
    int yPressed;
    int first;
    int numCounts;
    int stepCounts;
    int checkBoard[7][7];

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
