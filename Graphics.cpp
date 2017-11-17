#pragma once
#include "Dxlib.h"
/*
�摜�\���֌W
int graphID;
int sizeX, sizeY;
*/
class Graph {
public:
	/*�摜�n���h��*/
	int graphID;
	/*���S����̉摜�T�C�Y*/
	int sizeX, sizeY;
	Graph() {
		graphID = NULL;
		sizeX = sizeY = 0;
	}
	/*
	Graph(LoadGraph("\\00.png"))
	*/
	Graph(int _graph) {
		SetGraph(_graph);
	}
	/*
	SetGraph(LoadGraph("\\00.png"))
	*/
	void SetGraph(int graph) {
		graphID = graph;
		GetGraphSize(graphID, &sizeX, &sizeY);
		sizeX = sizeX / 2;
		sizeY = sizeY / 2;
	}
	/*
	if (graph.graphID == -1) 
			return false;
	graphID = graph.graphID;
	sizeX = graph.GetsizeX();
	sizeY = graph.GetsizeY();
	return true;
		*/
	bool SetGraph(Graph graph) {
		if (graph.graphID == -1) 
			return false;

		graphID = graph.graphID;
		sizeX = graph.GetsizeX();
		sizeY = graph.GetsizeY();
		return true;
	}
	/*
	x, y = �摜��\�������������W
	number = ����̈ʒu�𒆐S�Ƃ��ĕ\��(�e���L�[)
	*/
	void Draw_Graph(int x, int y, int number = 5) {
		if (number == 1)
			DrawGraph(x, y, graphID, TRUE);
		else if (number == 2)
			DrawGraph(x - sizeX, y, graphID, TRUE);
		else if (number == 3)
			DrawGraph(x - sizeX * 2, y, graphID, TRUE);
		else if (number == 4)
			DrawGraph(x, y - sizeY, graphID, TRUE);
		else if (number == 5)
			DrawGraph(x - sizeX, y - sizeY, graphID, TRUE);
		else if (number == 6)
			DrawGraph(x - sizeX * 2, y - sizeY, graphID, TRUE);
		else if (number == 7)
			DrawGraph(x, y - sizeY * 2, graphID, TRUE);
		else if (number == 8)
			DrawGraph(x - sizeX, y - sizeY * 2, graphID, TRUE);
		else if (number == 9)
			DrawGraph(x - sizeX * 2, y - sizeY * 2, graphID, TRUE);
	}
	/*
	�g��\���A�w�i�p
	DrawExtendGraph(0, 0, display_X, display_Y, graphID, TRUE);*/
	void Draw_BackGround(int display_X, int display_Y) {
		DrawExtendGraph(0, 0, display_X, display_Y, graphID, TRUE);
	}
	/*���S����̉摜�T�C�Y*/
	int GetsizeX() {
		return sizeX;
	}
	/*���S����̉摜�T�C�Y*/
	int GetsizeY() {
		return sizeY;
	}
	void Delete() {
		DeleteGraph(graphID);
	}
};