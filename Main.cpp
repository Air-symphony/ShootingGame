#include "InputKey.cpp"
#include "Material.cpp"
#include "PlayerUnit.cpp"
#include "EnemyUnit.cpp"
#include "FileReader.cpp"
#include <iostream>

InputKey input;
Material material;

void Draw_String(int x, int y, char str[]) {//flip����
	int StrLen = strlen(str);
	int StrWidth = GetDrawStringWidth(str, StrLen);
	DrawString(x - StrWidth / 2, y, str, GetColor(255, 255, 255));
}
void Draw_String(int x, int y, char str[], int item) {//flip����
	int StrLen = strlen(str);
	int StrWidth = GetDrawStringWidth(str, StrLen);
	DrawFormatString(x - StrWidth / 2, y, GetColor(255, 255, 255), str, item);
}
void Draw_String(char str[]) {
	int StrLen = strlen(str);
	int StrWidth = GetDrawStringWidth(str, StrLen);
	DrawString(SIZE_X / 2 - StrWidth / 2, SIZE_Y / 2, str, GetColor(255, 255, 255));
	ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������
}
void Draw_Graph(int x, int y, int graph) {
	int sizeX, sizeY;
	if (graph != -1) {
		GetGraphSize(graph, &sizeX, &sizeY);
	}
	else {
		sizeX = sizeY = 100;
	}
	DrawGraph(x - sizeX / 2, y - sizeY / 2, graph, TRUE);
}
void PrintRanking(FileReader fileReader, int rank);

int GameSceneCount = 5;

class Game {
private:
	Player player;
	Enemy enemys[10];
	int maxcount;

	bool loopbgm;
	int saveHP;
	int saveweapon;
	int cleartime;//�N���A���Ԃ̕ۑ�
public:
	Game(){
	}
	/*�Q�[���J�n���̏����ݒ�*/
	void init() {
		loopbgm = false;;
		saveHP = 5;
		saveweapon = 0;
		player.Set(material.playergraph, material.shotgraph, 3, material.reloadSE, material.shotSE, 2);
		cleartime = GetNowCount();
	}

	/*�V�[�����Ə����ݒ�*/
	void SetGameScene(int scene) {
		int _stoptime = GetNowCount();
		player.SetHP(saveHP);
		player.Setweapon(saveweapon);
		player.SetDamageSE(material.damageSE, 2);

		maxcount = 5 + scene;//�G�̐�
		for (int i = 0; i < maxcount; i++) {
			enemys[i].Set(material.enemygraph[scene], material.shotgraph, 3, scene, (i + 1) * (SIZE_X / (maxcount + 1)), 100, 2);//�����
			enemys[i].SetDamageSE(material.damageSE, 2);
		}

		ClearDrawScreen();// ��ʂ�����������
		/*�����`����e*/
		player.DrawGraph();
		for (int id = 0; id < maxcount; id++) {//enemy[count]�̕`��
			enemys[id].DrawGraph();
		}
		for (int i = 0; i < player.GetHP(); i++) {//HP�\��
			DrawGraph(10 + i * 30, SIZE_Y - 50, material.HPgraph, TRUE);//����
		}
		Draw_String("Please push SPACE");
		ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������ 

		while (ProcessMessage() == 0 && input.ForcedTermination() && !input.PushOneframe_Decide()) {}
		_stoptime = GetNowCount() - _stoptime;
		cleartime += _stoptime;
	}

