#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "jsoncpp/json.h" // C++编译时默认包含此库
 
using namespace std;
 
int currBotColor; // 我所执子颜色（1为黑，-1为白，棋盘状态亦同）
int gridInfo[7][7] = { 0 }; // 先x后y，记录棋盘状态

int max(int a, int b) 
	{return a < b ? b : a;}
// 从x0, y0提子，在x1, y1处落子，并模拟落子。
void ProcStep(int x0, int y0, int x1, int y1, int color) {
	if (max(abs(x0 - x1), abs(y0 - y1)) > 1)
		gridInfo[x0][y0] = 0; 
	gridInfo[x1][y1] = color;
	for (int i = x1 - 1; i <= x1 + 1; i++)
		for (int j = y1 - 1; j <= y1 + 1; j++)
			if (i >= 0 && i < 7 && j >= 0 && j < 7 && gridInfo[i][j] != 0)
				gridInfo[i][j] = color;
}

void decide(int &x0, int &y0, int &x1, int &y1, int color) {
	while (true) {
		int x = rand() % 7, y = rand() % 7;
		if (gridInfo[x][y]) continue ;
		for (int i = x - 1; i <= x + 1; i++)
			for (int j = y - 1; j <= y + 1; j++)
				if (i >= 0 && i < 7 && j >= 0 && j < 7 && gridInfo[i][j] == color) 
					{x0 = i; y0 = j; x1 = x; y1 = y; return ;}
		
		for (int i = x - 2; i <= x + 2; i++)
			for (int j = y - 2; j <= y + 2; j++)
				if (i >= 0 && i < 7 && j >= 0 && j < 7 && gridInfo[i][j] == color) 
					{x0 = i; y0 = j; x1 = x; y1 = y; return ;}
	}
}

int main() {
	int x0, y0, x1, y1;
	
	srand(time(NULL));
	// 初始化棋盘
	gridInfo[0][0] = gridInfo[6][6] = 1;  //|黑|白|
	gridInfo[6][0] = gridInfo[0][6] = -1; //|白|黑|
 
	// 读入JSON
	string str;
	getline(cin, str);
	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input);
 
	// 分析自己收到的输入和自己过往的输出，并恢复状态
	int turnID = input["responses"].size();
	currBotColor = input["requests"][(Json::Value::UInt) 0]["x0"].asInt() < 0 ? 1 : -1; // 第一回合收到坐标是-1, -1，说明我是黑方
	for (int i = 0; i < turnID; i++)
	{
		// 根据这些输入输出逐渐恢复状态到当前回合
		x0 = input["requests"][i]["x0"].asInt();
		y0 = input["requests"][i]["y0"].asInt();
		x1 = input["requests"][i]["x1"].asInt();
		y1 = input["requests"][i]["y1"].asInt();
		if (x0 >= 0)
			ProcStep(x0, y0, x1, y1, -currBotColor); // 模拟对方落子
		x0 = input["responses"][i]["x0"].asInt();
		y0 = input["responses"][i]["y0"].asInt();
		x1 = input["responses"][i]["x1"].asInt();
		y1 = input["responses"][i]["y1"].asInt();
		if (x0 >= 0)
			ProcStep(x0, y0, x1, y1, currBotColor); // 模拟己方落子
	}
 
	// 看看自己本回合输入
	x0 = input["requests"][turnID]["x0"].asInt();
	y0 = input["requests"][turnID]["y0"].asInt();
	x1 = input["requests"][turnID]["x1"].asInt();
	y1 = input["requests"][turnID]["y1"].asInt();
	if (x0 >= 0)
		ProcStep(x0, y0, x1, y1, -currBotColor); // 模拟对方落子
 
	// 找出合法落子点并做出决策
 
	int startX, startY, resultX, resultY;
 
	decide(startX, startY, resultX, resultY, currBotColor); 
	// 决策结束，输出结果
 
	Json::Value ret;
	ret["response"]["x0"] = startX;
	ret["response"]["y0"] = startY;
	ret["response"]["x1"] = resultX;
	ret["response"]["y1"] = resultY;
	Json::FastWriter writer;
	cout << writer.write(ret) << endl;
	return 0;
}
