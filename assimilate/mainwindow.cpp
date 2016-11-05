#include "mainwindow.h"
#include <cstring>
#include <cmath>
#include <algorithm>
#include <string>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent){
	xPressed = 0;
	yPressed = 0;
	numCounts = 4;
	turn = false;
	first = 0;
	memset(checkBoard, 0, sizeof checkBoard);
	checkBoard[0][0] = checkBoard[6][6] = 1;
	checkBoard[0][6] = checkBoard[6][0] = -1;
	setMinimumSize(600, 900);
	setMaximumSize(600, 900);;

	//Font
	font = QFont("MONACO", 20);


	//New Buttom
	newGame = new QPushButton("New Game", this);
	newGame->setGeometry(50, 750, 225, 100);
	newGame->setFont(font);

	connect(newGame, SIGNAL(clicked()), this, SLOT(clear()));
}

MainWindow::~MainWindow() {
}


void MainWindow::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	QPen Pen;
	Pen.setColor(Qt::black);
	painter.setPen(Pen);
	painter.setFont(font);

	//Draw welcome info
	QRectF welcome(0, 0, 600, 40); //left, top, width, height
	painter.drawText(welcome, Qt::AlignHCenter, "Welcome to play Ataxx!");

	// Draw checkboard
	for (int i = 120; i < 700; i += 80) {
		QPoint u(20, i);
		QPoint v(580, i);
		painter.drawLine(u, v);
	}
	for (int i = 20; i < 600; i += 80) {
		QPoint u(i, 120);
		QPoint v(i, 680);
		painter.drawLine(u, v);
	}

	// Draw chess pieces
	QBrush brush;
	brush.setStyle(Qt::SolidPattern);

	int yellowCount = 0, redCount= 0;
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			if (checkBoard[i][j]) {
				checkBoard[i][j] == 1 ? redCount++ : yellowCount++;
				brush.setColor(checkBoard[i][j] == -1 ? Qt::yellow : Qt::red);
				painter.setBrush(brush);
				painter.drawEllipse(25 + 80 * i, 125 + 80 * j, 70, 70);
			}

	// Draw status info
	QFont statusFont("MONACO", 16);
	painter.setFont(statusFont);

	char str[30] = "Red Counts: ";
	int len = strlen(str);
	str[len] = redCount / 10 + '0', str[len + 1] = redCount % 10 + '0';
	painter.drawText(QRectF(0, 40, 300, 30), Qt::AlignHCenter, str);

	char str1[30] = "Yellow Counts: ";
	len = strlen(str1);
	str1[len] = yellowCount / 10 + '0', str1[len + 1] = yellowCount % 10 + '0';
	painter.drawText(QRectF(300, 40, 300, 30), Qt::AlignHCenter, str1);

	// Draw turn
	char str2[30] = "Red Turn", str3[30] = "Yellow Turn";
	painter.drawText(QRectF(0, 70, 600, 30), Qt::AlignHCenter, turn ? str3 : str2);






}

void MainWindow::mousePressEvent(QMouseEvent *mouseEvent) {
	int x = float(mouseEvent->x());
	int y = float(mouseEvent->y());
	if (x < 20 || x > 580 || y < 120 || y > 680 || (x - 20) % 80 == 0 || (y - 120) % 80 == 0)
		return ;
	x = (x - 20) / 80, y = (y - 120) / 80;



	int stand = turn ? -1 : 1;
	if (first) {
		int dist = std::max(abs(x - xPressed), abs(y - yPressed));
		if (checkBoard[x][y] || dist > 2) {
			first = false;
			QMessageBox::information(NULL, "Warning", "Invaild Action!", "confirm");
			return ;
		}

		if (dist == 2)
			checkBoard[xPressed][yPressed] = 0;
		else
			numCounts++;
		checkBoard[x][y] = stand;
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				if (x + i >= 0 && x + i < 7 && y + j >= 0 && y + j < 7 && checkBoard[x + i][y + j])
					checkBoard[x + i][y + j] = stand;
		update();

		turn ^= 1;
		first = false;
		if (numCounts == 49)
			checkWin();
	}
	else {
		if (checkBoard[x][y] != stand) {
			QMessageBox::information(NULL, "Warning", "Invaild Action!", "confirm");
			return ;
		}
		xPressed = x;
		yPressed = y;
		first = true;
	}

}

void MainWindow::checkWin() {
	int counts = 0;
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			counts += checkBoard[i][j];
	winShow(counts < 0 ? -1 : 1);
}

void MainWindow::winShow(int stand) {
	QString info;
	info = stand == 1 ? "Red Win!" : "Yellow Win!";
	QMessageBox::information(NULL, "Game Over!", info, "Again", NULL);
	clear();
}

void MainWindow::clear() {
	numCounts = 4;
	turn = false;
	first = false;
	memset(checkBoard, 0, sizeof checkBoard);
	checkBoard[0][0] = checkBoard[6][6] = 1;
	checkBoard[0][6] = checkBoard[6][0] = -1;
	update();
}


