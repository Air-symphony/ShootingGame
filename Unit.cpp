#pragma once
#include "Display.cpp"
#include "Graphics.cpp"
#include <math.h>
#define PI 3.14159

class Unit :public Graph{
public:
	int X, Y;//���S
	int time = 0;

	/*�摜�`��*/
	void Draw() {
		X = (X + SIZE_X) % SIZE_X;//��ʒ[�̔��]
		if (graphID != -1) {
			Draw_Graph(X, Y, 5);
		}
		else {
			Draw_GreenBox();
			if (0 >= X - sizeX) {//���[
				Draw_GreenBox(X + SIZE_X, Y);
			}
			else if (X + sizeX >= SIZE_X) {//�E�[
				Draw_GreenBox(X - SIZE_X, Y);
			}
		}
	}
	void Draw_RedBox() {
		DrawBox(X - sizeX, Y - sizeY,//����
			X + sizeX, Y + sizeY,//�E��
			GetColor(255, 0, 0), TRUE);
	}
	void Draw_GreenBox() {
		DrawBox(X - sizeX, Y - sizeY,//����
			X + sizeX, Y + sizeY,//�E��
			GetColor(0, 255, 0), TRUE);
	}
	void Draw_BlueBox() {
		DrawBox(X - sizeX, Y - sizeY,//����
			X + sizeX, Y + sizeY,//�E��
			GetColor(0, 0, 255), TRUE);
	}
	void Draw_GreenBox(int x, int y) {
		DrawBox(x - sizeX, y - sizeY,//����
			x + sizeX, y + sizeY,//�E��
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