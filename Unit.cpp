#pragma once
#include "Display.cpp"
#include "Dxlib.h"
#include <math.h>
#define PI 3.14159

class Unit {
public:
	int graph;//画像ハンドラ
	int X, Y;//中心
	int sizeX = 0, sizeY = 0;//中央からの画像サイズ
	int time = 0;

	/*画像描画*/
	void Draw() {
		X = (X + SIZE_X) % SIZE_X;//画面端の反転
		if (graph != -1) {
			DrawGraph(X - sizeX,
				Y - sizeY, graph, TRUE);//左上
		}
		else {
			Draw_GreenBox();
			if (0 >= X - sizeX) {//左端
				Draw_GreenBox(X + SIZE_X, Y);
			}
			else if (X + sizeX >= SIZE_X) {//右端
				Draw_GreenBox(X - SIZE_X, Y);
			}
		}
	}
	void Draw_RedBox() {
		DrawBox(X - sizeX, Y - sizeY,//左上
			X + sizeX, Y + sizeY,//右下
			GetColor(255, 0, 0), TRUE);
	}
	void Draw_GreenBox() {
		DrawBox(X - sizeX, Y - sizeY,//左上
			X + sizeX, Y + sizeY,//右下
			GetColor(0, 255, 0), TRUE);
	}
	void Draw_BlueBox() {
		DrawBox(X - sizeX, Y - sizeY,//左上
			X + sizeX, Y + sizeY,//右下
			GetColor(0, 0, 255), TRUE);
	}
	void Draw_GreenBox(int x, int y) {
		DrawBox(x - sizeX, y - sizeY,//左上
			x + sizeX, y + sizeY,//右下
			GetColor(0, 255, 0), TRUE);
	}
	int GetX() {
		return X;
	}
	int GetY() {
		return Y;
	}
	int GetsizeX() {
		return sizeX;
	}
	int GetsizeY() {
		return sizeY;
	}
	int Gettime() {
		return time;
	}
};