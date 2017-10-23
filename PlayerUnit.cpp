#pragma once
#include "Character.cpp"
#include "InputKey.cpp"

const int shotlimit = 3;
const int weaponkind = 3;
class Player :public Character {
private:
	InputKey input;
	int reloadSE;
	Graph shotgraph[3];
	int shotSE[2];

	int weapon;
	int inertiaX = 0, inertiaY = 0;
	int inertiaY_time = 0, inertiaX_time = 0;
	Shot shots[shotlimit];
	bool laserwait = false;
public:
	void Set(Graph _graph, Graph _shotgraph[], int kind, int _reloadSE, int _shotSE[],int kind2) {
		if(!SetGraph(_graph)){
			sizeX = sizeY = 40 / 2;
		}
		for (int i = 0; i < kind; i++) {
			shotgraph[i].SetGraph(_shotgraph[i]);
		}
		reloadSE = _reloadSE;
		for (int i = 0; i < kind2; i++) {
			shotSE[i] = _shotSE[i];
		}
		SetInit();
		weapon = 0;
		movespeed = 5;
		HP = 5;
		attack = 1;
	}
	/*�����ݒ�
	X = SIZE_X * 2 / 4;
		Y = SIZE_Y * 5 / 6;
		time = shotcount = 0;
		inertiaX = inertiaY = 0;
		inertiaY_time = inertiaX_time = 0;*/
	void SetInit() {
		X = SIZE_X * 2 / 4;
		Y = SIZE_Y * 5 / 6;
		time = shotcount = 0;
		inertiaX = inertiaY = 0;
		inertiaY_time = inertiaX_time = 0;
	}
	/*�`��֌W�A�ړ��A����A����*/
	void Move() {
		if (!laserwait) {
			/*�������*/
			if (input.PushOneframe_WeaponChange()) {
				weapon++;
				weapon %= weaponkind;//����̐�
				PlaySoundMem(reloadSE, DX_PLAYTYPE_BACK, TRUE);
			}
			/*����Η�*/
			if (weapon == 2) {
				attack = 3;
			}
			else {
				attack = 1;
			}
			/*�㉺���E�̈ړ�*/
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
		/*�����ړ�*/
		Inertia(3);
		/*��ʏ㉺����*/
		if (Y < 0 + sizeY) {//��ʏ�
			Y = 0 + sizeY;
		}
		else if (SIZE_Y - sizeY < Y) {//��ʉ�
			Y = SIZE_Y - sizeY;
		}
		Draw();
	}
	/*�����ړ�
	fpsspeed = ���t���[���ő��x�𗎂Ƃ���*/
	void Inertia(int fpsspeed) {
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
	int Getweapon() {
		return weapon;
	}
	void Setweapon(int type) {//0,1,2
		weapon = type;
	}
};