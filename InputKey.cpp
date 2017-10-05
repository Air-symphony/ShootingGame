#pragma once
#include "DxLib.h"

class InputKey {
	static const int keytype = 7;//1frameƒ{ƒ^ƒ“‚ÌŽí—Þ
	int keytime[keytype];
public:
	bool PushOneframe_Dicide() {
		if (CheckHitKey(KEY_INPUT_SPACE) == 1) keytime[0]++;
		else keytime[0] = 0;

		return keytime[0] == 1;
	}
	bool PushOneframe_Attack() {
		if (CheckHitKey(KEY_INPUT_SPACE) == 1) keytime[0]++;
		else keytime[0] = 0;

		return keytime[0] == 1;
	}
	bool PushOneframe_WeaponChange() {
		if (CheckHitKey(KEY_INPUT_C) == 1) keytime[1]++;
		else keytime[1] = 0;

		return keytime[1] == 1;
	}
	bool PushOneframe_Stop() {
		if (CheckHitKey(KEY_INPUT_BACK) == 1) keytime[2]++;
		else keytime[2] = 0;

		return keytime[2] == 1;
	}
	bool PushOneframe_Reset() {
		if (CheckHitKey(KEY_INPUT_R) == 1) keytime[3]++;
		else keytime[3] = 0;

		return keytime[3] == 1;
	}
	bool PushOneframe_RETURN() {
		if (CheckHitKey(KEY_INPUT_RETURN) == 1) keytime[4]++;
		else keytime[4] = 0;

		return keytime[4] == 1;
	}
	bool PushOneframe_KeyUP() {
		if (CheckHitKey(KEY_INPUT_UP) == 1) keytime[5]++;
		else keytime[5] = 0;

		return keytime[5] == 1;
	}
	bool PushOneframe_KeyDOWN() {
		if (CheckHitKey(KEY_INPUT_DOWN) == 1) keytime[6]++;
		else keytime[6] = 0;

		return keytime[6] == 1;
	}
};