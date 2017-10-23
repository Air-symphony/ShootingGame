#pragma once
#include "Display.cpp"
#include "Graphics.cpp"
#include <math.h>
#define PI 3.14159

class Unit :public Graph{
public:
	int X, Y;//中心
	int time = 0;

	/*画像描画*/
	void Draw() {
		X = (X + SIZE_X) % SIZE_X;//画面端の反転
		if (graphID != -1) {
			Draw_Graph(X, Y, 5);
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
	int Gettime() {
		return time;
	}
};