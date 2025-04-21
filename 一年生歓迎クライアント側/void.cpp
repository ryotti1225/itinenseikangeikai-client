#include "Dxlib/DxLib.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include<array>

///ret true �ܖڕ��א���
///		false game���s
bool Judge(int x, int y, std::vector<std::vector<int>>& board)
{
	int col = board[x][y];
	if (col == 0) return false;

	// 8�����̃`�F�b�N�p�̔z��
	const int dx[] = { 1, 0, 1, -1 };  // �c�A���A�΂߁i�E���j�A�΂߁i�����j
	const int dy[] = { 0, 1, 1, 1 };

	for (int dir = 0; dir < 4; dir++) {
		int count = 1;  // ���݂̈ʒu��1����J�E���g�J�n

		// ������
		for (int i = 1; i < 5; i++) {
			int nx = x + dx[dir] * i;
			int ny = y + dy[dir] * i;

			if (nx < 0 || nx >= board.size() || ny < 0 || ny >= board[0].size() || board[nx][ny] != col) {
				break;
			}
			count++;
		}

		// �t����
		for (int i = 1; i < 5; i++) {
			int nx = x - dx[dir] * i;
			int ny = y - dy[dir] * i;

			if (nx < 0 || nx >= board.size() || ny < 0 || ny >= board[0].size() || board[nx][ny] != col) {
				break;
			}
			count++;
		}

		if (count >= 5) {
			return true;
		}
	}
	return false;
}


// �E�B���h�E�T�C�Y
const int WIN_WIDTH = 1280;
const int WIN_HEIGHT = 960;

// �}�X�ڂ̃T�C�Y
const int CELL_SIZE = 22;

// �Ֆʂ̃T�C�Y
const int BOARD_SIZE = 19;

// �摜�n���h��
int bgHandle, siroHandle, kuroHandle, blankHandle;

// �Ֆʃf�[�^ (1=��, 2=��, 0=��)
std::vector<std::vector<int>> board(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0));

// �v���C���[�^�[�� (1=Player1, 2=Player2)
int currentPlayer = 1;

const char* TITLE = "Untitled_Dxlib_window";


void start_dxlib(int WIN_WIDTH, int WIN_HEIGHT, const char* TITLE)
{
	ChangeWindowMode(true);
	SetWindowSizeChangeEnableFlag(true, true);
	SetMainWindowText((const TCHAR*)TITLE);
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);
	SetWindowSizeExtendRate(1.0);
	SetBackgroundColor(0, 128, 128);
	SetDrawScreen(DX_SCREEN_BACK);

	if (DxLib_Init() == -1)
	{
		std::cout << "#WARNIN# DxLib init FAILED #WARNIN#";
	}

}

// �������֐�
void InitGame() {
	// �摜�̓ǂݍ���
	bgHandle = LoadGraph("bg.png");
	siroHandle = LoadGraph("siro.png");
	kuroHandle = LoadGraph("kuro.png");
	blankHandle = LoadGraph("blank.png");
}

// �^�[�����̕`��
void DrawTurnInfo() {
	std::string turnText = "Turn: Player " + std::to_string(currentPlayer);
	DrawString(600, 50, turnText.c_str(), GetColor(255, 255, 255));
}
// �Ֆʂ̕`��
void DrawBoard() {
	// �w�i�摜�̕��ƍ������擾
	int bgWidth, bgHeight;
	GetGraphSize(bgHandle, &bgWidth, &bgHeight);

	// �w�i����ʒ����ɕ`��
	int bgX = (WIN_WIDTH - bgWidth) / 2;
	int bgY = ((WIN_HEIGHT - bgHeight) / 2)-80;
	DrawGraph(bgX, bgY, bgHandle, TRUE);

	// �Ֆʂ̕��ƍ������v�Z
	int boardWidth = BOARD_SIZE * CELL_SIZE;
	int boardHeight = BOARD_SIZE * CELL_SIZE;

	// �Ֆʂ̍���̕`��J�n�ʒu���v�Z
	int startX = (WIN_WIDTH - boardWidth) / 2;
	int startY =  ((WIN_HEIGHT - boardHeight) / 2)-80;

	// �Ֆʂ�`��
	for (int y = 0; y < BOARD_SIZE; ++y) {
		for (int x = 0; x < BOARD_SIZE; ++x) {
			int drawX = startX + x * CELL_SIZE;
			int drawY = startY + y * CELL_SIZE;
			int drawX2 = drawX + CELL_SIZE; // �k����̉E��X���W
			int drawY2 = drawY + CELL_SIZE; // �k����̉E��Y���W

			// �}�X�̏�Ԃɉ����ĉ摜��`��
			if (board[y][x] == 1) {
				DrawExtendGraph(drawX, drawY, drawX2, drawY2, siroHandle, TRUE);
			}
			else if (board[y][x] == 2) {
				DrawExtendGraph(drawX, drawY, drawX2, drawY2, kuroHandle, TRUE);
			}
			else {
				DrawExtendGraph(drawX, drawY, drawX2, drawY2, blankHandle, TRUE);
			}
		}
	}
}

// ���C���֐�
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// DxLib ������
	ChangeWindowMode(TRUE);
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);
	if (DxLib_Init() == -1) {
		return -1;
	}
	start_dxlib(WIN_WIDTH, WIN_HEIGHT, TITLE);

	// �Q�[��������
	InitGame();

	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		// �Ֆʂ̕`��
		DrawBoard();

		// �^�[�����̕`��
		DrawTurnInfo();

		// ���͏���
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}

		// �}�E�X�N���b�N�ŔՖʂ��X�V
		// �}�E�X�N���b�N�ŔՖʂ��X�V
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			int mouseX, mouseY;
			GetMousePoint(&mouseX, &mouseY);

			// �Ֆʂ̍���̕`��J�n�ʒu���v�Z
			int boardWidth = BOARD_SIZE * CELL_SIZE;
			int boardHeight = BOARD_SIZE * CELL_SIZE;
			int startX = (WIN_WIDTH - boardWidth) / 2;
			int startY = ((WIN_HEIGHT - boardHeight) / 2) - 80;

			// �}�E�X���W��Ֆʂ̍��W�ɕϊ�
			int gridX = (mouseX - startX) / CELL_SIZE;
			int gridY = (mouseY - startY) / CELL_SIZE;

			// �Ֆʂ͈͓̔����m�F
			if (gridX >= 0 && gridX < BOARD_SIZE && gridY >= 0 && gridY < BOARD_SIZE) {
				if (board[gridY][gridX] == 0) {
					board[gridY][gridX] = currentPlayer;
					currentPlayer = (currentPlayer == 1) ? 2 : 1;

			if (Judge(gridY, gridX, board))
			{
				std::string turnText = "winner: Player " + std::to_string(currentPlayer);
				DrawString(300, 50, turnText.c_str(), GetColor(255, 255, 255));


			}
				}
			}
		}

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}
