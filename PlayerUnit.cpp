#pragma once
#include "Character.cpp"
#include "InputKey.cpp"

const int shotlimit = 3;
const int weaponkind = 3;
class Player :public Character {
private:
	InputKey input;
	int playergraph;
	int reloadSE;
	int shotgraph[3];
	int shotSE[2];

	int weapon;
	int inertiaX = 0, inertiaY = 0;
	int inertiaY_time = 0, inertiaX_time = 0;
	int shotcount = 0;
	Shot shots[shotlimit];
	bool laserwait = false;
public:
	Player(int graph, int _shotgraph[], int kind, int _reloadSE, int _shotSE[],int kind2) {
		if (graph != -1) {
			GetGraphSize(graph, &sizeX, &sizeY);
			sizeX /= 2;
			sizeY /= 2;
			playergraph = graph;
		}
		else {
			sizeX = sizeY = 40 / 2;
		}
		for (int i = 0; i < kind; i++) {
			shotgraph[i] = _shotgraph[i];
		}
		reloadSE = _reloadSE;
		for (int i = 0; i < kind2; i++) {
			shotSE[i] = _shotSE[i];
		}
		X = SIZE_X * 2 / 4;
		Y = SIZE_Y * 5 / 6;
		weapon = 0;
		movespeed = 5;
		HP = 5;
		attack = 1;
	}
	void Move() {
		if (!laserwait) {
			if (input.PushOneframe_WeaponChange()) {
				weapon++;
				weapon %= weaponkind;//•Ší‚Ì”
				PlaySoundMem(reloadSE, DX_PLAYTYPE_BACK, TRUE);
			}
			if (weapon == 2) {
				attack = 3;
			}
			else {
				attack = 1;
			}
			if (input.Push_KeyUP()) {
				inertiaY = -movespeed;
				Y -= movespeed;
			}
			if (input.Push_KeyDOWN()) {
				inertiaY = movespeed;
				Y += movespeed;
			}
			if (input.Push_KeyRIGHT()) {
				inertiaX = movespeed;
				X += movespeed;
			}
			if (input.Push_KeyLEFT()) {
				inertiaX = -movespeed;
				X -= movespeed;
			}
		}
		Inertia(3);
		if (Y < 0 + sizeY) {//‰æ–Êã
			Y = 0 + sizeY;
		}
		else if (SIZE_Y - sizeY < Y) {//‰æ–Ê‰º
			Y = SIZE_Y - sizeY;
		}
		Draw(playergraph);
	}
	void Inertia(int fpsspeed) {//‰½ƒtƒŒ[ƒ€‚Å‘¬“x‚ð—Ž‚Æ‚·‚©
		if (input.Push_KeyUP() && input.Push_KeyDOWN() || laserwait) {
			if (inertiaY != 0) {
				Y += inertiaY;
				inertiaY_time += 1;
				if (inertiaY_time >= fpsspeed) {
					inertiaY_time = 0;
					if (inertiaY < 0) {
						inertiaY += 1;
						if (inertiaY > 0) {
							inertiaY = 0;
						}
					}
					else {
						inertiaY -= 1;
						if (inertiaY < 0) {
							inertiaY = 0;
						}
					}
				}
			}
		}
		if (input.Push_KeyRIGHT() && input.Push_KeyLEFT() || laserwait) {
			if (inertiaX != 0) {
				X += inertiaX;
				inertiaX_time += 1;
				if (inertiaX_time >= fpsspeed) {
					inertiaX_time = 0;
					if (inertiaX < 0) {
						inertiaX += 1;
						if (inertiaX > 0) {
							inertiaX = 0;
						}
					}
					else {
						inertiaX -= 1;
						if (inertiaX < 0) {
							inertiaX = 0;
						}
					}
				}
			}
		}
	}
	void Shoot() {
		if (input.PushOneframe_Attack()) {
			if (shotcount < shotlimit) {
				if (weapon == 0 || weapon == 1) {
					PlaySoundMem(shotSE[0], DX_PLAYTYPE_BACK, TRUE);
					shots[shotcount].Set(shotgraph[0], X, Y, weapon);
					shotcount++;
				}
				else if (weapon == 2) {
					if (shotcount == 0) {
						laserwait = true;
					}
				}
			}
		}
		if (laserwait) {
			time++;
			if (time >= 20) {
				laserwait = false;
				time = 0;
				PlaySoundMem(shotSE[1], DX_PLAYTYPE_BACK, TRUE);
				shots[shotcount].Set(shotgraph[0], X, Y, weapon);
				shotcount++;
			}
		}
		for (int i = 0; i < shotcount; i++) {
			if (shots[i].Getshot()) {
				if (!shots[i].PlayerMove()) {
					Compression(i);
					shotcount--;
				}
			}
		}
	}
	bool HitCheck(Unit e) {
		for (int i = 0; i < shotcount; i++) {
			if (shots[i].Getshot()) {
				if (shots[i].Hit(e, TRUE)) {
					Compression(i);
					shotcount--;
					return true;
				}
			}
		}
		return false;
	}
	void Compression(int i)
	{
		if (i + 1 < shotlimit)
		{
			if (shots[i + 1].Getshot()) {
				shots[i] = shots[i + 1];
				shots[i + 1].Setshot(FALSE);
				Compression(i + 1);
			}
		}
	}
	Shot* Getshots() {
		return shots;
	}
	int Getshotcount() {
		return shotcount;
	}
	int Getweapon() {
		return weapon;
	}
	void Setweapon(int type) {//0,1,2
		weapon = type;
	}
};