	/*�Q�[�����e*/
	bool PlayGame(int scene) {
		SetGameScene(scene);

		if (CheckSoundMem(material.bgm_pre) == 0 && !loopbgm) {
			PlaySoundMem(material.bgm_loop, DX_PLAYTYPE_BACK);
			loopbgm = (CheckSoundMem(material.bgm_loop) == 1);
		}

		bool start = true;//pause�p
		while (ProcessMessage() == 0 && input.ForcedTermination())
		{
			loopbgm = (CheckSoundMem(material.bgm_loop) == 1);
			if (CheckSoundMem(material.bgm_pre) == 0 && !loopbgm) {
				PlaySoundMem(material.bgm_loop, DX_PLAYTYPE_BACK);
			}
			else if (!loopbgm) {
				PlaySoundMem(material.bgm_loop, DX_PLAYTYPE_BACK);
			}

			ClearDrawScreen();// ��ʂ�����������

			/*�v���C���[�̓���*/
			player.Move();
			player.Shoot();

			/*����^�C�v�̕\��*/
			DrawFormatString(10 + 5 * 30, SIZE_Y - 60, GetColor(255, 255, 255), "weapon(%d)", player.Getweapon() + 1);


			/*�G�̐�*/
			int finish = maxcount;
			/*�G�̓���*/
			for (int id = 0; id < maxcount; id++) {//enemy[count]�̕`��
				if (enemys[id].Move(player)) {//�G�̕`��ƒe����
					player.Damege(enemys[id].Getattack());
				}
				if (enemys[id].GetHP() > 0) {//�����Ă����
					if (player.HitCheck(enemys[id])) {//�����̒e�̊m�F
						enemys[id].Damege(player.Getattack());
					}
				}
				else {
					finish--;
				}
			}

			for (int i = 0; i < player.GetHP(); i++) {//HP�\��
				DrawGraph(10 + i * 30, SIZE_Y - 50, material.HPgraph, TRUE);//����
			}
			
			//�f�o�b�O�p
			input.setJoypad();
			DrawFormatString(0, 0, GetColor(255, 255, 255), "joypad=%d", input.getJoypad());
			/*clsDx();
			printfDx("HP=%d, (%d.%d, %d)\n", player.GetHP(), player.GetX(), player.GetY(), player.Getweapon());
			printfDx("�G(�c��%d��)\n", finish);
			for (int id = 0; id < maxcount; id++) {//enemy[count]�̕`��
				printfDx("%d_%d(",id, enemys[id].GetHP());
				for (int i = 0; i < enemys[id].Getshotcount(); i++) {
					printfDx("%d,", enemys[id].Getshot(i));
				}
				printfDx(")\n");
			}*/

			if (input.PushOneframe_Stop()) {
				//Draw_String(SIZE_X / 2, SIZE_Y * 3 / 5, "BACKSPACE�FContinue\nSPACE�FGo back Title");
				while (ProcessMessage() == 0 && input.ForcedTermination()) {
					int stoptime = GetNowCount();
					if (input.PushOneframe_KeyUP() ||
						input.PushOneframe_KeyDOWN()) {
						PlaySoundMem(material.cursorSE[2], DX_PLAYTYPE_BACK);
						start = !start;
					}
					Draw_Graph(SIZE_X / 2, SIZE_Y * 3 / 5, material.textgraph[0]);
					Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10, material.textgraph[2]);
					if (start) Draw_Graph(SIZE_X / 2, SIZE_Y * 3 / 5, material.textgraph[1]);
					else Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10, material.textgraph[3]);
					Draw_String("Pause");

					if (input.PushOneframe_RETURN()) {
						if (start) {
							stoptime = GetNowCount() - stoptime;
							cleartime = cleartime + stoptime;
							PlaySoundMem(material.cursorSE[0], DX_PLAYTYPE_BACK);
							break;
						}
						else {
							PlaySoundMem(material.cursorSE[1], DX_PLAYTYPE_BACK);
							StopSoundMem(material.bgm_loop);
							return false;
						}
					}
				}
			}

			if (input.PushOneframe_RETURN()) finish = 0;
			ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������

