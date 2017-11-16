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
	id = éOäpä÷êîãOìπÅAtype = ãOìπ
	if (!SetGraph(_graph)) sizeX = sizeY = 20 / 2;
		
		X = posX = x;
		Y = posY = y;
		player = p;
		id = _id; type = _type; type_2 = _type_2;
		shot = true; time = 0;
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
		/*ëÂícâ~*/
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
		/*éOï˚å¸ÉzÅ[É~ÉìÉO*/
		else if (type == 3) {
			speed = 20.0;
			double y = (double)(player.GetY() - posY), x = 0.0;
			/*ç∂*/
			if (id % 3 == 0) {
				x = (double)(-SIZE_X - (posX - player.GetX()));
			}
			/*íÜâõ*/
			else if (id % 3 == 1) {
				x = (double)(player.GetX() - posX);
			}
			/*âE*/
			else if (id % 3 == 2){
				x = (double)(SIZE_X + (player.GetX() - posX));
			}
			double theta = atan2(y, x);
			X = (double)posX + (double)time * speed * cos(theta);
			Y = (double)posY + (double)time * speed * sin(theta);
			//printfDx("%d, %d \n", X, Y);
		}
		/*å≈íËâÒì]*/
		else if (type == 4) {
			speed = 4.0;
			double theta = (double)((id % 6) * 60.0 + (double)type_2) * (double)(PI) / 180.0;
			X = (double)posX +
				(double)time * speed * cos(theta);
			Y = (double)posY +
				(double)time * speed * sin(theta);
		}
		/*å≈íËâQä™Ç´*/
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
	/*ìñÇΩÇËîªíË*/
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

const int maxShot = 1000;
class TohoEnemy :public Character {
private:
	Graph shotgraph[7];
	int shotSE[2];
	int type;
	TohoShot shot[maxShot];
	int period = 0;
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
		/*if (type == 1) {
			Setstatus(type, 1);
		}
		else if (type == 2) {
			Setstatus(type, 1);
		}
		else if (type == 3 || type == 4) {
			Setstatus(type, 2);
		}
		else if (type == 5) {
			Setstatus(type, 2);
		}*/
		X = x; Y = y;
		movespeed = speed;
		type = ty;
	}
	void SetshotSE(int SE[], int count) {
		for (int i = 0; i < count; i++) {
			shotSE[i] = SE[i];
		}
		ChangeVolumeSoundMem(255 * (0.5), shotSE[0]);
	}
	/*ìÆÇ´ä÷åW*/
	bool Move(Unit p) {
		/*íeÇÃòAë±éÀèoãKêßÅAíeÇÃä‘äuí≤êÆ*/
		if (time > 0) time--;
		/*ê∂Ç´ÇƒÇ¢ÇÈéû,åÇÇƒÇÈèÛë‘Ç»ÇÁ*/
		if (HP > 0 && time <= 0) {
			if (type == 1) {
				//X += 3;
				int count = 2;
				if (shotcount - count < maxShot) {
					for (int i = 0; i < count; i++) {
						period = (period + 1) % 36;
						shot[shotcount].Set(shotgraph[i], type, X, Y, p, period);
						shot[shotcount].AddSetType2(i);
						shotcount++;
					}
					PlaySoundMem(shotSE[0], DX_PLAYTYPE_BACK, TRUE);
					time += 2;
				}
			}
			else if (type == 2) {
				int count = 20;
				if (shotcount - count < maxShot) {
					for (int i = 0; i < count; i++) {
						period = (period + 1) % 36;
						shot[shotcount].Set(shotgraph[3], type, X, Y, p, period);
						shotcount++;
					}
					PlaySoundMem(shotSE[0], DX_PLAYTYPE_BACK, TRUE);
					time += 20;
				}
			}
			else if (type == 3) {
				int count = 3;
				if (shotcount - count < maxShot) {
					for (int i = 0; i < count; i++) {
						shot[shotcount].Set(shotgraph[4], type, X, Y, p, shotcount);
						shotcount++;
					}
					PlaySoundMem(shotSE[0], DX_PLAYTYPE_BACK, TRUE);
					time += 2;
				}
			}
			else if (type == 4 || type == 5) {
				//X += 3;
				int count = 6;
				if (shotcount - count < maxShot) {
					for (int i = 0; i < count; i++) {
						period = (period + 1) % (360 / count);
						shot[shotcount].Set(shotgraph[5], type, X, Y, p, shotcount);
						shot[shotcount].AddSetType2(period);
						shotcount++;
					}
					PlaySoundMem(shotSE[0], DX_PLAYTYPE_BACK, TRUE);
					time += 4;
				}
			}
		}
		for (int i = 0; i < shotcount; i++) {
			if (Getshot(i)) {
				if (!shot[i].Move()) {//íeÇÃìÆÇ´Ç∆ë¨ìx
					Compression(i);
					shotcount--; 
				}
			}
		}
		if (HP > 0) {
			//Y %= SIZE_Y;
			Draw();
			DrawFormatString(0, 0, GetColor(255, 255, 255), "shotcount = %d", shotcount);
		}
		for (int i = 0; i < shotcount; i++) {
			if (shot[i].Hit(p, FALSE)) {
				Compression(i);
				shotcount--;
				return true;
			}
		}
		if (HP > 0) {
			return abs(Y - p.GetY()) <= sizeY + p.GetsizeY()
				&& abs(X - p.GetX()) <= sizeX + p.GetsizeX();//é©ã@Ç∆ÇÃê⁄êG
		}
		return false;
	}

	/*shotcount <= 10*/
	void Setstatus(int HP, int _attack) {
		SetHP(HP);
		attack = _attack;
	}

	int Getshotcount() {
		return shotcount;
	}

	bool Getshot(int i) {
		return shot[i].Getshot();
	}

	void Compression(int i)
	{
		if (i + 1 < maxShot)
		{
			if (shot[i + 1].Getshot() == TRUE) {
				shot[i] = shot[i + 1];
				shot[i + 1].Setshot(FALSE);
				Compression(i + 1);
			}
		}
	}
};
