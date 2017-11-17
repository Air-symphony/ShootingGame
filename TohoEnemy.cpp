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
	int enemycount = 0, supportTime = 30000;
	
public:
	void Set(Graph _enemygraph, int x, int y, int _type, Graph _shotgraph[], int shotgraphKind, Graph Supportenemygraph) {
		if (!SetGraph(_enemygraph)) sizeX = sizeY = 40 / 2;

		X = x; Y = y; type = _type;
		for (int i = 0; i < shotgraphKind; i++) {
			shotgraph[i].SetGraph(_shotgraph[i]);
		}

		if (type == 1) {
			enemycount = 2;
			for (int i = 0; i < enemycount; i++) {
				enemy[i].SetSupportEnemy(Supportenemygraph, X, Y, i + 1, supportTime);
				enemy[i].Setshotgraph(shotgraph[2]);
			}
			Setstatus(type + 20, 7, 0);
		}
		else if (type == 2) {
			Setstatus(type + 20, 7, 0);
		}
		else if (type == 3 || type == 4) {
			enemycount = 2;
			for (int i = 0; i < enemycount; i++) {
				enemy[i].SetSupportEnemy(Supportenemygraph, X, Y, i + 1, supportTime);
				enemy[i].Setshotgraph(shotgraph[2]);
			}
			Setstatus(type + 20, 7, 0);
		}
		else if (type == 5) {
			Setstatus(type + 20, 7, 0);
		}
	}
	/*ƒ{ƒŠƒ…[ƒ€’²®‚àŠÜ‚Þ*/
	void SetshotSE(int SE[], int count) {
		for (int i = 0; i < count; i++) {
			shotSE[i] = SE[i];
		}
		for (int i = 0; i < enemycount; i++) {
			enemy[i].SetshotSE(shotSE, count);
		}
		ChangeVolumeSoundMem((int)(255.0 * (0.5)), shotSE[0]);
	}
	/*
	“®‚«ŠÖŒW
	return Player‚Éƒ_ƒ[ƒW‚ª“ü‚é
	*/
	bool Move(Unit p) {
		/*’e‚Ì˜A‘±ŽËo‹K§A’e‚ÌŠÔŠu’²®*/
		if (time > 0) time--;
		/*¶‚«‚Ä‚¢‚éŽž,Œ‚‚Ä‚éó‘Ô‚È‚ç*/
		if (HP > 0 && time <= 0) {
			if (type == 1) {
				//X += 3;
				int count = 2;
				if (shotcount - count < maxShot) {
					period = (period + 1) % 36;
					shot[shotcount].SetInit(shotgraph[0], X, Y, p);
					shot[shotcount].SetShotType(type, 3.0, period, 0);
					shotcount++;

					period = (period + 1) % 36;
					shot[shotcount].SetInit(shotgraph[1], X, Y, p);
					shot[shotcount].SetShotType(type, 3.0, period, 1);
					shotcount++;
					
					PlaySoundMem(shotSE[0], DX_PLAYTYPE_BACK, TRUE);
					time += 2;
				}
			}
			else if (type == 2) {
				//X += 5;
				int count = 20;
				if (shotcount - count < maxShot) {
					for (int i = 0; i < count; i++) {
						period = (period + 1) % 36;
						shot[shotcount].SetInit(shotgraph[3], X, Y, p);
						shot[shotcount].SetShotType(type, 3.0, period);
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
						shot[shotcount].SetInit(shotgraph[4], X, Y, p);
						shot[shotcount].SetShotType(type, 20.0, shotcount);
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
						shot[shotcount].SetInit(shotgraph[5], X, Y, p);
						shot[shotcount].SetShotType(type, 4.0, shotcount, period);
						shotcount++;
					}
					PlaySoundMem(shotSE[0], DX_PLAYTYPE_BACK, TRUE);
					time += 4;
				}
			}
		}
		/*’e‚Ì“®‚«•`‰æ*/
		for (int i = 0; i < shotcount; i++) {
			if (Getshot(i)) {
				if (!shot[i].Move()) {//’e‚Ì“®‚«‚Æ‘¬“x
					Compression(i);
					shotcount--; 
				}
			}
		}
		/*Ž©g‚Ì•`‰æ*/
		if (HP > 0) {
			//Y %= SIZE_Y;
			Draw();
			//DrawFormatString(0, 0, GetColor(255, 255, 255), "shotcount = %d", shotcount);
		}
		/*’e‚Ì“–‚½‚è”»’è*/
		for (int i = 0; i < shotcount; i++) {
			if (shot[i].Hit(p)) {
				Compression(i);
				shotcount--;
				return true;
			}
		}
		/*Ž©g‚Æ‚ÌÕ“Ë”»’è*/
		if (HP > 0) {
			return abs(Y - p.GetY()) <= sizeY + p.GetsizeY()
				&& abs(X - p.GetX()) <= sizeX + p.GetsizeX();
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

	/*HP, movespeed, attack*/
	void Setstatus(int HP, int speed, int _attack = 1) {
		SetHP(HP);
		movespeed = speed;
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