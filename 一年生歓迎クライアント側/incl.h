#pragma once

#include "Dxlib/DxLib.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include<array>
#include"json.hpp"
#include "nw.h"


extern bool put;
// ウィンドウサイズ
extern const int WIN_WIDTH;
extern const int WIN_HEIGHT;
// マス目のサイズ
extern const int CELL_SIZE;
// 盤面のサイズ
extern const int BOARD_SIZE;
// 画像ハンドル
extern int bgHandle, siroHandle, kuroHandle, blankHandle, kuro2Handle;///マウスオーバー時の画像
// 盤面データ (1=白, 2=黒, 0=空白)
extern std::vector<std::vector<int>> board;
// プレイヤーターン (1=Player1, 2=Player2)
extern int currentPlayer;
extern const char* TITLE;
extern char key;
extern const std::vector<unsigned int> cellColors;

extern int putx; 
extern int puty;
extern std::string rsv;


