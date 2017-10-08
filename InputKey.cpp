#pragma once
#include "DxLib.h"

/*
PAD_INPUT_1 A
PAD_INPUT_2 B
PAD_INPUT_3 X
PAD_INPUT_4 Y
PAD_INPUT_5 L
PAD_INPUT_6 R
PAD_INPUT_7 SELECT
PAD_INPUT_8 START
*/
class InputKey {
private:
	int id;
	static const int keytype = 7;//1frameÉ{É^ÉìÇÃéÌóﬁ
	int keytime[keytype];
	bool PushOneframe(int keyCode) {
		if (CheckHitKey(keyCode) == 1) keytime[id]++;
		else keytime[id] = 0;
		return keytime[id] == 1;
	}
	bool PushOneframe(int PADID, int keyCode) {
		if ((GetJoypadInputState(PADID) & keyCode) != 0) keytime[id]++;
		else keytime[id] = 0;
		return keytime[id] == 1;
	}
	int joypad = 0;
public:
	InputKey() {
		joypad = GetJoypadNum();
	}
	int getJoypad() {
		return joypad;
	}
	/*0, KEY_INPUT_SPACE*/
	bool PushOneframe_Decide() {
		id = 0;
		if (joypad > 0) return PushOneframe(DX_INPUT_PAD1, PAD_INPUT_1);
		return PushOneframe(KEY_INPUT_SPACE);
	}
	/*0, KEY_INPUT_SPACE*/
	bool PushOneframe_Attack() {
		id = 0;
		if (joypad > 0) return PushOneframe(DX_INPUT_PAD1, PAD_INPUT_1);
		return PushOneframe(KEY_INPUT_SPACE);
	}
	/*1, KEY_INPUT_C*/
	bool PushOneframe_WeaponChange() {
		id = 1;
		if (joypad > 0) return PushOneframe(DX_INPUT_PAD1, PAD_INPUT_5);
		return PushOneframe(KEY_INPUT_C);
	}
	/*2, KEY_INPUT_BACK*/
	bool PushOneframe_Stop() {
		id = 2;
		if (joypad > 0) return PushOneframe(DX_INPUT_PAD1, PAD_INPUT_8);
		return PushOneframe(KEY_INPUT_BACK);
	}
	/*3, KEY_INPUT_R*/
	bool PushOneframe_Reset() {
		id = 3;
		//if (joypad > 0) return PushOneframe(DX_INPUT_PAD1, PAD_INPUT_7);
		return PushOneframe(KEY_INPUT_R);
	}
	/*4, KEY_INPUT_RETURN*/
	bool PushOneframe_RETURN() {
		id = 4;
		if (joypad > 0) return PushOneframe(DX_INPUT_PAD1, PAD_INPUT_7);
		return PushOneframe(KEY_INPUT_RETURN);
	}
	/*5, KEY_INPUT_UP*/
	bool PushOneframe_KeyUP() {
		id = 5;
		if (joypad > 0) return PushOneframe(DX_INPUT_PAD1, PAD_INPUT_UP);
		return PushOneframe(KEY_INPUT_UP);
	}
	/*6, KEY_INPUT_DOWN*/
	bool PushOneframe_KeyDOWN() {
		id = 6;
		if (joypad > 0) return PushOneframe(DX_INPUT_PAD1, PAD_INPUT_DOWN);
		return PushOneframe(KEY_INPUT_DOWN);
	}


	/*CheckHitKey(KEY_INPUT_UP) == 1*/
	bool Push_KeyUP() {
		if (joypad > 0) return (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_UP) != 0;
		return CheckHitKey(KEY_INPUT_UP) == 1;
	}
	/*CheckHitKey(KEY_INPUT_DOWN) == 1*/
	bool Push_KeyDOWN() {
		if (joypad > 0) return (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_DOWN) != 0;
		return CheckHitKey(KEY_INPUT_DOWN) == 1;
	}
	/*CheckHitKey(KEY_INPUT_LEFT) == 1*/
	bool Push_KeyLEFT() {
		if (joypad > 0) return (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_LEFT) != 0;
		return CheckHitKey(KEY_INPUT_LEFT) == 1;
	}
	/*CheckHitKey(KEY_INPUT_RIGHT) == 1*/
	bool Push_KeyRIGHT() {
		if (joypad > 0) return (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_RIGHT) != 0;
		return CheckHitKey(KEY_INPUT_RIGHT) == 1;
	}
	/*ã≠êßèIóπ âüÇ≥ÇÍÇƒÇ»ÇØÇÍÇŒ
	CheckHitKey(KEY_INPUT_ESCAPE) == 0*/
	bool ForcedTermination() {
		return CheckHitKey(KEY_INPUT_ESCAPE) == 0;
	}
};