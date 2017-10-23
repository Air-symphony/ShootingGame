#pragma once
#include "Character.cpp"

const int max = 10;
class Enemy :public Character {
private:
	Graph shotgraph[3];
	int type;
	Shot shot[max];
	int maxshot = 0;
public:
	void Set(Graph _enemygraph, Graph _shotgraph[], int kind, int ty, int x, int y, int speed) {
		type = ty;
		if (!SetGraph(_enemygraph)) {
			sizeX = sizeY = 40 / 2;
		}
		for (int i = 0; i < kind; i++) {
			shotgraph[i].SetGraph(_shotgraph[i]);
		}
		HP = type;
		if (type == 1) {
			Setstatus(type, 1, 2);
		}
		else if (type == 2) {
			Setstatus(type, 1, 6);
		}
		else if (type == 3 || type == 4) {
			Setstatus(type, 2, 5);
		}
		else if (type == 5) {
			Setstatus(type, 2, 10);
		}
		X = x;
		Y = y;
		movespeed = speed;
	}
	/*�����֌W*/
	bool Move(Unit p) {
		/*�e�̘A���ˏo�K��*/
		if (time > 0) time--;
		/*�����Ă��鎞*/
		if (HP > 0) {
			/*���Ă��ԂȂ�*/
			if (time <= 0 && shotcount < maxshot) {
				if (!shot[shotcount].Getshot()) {
					if (type == 1 || type == 3) {
						/*���ʂɗ�����ł�*/
						int dx = 40;//��
						if (X - sizeX - dx <= p.GetX() && p.GetX() <= X + sizeX + dx && p.GetY() > Y) {//�ǂ̃^�C�~���O�őł�
							shot[shotcount].Set(shotgraph[1], X, Y, type);
							shotcount++;
							time = 20 + (5 - type) * 10;
						}
					}
					else if (type == 4 || type == 5)
					{
						/*���ʂɗ�����ł�*/
						int dx = 150 * (type - 3);//��
						if (X - sizeX - dx <= p.GetX() && p.GetX() <= X + sizeX + dx && p.GetY() > Y) {//�ǂ̃^�C�~���O�őł�
							shot[shotcount].Set(shotgraph[1], X, Y, type);
							shotcount++;
							time = 10 + (5 - type) * 10;
						}
					}
					else if (type == 2) {
						shot[shotcount].Set(shotgraph[2], X, Y, type);
						shotcount++;
						time = 40;
					}
				}
			}
			if (type == 4 || type == 5) {
				X += movespeed;
				X %= SIZE_X;
			}
			Y %= SIZE_Y;
			Draw();
		}
		bool hit[max] = { false };
		for (int i = 0; i < maxshot; i++) {
			if (shot[i].Getshot()) {
				if (!shot[i].EnemyMove(p)) {//�e�̓����Ƒ��x
					shotcount--;
				}
				hit[i] = shot[i].Hit(p, FALSE);
			}
		}
		for (int i = 0; i < maxshot; i++) {
			if (hit[i]) {
				shotcount--;
				time += 20;
				return true;
			}
		}
		if (HP > 0) {
			return abs(Y - p.GetY()) <= sizeY + p.GetsizeY()
				&& abs(X - p.GetX()) <= sizeX + p.GetsizeX();//���@�Ƃ̐ڐG
		}
		return false;
	}

	/*shotcount <= 10*/
	void Setstatus(int HP, int _attack, int shotcount) {
		SetHP(HP);
		attack = _attack;
		maxshot = shotcount;
	}
	
	int Getshotcount() {
		return shotcount;
	}
	
	bool Getshot(int i) {
		return shot[i].Getshot();
	}
};