#pragma once
#include "Unit.cpp"

class Shot :public Unit {
private:
	bool shot = false;
	bool hit;
	int graphID;
	int type;
	int time = 0;
	int keepY = 0;
public:
	void Set(int graph, int x, int y, int t) {
		if (graph != -1) {
			graphID = graph;
			GetGraphSize(graph, &sizeX, &sizeY);
			sizeX /= 2;
			sizeY /= 2;
		}
		else {
			sizeX = sizeY = 20 / 2;
		}
		X = x;
		Y = y;
		type = t;
		shot = true;
		keepY = Y;
	}
	void Set(int graph, int x, int y, int ty, int ti) {
		if (graph != -1) {
			graphID = graph;
			GetGraphSize(graph, &sizeX, &sizeY);
			sizeX /= 2;
			sizeY /= 2;
		}
		else {
			sizeX = sizeY = 20 / 2;
		}
		X = x;
		Y = y;
		type = ty;
		shot = true;
		time = ti;
	}
	bool PlayerMove() {
		if (shot) {
			Draw(graphID);
			/*直進*/
			if (type == 0) {
				Y -= 15;
			}
			/*波形*/
			else if (type == 1) {// || (type == 2) {
				time += 2;
				Y -= 7;
				X += (int)(cos(PI / 90 * time) * 5);
			}
			/*レーザー*/
			else if (type == 2) {
				DrawBox(X - 2, Y, X + 2, 0, GetColor(255, 0, 0), TRUE);
				Y -= (keepY - 1);
			}
		}
		shot = Y > 0;
		return shot;
	}
	/*移動方法*/
	bool EnemyMove(Unit p) {
		if (shot) {
			Draw(graphID);
			if (type == 1) {
				Y += 7;
			}
			else if (type == 2) {
				time += 2;
				Y += 4;
				X += (int)(cos(PI / 90 * time) * 4);
			}
			else if (type == 3) {
				Y += 14;
			}
			else if (type == 4) {
				time += 4;
				Y += 7;
				X += (int)(cos(PI / 45 * time) * 4);
			}
			else if (type == 5) {
				time = (time + 1) % 2;//何フレーム毎に
				Y += 3;
				if (time == 0) {
					int dx = 5;
					if (abs(p.GetX() - X) < SIZE_X / 2) {
						if (X - p.GetX() > 0) {
							dx = -dx;
						}
					}
					else {
						if (X - p.GetX() < 0) {
							dx = -dx;
						}
					}
					X += dx;
				}
			}
		}
		shot = Y < SIZE_Y;
		return shot;
	}
	bool Hit(Unit e, bool player) {
		if (player && type == 2) {
			if (e.GetX() - e.GetsizeX() <= X && X <= e.GetX() + e.GetsizeX()) {
				shot = false;
				return true;
			}
		}
		else {
			if (abs(Y - e.GetY()) <= sizeY + e.GetsizeY()) {
				if (abs(X - e.GetX()) <= sizeX + e.GetsizeX()) {
					shot = false;
					return true;
				}
			}
		}
		return false;
	}
	void Setshot(bool s) {
		shot = s;
	}
	bool Getshot() {
		return shot;
	}
};