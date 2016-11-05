#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstring>
#include <cmath>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow) {
	this->xPressed = 0;
	this->yPressed = 0;
	this->numCounts = 4;
	this->stepCounts = 0;
	this->first = 0;
	memset(checkBoard, 0, sizeof checkBoard);
	checkBoard[0][0] = checkBoard[6][6] = 1;
	checkBoard[0][6] = checkBoard[6][0] = -1;
	this->setMaximumHeight(600);
	this->setMinimumHeight(600);
	this->setMinimumSize(600, 600);
	this->setMaximumSize(600, 600);
	ui->setupUi(this);
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	QPen pen;
	pen.setColor(Qt::blue);
	painter.setPen(pen);

	// Draw checkboard
	for (int i = 20; i < 600; i += 80) {
		QPoint u(20, i);
		QPoint v(580, i);
		painter.drawLine(u, v);
	}
	for (int i = 20; i < 600; i += 80) {
		QPoint u(i, 20);
		QPoint v(i, 580);
		painter.drawLine(u, v);
	}

	// Draw chess pieces
	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			if (this->checkBoard[i][j]) {
				brush.setColor(checkBoard[i][j] == -1 ? Qt::white : Qt::black);
				painter.setBrush(brush);
				painter.drawEllipse(25 + 80 * i, 25 + 80 * j, 70, 70);
			}
}

void MainWindow::mousePressEvent(QMouseEvent *mouseEvent) {
	int x = float(mouseEvent->x());
	int y = float(mouseEvent->y());
	if (x < 20 || x > 580 || y < 20 || y > 580 || (x - 20) % 80 == 0 || (y - 20) % 80 == 0)
		return ;
	x = (x - 20) / 80, y = (y - 20) / 80;



	int stand = (this->stepCounts & 1) ? -1 : 1;
	if (first) {
		int dist = std::max(abs(x - xPressed), abs(y - yPressed));
		if (this->checkBoard[x][y] || dist > 2) {
			first = false;
			return ;
		}

		if (dist == 2)
			this->checkBoard[xPressed][yPressed] = 0;
		else
			this->numCounts++;
		this->checkBoard[x][y] = stand;
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				if (x + i >= 0 && x + i < 7 && y + j >= 0 && y + j < 7 && this->checkBoard[x + i][y + j])
					this->checkBoard[x + i][y + j] = stand;
		this->update();

		this->stepCounts++;
		first = false;
		if (this->numCounts == 49)
			checkWin();
	}
	else {
		if (checkBoard[x][y] != stand)
			return ;
		xPressed = x;
		yPressed = y;
		first = true;
	}

}

void MainWindow::checkWin() {
	int counts = 0;
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			counts += this->checkBoard[i][j];
	winShow(counts < 0 ? -1 : 1);
}

void MainWindow::winShow(int stand) {
	QString info;
	info = stand == 1 ? "Black Win!" : "White Win!";
	if (QMessageBox::information(NULL, "Game Over!", info, "Again", "Exit"))
		close();
	clear();
}

void MainWindow::clear() {
	this->numCounts = 4;
	this->stepCounts = 0;
	this->first = false;
	memset(checkBoard, 0, sizeof checkBoard);
	checkBoard[0][0] = checkBoard[6][6] = 1;
	checkBoard[0][6] = checkBoard[6][0] = -1;
}