			/*���̃V�[�����I�������*/
			if (player.GetHP() <= 0 || finish <= 0) {
				/*�������ꍇ*/
				if (player.GetHP() <= 0) {
					StopSoundMem(material.bgm_loop);
					Draw_String("Game Over");
				}
				/*�S�ẴV�[�����I�������*/
				else if (scene >= GameSceneCount) {
					cleartime = GetNowCount() - cleartime;
					float time = ((float)cleartime / 1000.0);
					Draw_String(SIZE_X / 2, SIZE_Y / 2 + 50, "Time : %f s", time);
					Draw_String("Finish");
					StopSoundMem(material.bgm_loop);
				}
				/*�r���V�[�����I�������*/
				else {
					saveHP = player.GetHP();
					saveweapon = player.Getweapon();
					Draw_String("Next Stage");
				}
				int waittime = 1000;
				WaitTimer(waittime);//1�b�ҋ@
				cleartime += waittime;
				return player.GetHP() > 0;
			}
		}
		return false;
	}

	int getCleartime() {
		return cleartime;
	}
};

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	ChangeWindowMode(TRUE);//window���[�h�ɐݒ�
	SetGraphMode(SIZE_X, SIZE_Y, 32);
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	SetDrawScreen(DX_SCREEN_BACK);

	material.SetMatrial();
	SetFontSize(40);

	FileReader fileReader;
	Game game;
	input.setJoypad();
	int select = 0;//start�ɃJ�[�\�������邩�ǂ���
	while (ProcessMessage() == 0 && input.ForcedTermination()) {
		ClearDrawScreen();// ��ʂ�����������
		clsDx();

		if (input.PushOneframe_KeyUP()) {
			PlaySoundMem(material.cursorSE[2], DX_PLAYTYPE_BACK);
			select -= 1;
			if (select < 0)select += 3;
		}
		if (input.PushOneframe_KeyDOWN()) {
			PlaySoundMem(material.cursorSE[2], DX_PLAYTYPE_BACK);
			select += 1;
			select %= 3;
		}

		DrawFormatString(0, 0, GetColor(255, 255, 255), "joypad=%d", input.getJoypad());

		Draw_Graph(SIZE_X / 2, SIZE_Y * 5 / 10, material.textgraph[0]);
		Draw_Graph(SIZE_X / 2, SIZE_Y * 6 / 10, material.textgraph[4]);
		Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10, material.textgraph[2]);
		if (select == 0) Draw_Graph(SIZE_X / 2, SIZE_Y * 5 / 10, material.textgraph[1]);
		else if (select == 1)Draw_Graph(SIZE_X / 2, SIZE_Y * 6 / 10, material.textgraph[5]);
		else Draw_Graph(SIZE_X / 2, SIZE_Y * 7 / 10, material.textgraph[3]);

		Draw_String(SIZE_X / 2, SIZE_Y / 5, "Shooting");
		ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������

		if (input.PushOneframe_Decide()) {
			/*Start*/
			if (select == 0) {
				PlaySoundMem(material.cursorSE[0], DX_PLAYTYPE_BACK);
				game.init();
				/*�Q�[�����e*/
				for (int i = 1; i <= GameSceneCount; i++) {
					if (!game.PlayGame(i)) {
						break;
					}
				}
				/*�����I���̏ꍇ�͖���*/
				if (input.ForcedTermination()) {
					int rank = fileReader.CheckInRanking((double)game.getCleartime() / 1000.0);
					PrintRanking(fileReader, rank);
				}
			}
			/*Ranking*/
			else if (select == 1) {
				PlaySoundMem(material.cursorSE[0], DX_PLAYTYPE_BACK);
				PrintRanking(fileReader, -1);
			}
			/*Finish*/
			else {
				PlaySoundMem(material.cursorSE[1], DX_PLAYTYPE_BACK);
				break;
			}
		}
	}
	material.DeleteMaterial();
	DxLib_End();				// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 
}

/*�����L���O�̕\��
0 <= rank <= 9 �����L���O����*/
void PrintRanking(FileReader fileReader, int rank) {
	float _rank[10];
	for (int i = 0; i < fileReader.getRANKING(); i++) {
		_rank[i] = fileReader.rank[i];
	}

	ClearDrawScreen();// ��ʂ�����������
	for (int i = 0; i < fileReader.getRANKING(); i++) {
		DrawFormatString(SIZE_X / 3 - 20, 100 + 40 * i, GetColor(255, 255, 255), "%d��:", i + 1);
		DrawFormatString(SIZE_X / 3 + 80, 100 + 40 * i, GetColor(255, 255, 255), "%f", _rank[i]);
	}
	if (0 <= rank) {
		DrawFormatString(SIZE_X / 3 + 80, 100 + 40 * rank, GetColor(0, 255, 255), "%f", _rank[rank]);
		fileReader.UpdateFile();
	}
	ScreenFlip();// ����ʂ̓��e��\��ʂɔ��f������
	while (ProcessMessage() == 0 && input.ForcedTermination()) {
		if (input.PushOneframe_Decide()) {
			PlaySoundMem(material.cursorSE[0], DX_PLAYTYPE_BACK);
			break;
		}
	}
}