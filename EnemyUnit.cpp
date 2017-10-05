#pragma once
#include "Character.cpp"

const int max = 10;
class Enemy :public Character {
private:
	int enemygraph;
	int shotgraph[3];
	int type;
	Shot shot[max];
	int maxshot, shotcount = 0;
public:
	void Set(int _enemygraph, int _shotgraph[], int kind, int ty, int x, int y, int speed) {
		type = ty;
		if (_enemygraph != -1) {
			GetGraphSize(_enemygraph, &sizeX, &sizeY);
			sizeX /= 2;
			sizeY /= 2;
			enemygraph = _enemygraph;
		}
		else {
			sizeX = sizeY = 40 / 2;
		}
		for (int i = 0; i < kind; i++) {
			shotgraph[i] = _shotgraph[i];
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
	bool Move(Unit p) {
		if (time > 0) {
			time--;
		}
		if (HP > 0) {
			Draw(enemygraph);
			if (time <= 0 && shotcount < maxshot) {
				if (!shot[shotcount].Getshot()) {
					if (type == 1 || type == 3) {
						int dx = 20;
						if (X - sizeX - dx <= p.GetX() && p.GetX() <= X + sizeX + dx && p.GetY() > Y) {//どのタイミングで打つか
							shot[shotcount].Set(shotgraph[1], X, Y, type);
							shotcount++;
							time = 20 + (5 - type) * 10;
						}
					}
					else if (type == 4 || type == 5)
					{
						int dx = 100;
						if (X - sizeX - dx <= p.GetX() && p.GetX() <= X + sizeX + dx && p.GetY() > Y) {//どのタイミングで打つか
							shot[shotcount].Set(shotgraph[1], X, Y, type);
							shotcount++;
							time = 20 + (5 - type) * 10;
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
		}
		bool hit[max] = { false };
		for (int i = 0; i < maxshot; i++) {
			if (shot[i].Getshot()) {
				if (!shot[i].EnemyMove(p)) {//弾の動きと速度
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
				&& abs(X - p.GetX()) <= sizeX + p.GetsizeX();//自機との接触
		}
		return false;
	}
	void Setstatus(int HP, int at, int shotcount) {//shotcount <= 10
		SetHP(HP);
		attack = at;
		maxshot = shotcount;
	}
	int Getshotcount() {
		return shotcount;
	}
	bool Getshot(int i) {
		return shot[i].Getshot();
	}
};