#pragma once
#include "Dxlib.h"
/*
画像表示関係
int graphID;
int sizeX, sizeY;
*/
class Graph {
public:
	/*画像ハンドラ*/
	int graphID;
	/*中心からの画像サイズ*/
	int sizeX, sizeY;
	Graph() {
		graphID = NULL;
	}
	/*
	Graph(LoadGraph("\\00.png"))
	*/
	Graph(int _graphID) {
		graphID = _graphID;
		GetGraphSize(graphID, &sizeX, &sizeY);
		sizeX = sizeX / 2;
		sizeY = sizeY / 2;
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
	x, y = 画像を表示させたい座標
	number = 特定の位置を中心として表示(テンキー)
	*/
	void Draw_Graph(int x, int y, int number) {
		if (number == 1)
			DrawGraph(x, y, graphID, TRUE);
		else if (number == 2)
			DrawGraph(x - sizeX, y, graphID, TRUE);
		else if (number == 3)
			DrawGraph(x + sizeX , y, graphID, TRUE);
		else if (number == 4)
			DrawGraph(x, y - sizeY, graphID, TRUE);
		else if (number == 5)
			DrawGraph(x - sizeX, y - sizeY, graphID, TRUE);
		else if (number == 6)
			DrawGraph(x + sizeX, y - sizeY, graphID, TRUE);
		else if (number == 7)
			DrawGraph(x, y + sizeY, graphID, TRUE);
		else if (number == 8)
			DrawGraph(x - sizeX, y + sizeY, graphID, TRUE);
		else if (number == 9)
			DrawGraph(x + sizeX, y + sizeY, graphID, TRUE);
	}
	int GetsizeX() {
		return sizeX;
	}
	int GetsizeY() {
		return sizeY;
	}
};