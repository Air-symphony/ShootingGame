#pragma once
#include <math.h>
#include "Character.cpp"

class TohoShot :public Unit {
private:
	int posX, posY;
	bool shot = false;
	Unit player;
	int id;
	int type, type_2;
public:
	/*
	type :
	1, 2•ûŒü‹t‰ñ“]Œ‚‚¿(‘S•ûˆÊ)
	2, “Á’è”ÍˆÍ‚Ì•ïˆÍŒ‚‚¿(‘S•ûˆÊ)
	3, 3•ûŒü‘_‚¢Œ‚‚¿(¶A’†‰›A‰E)
	4, 6•ûŒüƒ‰ñ“]Œ‚‚¿(‘S•ûˆÊ)
	5, ŒÅ’è‰QŠª‚«
	*/
	void Set(Graph _graph, int _type, int x, int y, Unit p, int _id) {
		if (!SetGraph(_graph)) sizeX = sizeY = 20 / 2;

		X = posX = x;
		Y = posY = y;
		player = p;
		id = _id; type = _type;
		type_2 = 0; shot = true; time = 0;
	}
	/*type = 0,1*/
	void AddSetType2(int _type = 0) {
		type_2 = _type;
	}
	bool Move() {
		if (shot == false) {
			return false;
		}
		time++;
		double speed = 3.0;
		/*‘å’c‰~*/
		if (type == 1 || type == 2) {
			double theta;
			if (type_2 == 0) {
				theta = (double)(id * 10.0) * (double)(PI) / 180.0;
			}
			else if (type_2 == 1) {
				theta = (180.0 - (double)(id * 10.0)) * (double)(PI) / 180.0;
			}
			X = (double)posX + (double)time * speed * cos(theta);
			Y = (double)posY + (double)time * speed * sin(theta);
		}
		/*O•ûŒüƒz[ƒ~ƒ“ƒO*/
		else if (type == 3) {
			speed = 20.0;
			double y = (double)(player.GetY() - posY), x = 0.0;
			/*¶*/
			if (id % 3 == 0) {
				x = (double)(-SIZE_X - (posX - player.GetX()));
			}
			/*’†‰›*/
			else if (id % 3 == 1) {
				x = (double)(player.GetX() - posX);
			}
			/*‰E*/
			else if (id % 3 == 2) {
				x = (double)(SIZE_X + (player.GetX() - posX));
			}
			double theta = atan2(y, x);
			X = (double)posX + (double)time * speed * cos(theta);
			Y = (double)posY + (double)time * speed * sin(theta);
			//printfDx("%d, %d \n", X, Y);
		}
		/*ŒÅ’è‰ñ“]*/
		else if (type == 4) {
			speed = 4.0;
			double theta = (double)((id % 6) * 60.0 + (double)type_2) * (double)(PI) / 180.0;
			X = (double)posX +
				(double)time * speed * cos(theta);
			Y = (double)posY +
				(double)time * speed * sin(theta);
		}
		/*ŒÅ’è‰QŠª‚«*/
		else if (type == 5) {
			speed = 4.0;
			double theta = (double)((id % 6) * 60.0 + time * 10.0) * (double)(PI) / 180.0;
			X = (double)posX +
				(double)time * speed * cos(theta) +
				(double)time * speed * cos((double)(time * 1.0) * (double)(PI) / 180.0);
			Y = (double)posY +
				(double)time * speed * sin(theta) +
				(double)time * speed * sin((double)(time * 1.0) * (double)(PI) / 180.0);
		}

		shot = (-GetsizeY() <= Y && Y <= SIZE_Y + GetsizeY()) &&
			//(-GetsizeX() < X && X < SIZE_X + GetsizeX()) &&
			(time <= 1500);
		if (shot == true) Draw();

		return shot;
	}
	/*“–‚½‚è”»’è*/
	bool Hit(Unit e, bool isPlayer) {
		if (isPlayer && type == 2) {
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
	int GetposX() {
		return posX;
	}
	int GetposY() {
		return posY;
	}
};