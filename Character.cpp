#pragma once
#include "Shot.cpp"

class Character :public Unit {
public:
	int graph;
	int damageSE[2];
	int HP = 0;
	int attack = 0;
	int movespeed = 0;//“®‚«
	int time = 0;

	void DrawGraph() {
		Draw(graph);
	}
	void SetDamageSE(int _SE[], int kind) {
		for (int i = 0; i < kind; i++) {
			damageSE[i] = _SE[i];
		}
	}
	void Damege(int i) {
		HP -= i;
		if (HP > 0) {
			PlaySoundMem(damageSE[0], DX_PLAYTYPE_BACK, TRUE);
		}
		else {
			PlaySoundMem(damageSE[1], DX_PLAYTYPE_BACK, TRUE);
		}
		Draw_RedBox();
	}
	
	void SetHP(int hp) {
		HP = hp;
	}
	int GetHP() {
		return HP;
	}
	int Getattack() {
		return attack;
	}
	int Gettime() {
		return time;
	}
};