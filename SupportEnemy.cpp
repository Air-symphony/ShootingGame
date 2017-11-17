#pragma once
#include "TohoShot.cpp";

const int maxShot_support = 500;
class SupportEnemy :public Character {
private:
	bool Show;
	int ShowLimit;
	int supportTime;
	Graph shotgraph;
	int shotSE[2];
	TohoShot shot[maxShot_support];
	int type;
public:
	SupportEnemy() {
		Show = false;
	}
	/*Unit boss, Unit _player, int x, int y, int _type, int _time = 100*/
	void SetSupportEnemy(Graph _enemygraph, int x, int y, int _type, int _supportTime, int Waittime = 100) {
		if (!SetGraph(_enemygraph)) {
			sizeX = sizeY = 30 / 2;
		}
		X = x, Y = y;
		type = _type;
		time = Waittime;
		movespeed = 3;
		Show = true;
		ShowLimit = GetNowCount();
		supportTime = _supportTime;
	}
	void SetshotSE(int SE[], int count) {
		for (int i = 0; i < count; i++) {
			shotSE[i] = SE[i];
		}
		ChangeVolumeSoundMem(255 * (0.5), shotSE[0]);
	}
	void Setshotgraph(Graph _shotgraph) {
		shotgraph = _shotgraph;
	}
	bool Move(Unit boss, Unit player) {
		if (Show == false) 	return false;

		if (type == 1 || type == 2) {
			if (type == 1) {
				X -= movespeed;
				if (X < boss.GetX() - 100) {
					X = boss.GetX() - 100;
				}
			}
			else if (type == 2) {
				X += movespeed;
				if (X > boss.GetX() + 100) {
					X = boss.GetX() + 100;
				}
			}
			time--;
			if (time <= 0) {
				/*ホーミング*/
				int count = 3;
				if (shotcount - count < maxShot_support) {
					for (int i = 0; i < count; i++) {
						shot[shotcount].Set(shotgraph, 3, X, Y, player, shotcount);
						shotcount++;
					}
					PlaySoundMem(shotSE[0], DX_PLAYTYPE_BACK, TRUE);
					time += 4;
				}
			}
		}
		for (int i = 0; i < shotcount; i++) {
			if (Getshot(i)) {
				if (!shot[i].Move()) {//弾の動きと速度
					Compression(i);
					shotcount--;
				}
			}
		}
		Draw();
		printfDx("Move type(%d)\n", type);
		printfDx("Move size(%d, %d)\n", sizeX, sizeY);
		printfDx("Move (%d, %d) time = %d\n", X, Y, time);
		printfDx("Move time = %d : %d\n", Show, GetNowCount() - ShowLimit);
		printfDx("\n");
		Show = ((GetNowCount() - ShowLimit) <= supportTime);
		for (int i = 0; i < shotcount; i++) {
			if (shot[i].Hit(player, FALSE)) {
				Compression(i);
				shotcount--;
				return true;
			}
		}
		return false;
	}

	bool GetShow() {
		return Show;
	}

	int Getshotcount() {
		return shotcount;
	}

	bool Getshot(int i) {
		return shot[i].Getshot();
	}

	void Compression(int i)
	{
		if (i + 1 < maxShot_support)
		{
			if (shot[i + 1].Getshot() == TRUE) {
				shot[i] = shot[i + 1];
				shot[i + 1].Setshot(FALSE);
				Compression(i + 1);
			}
		}
	}

	void SetShow(bool _Show) {
		Show = _Show;
	}
};