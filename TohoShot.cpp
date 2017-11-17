#pragma once
#include <math.h>
#include "Character.cpp"

class TohoShot :public Unit {
private:
	/*�����e�̏����ʒu*/
	int posX, posY;
	bool shot = false;
	Unit player;
	int ShotType;
	int id, id_2;

	double speed = 3.0;
public:
	/**/
	void SetInit(Graph _graph, int x, int y, Unit p) {
		if (!SetGraph(_graph)) sizeX = sizeY = 20 / 2;
		X = posX = x;
		Y = posY = y;
		player = p;
		time = id = id_2 = 0; 
		shot = true;
	}
	/*type : 
		1, 2�����t��]����(�S����)
		2, ����͈͂̕�͌���(�S����)
		3, 3�����_������(���A�����A�E)
		4, 6��������]����(�S����)
		5, �Œ�Q����
	speed : �e�̑��x(�����l = 3.0)
	id : ��������
	id_2 : ��������2*/
	void SetShotType(int _type, double _speed, int _id, int _id2 = 0) {
		ShotType = _type;
		speed = _speed;
		id = _id; id_2 = _id2;
	}
	
	bool Move() {
		if (shot == false) 	return false;

		time++;
		/*��c�~�A36����*/
		if (ShotType == 1 || ShotType == 2) {
			double theta;
			/*�E�n�_�A���v���*/
			if (id_2 == 0) {
				theta = radian((double)(id * (360.0 / 36.0)));
			}
			/*���n�_�A�����v���*/
			else if (id_2 == 1) {
				theta = radian((180.0 - (double)(id * (360.0 / 36.0))));
			}
			X = (int)((double)posX + (double)time * speed * cos(theta));
			Y = (int)((double)posY + (double)time * speed * sin(theta));
		}
		/*�O�����z�[�~���O*/
		else if (ShotType == 3) {
			double y = (double)(player.GetY() - posY), x = 0.0;
			/*���A��ʒ[�o�R*/
			if (id % 3 == 0) {
				x = (double)(-SIZE_X - (posX - player.GetX()));
			}
			/*����*/
			else if (id % 3 == 1) {
				x = (double)(player.GetX() - posX);
			}
			/*�E�A��ʒ[�o�R*/
			else if (id % 3 == 2) {
				x = (double)(SIZE_X + (player.GetX() - posX));
			}
			double theta = atan2(y, x);
			X = (int)((double)posX + (double)time * speed * cos(theta));
			Y = (int)((double)posY + (double)time * speed * sin(theta));
		}
		/*�Œ��]�A6����*/
		else if (ShotType == 4) {
			double theta = radian((double)((id % 6) * (360.0 / 6.0) + (double)id_2));
			X = (int)((double)posX + (double)time * speed * cos(theta));
			Y = (int)((double)posY + (double)time * speed * sin(theta));
		}
		/*�Œ�Q����*/
		else if (ShotType == 5) {
			double theta = radian((double)((id % 6) * (360.0 / 6.0) + time * 10.0));
			X = (int)(
				(double)posX + (double)time * speed * cos(theta) +
				(double)time * speed * cos(radian((double)(time * 1.0)))
				);
			Y = (int)(
				(double)posY + (double)time * speed * sin(theta) +
				(double)time * speed * sin(radian((double)(time * 1.0)))
				);
		}

		shot = (-GetsizeY() <= Y && Y <= SIZE_Y + GetsizeY()) &&
			//(-GetsizeX() < X && X < SIZE_X + GetsizeX()) &&
			(time <= 1500);
		if (shot == true) Draw();

		return shot;
	}
	/*�����蔻��*/
	bool Hit(Unit e) {
		/*�l�p�`*/
		if (abs(Y - e.GetY()) <= sizeY + e.GetsizeY()) {
			if (abs(X - e.GetX()) <= sizeX + e.GetsizeX()) {
				shot = false;
				return true;
			}
		}
		return false;
	}
	/*bool*/
	void Setshot(bool _shot) {
		shot = _shot;
	}
	bool Getshot() {
		return shot;
	}
	int GetposX() {
		return posX;
	}
	int GetposY() {
		return posY;
	}
	/*return degree * (double)(PI) / 180.0;*/
	double radian(double degree) {
		return degree * (double)(PI) / 180.0;
	}
};