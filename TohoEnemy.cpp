#pragma once
#include "SupportEnemy.cpp"

const int maxShot = 1000;
class TohoEnemy :public Character {
private:
	Graph shotgraph[7];
	int shotSE[2];
	int type;
	TohoShot shot[maxShot];
	int period = 0;

	SupportEnemy enemy[5];
	int enemycount, supportTime = 30000;
	
public:
	void Set(Graph _enemygraph, Graph _shotgraph[], int kind, int ty, int x, int y, int speed, Graph Supportenemygraph) {
		type = ty;
		if (!SetGraph(_enemygraph)) {
			sizeX = sizeY = 40 / 2;
		}
		for (int i = 0; i < kind; i++) {
			shotgraph[i].SetGraph(_shotgraph[i]);
		}
		HP = type + 20;
		X = x; Y = y;
		if (type == 1) {
			enemycount = 2;
			for (int i = 0; i < enemycount; i++) {
				enemy[i].SetSupportEnemy(Supportenemygraph, X, Y, i + 1, supportTime);
				enemy[i].Setshotgraph(shotgraph[2]);
			}
			//Setstatus(type, 1);
		}
		else if (type == 2) {
			//Setstatus(type, 1);
		}
		else if (type == 3 || type == 4) {
			enemycount = 2;
			for (int i = 0; i < enemycount; i++) {
				enemy[i].SetSupportEnemy(Supportenemygraph, X, Y, i + 1, supportTime);
				enemy[i].Setshotgraph(shotgraph[2]);
			}
			//Setstatus(type, 2);
		}
		else if (type == 5) {
			//Setstatus(type, 2);
		}
		movespeed = speed;
		type = ty;
	}
	void SetshotSE(int SE[], int count) {
		for (int i = 0; i < count; i++) {
			shotSE[i] = SE[i];
		}
		for (int i = 0; i < enemycount; i++) {
			enemy[i].SetshotSE(shotSE, count);
		}
		ChangeVolumeSoundMem(255 * (0.5), shotSE[0]);
	}
	/*“®‚«ŠÖŒW*/
	bool Move(Unit p) {
		/*’e‚Ì˜A‘±ŽËo‹K§A’e‚ÌŠÔŠu’²®*/
		if (time > 0) time--;
		/*¶‚«‚Ä‚¢‚éŽž,Œ‚‚Ä‚éó‘Ô‚È‚ç*/
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
				X += 5;
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
				if (!shot[i].Move()) {//’e‚Ì“®‚«‚Æ‘¬“x
					Compression(i);
					shotcount--; 
				}
			}
		}
		if (HP > 0) {
			//Y %= SIZE_Y;
			Draw();
			//DrawFormatString(0, 0, GetColor(255, 255, 255), "shotcount = %d", shotcount);
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
				&& abs(X - p.GetX()) <= sizeX + p.GetsizeX();//Ž©‹@‚Æ‚ÌÚG
		}
		return false;
	}

	bool SupportEnemyMove(Unit boss, Unit player) {
		bool hit = false;
		printfDx("SupportEnemyMove\n");
		printfDx("%d\n\n", enemycount);

		for (int i = 0; i < enemycount; i++) {
			if (enemy[i].GetShow() == false) {
				CompressionEnemy(i);
				enemycount--;
				continue;
			}
			if (enemy[i].Move(boss, player)) {
				hit = true;
			}
		}
		return hit;
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

	void CompressionEnemy(int i)
	{
		if (i + 1 < 5)
		{
			if (enemy[i + 1].GetShow() == FALSE) {
				enemy[i] = enemy[i + 1];
				enemy[i].SetShow(FALSE);
				CompressionEnemy(i + 1);
			}
		}
	}
};