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
// �E�B���h�E�T�C�Y
extern const int WIN_WIDTH;
extern const int WIN_HEIGHT;
// �}�X�ڂ̃T�C�Y
extern const int CELL_SIZE;
// �Ֆʂ̃T�C�Y
extern const int BOARD_SIZE;
// �摜�n���h��
extern int bgHandle, siroHandle, kuroHandle, blankHandle, kuro2Handle;///�}�E�X�I�[�o�[���̉摜
// �Ֆʃf�[�^ (1=��, 2=��, 0=��)
extern std::vector<std::vector<int>> board;
// �v���C���[�^�[�� (1=Player1, 2=Player2)
extern int currentPlayer;
extern const char* TITLE;
extern char key;
extern const std::vector<unsigned int> cellColors;

extern int putx; 
extern int puty;
extern std::string rsv;


