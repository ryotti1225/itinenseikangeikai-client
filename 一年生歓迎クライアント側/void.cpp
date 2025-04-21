#include "Dxlib/DxLib.h"
#include <vector>
#include <string>
#include <iostream>

// �E�B���h�E�T�C�Y
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;

// �}�X�ڂ̃T�C�Y
const int CELL_SIZE = 23;
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
	// �w�i�摜�̃T�C�Y���擾
	int bgWidth, bgHeight;
	GetGraphSize(bgHandle, &bgWidth, &bgHeight);

	// �w�i��`�� (��ʒ�����菭����ɔz�u)
	int bgX = (WIN_WIDTH / 2) - (bgWidth / 2);
	int bgY = (WIN_HEIGHT / 2) - (bgHeight / 2) - 50; // ������ɔz�u
	DrawGraph(bgX, bgY, bgHandle, TRUE);

	// �Ֆʂ�`��


	bgX = (WIN_WIDTH / 2) - (bgWidth / 2)+65;
	bgY = (WIN_HEIGHT / 2) - (bgHeight / 2) -10; // ������ɔz�u

	for (int y = 0; y < BOARD_SIZE; ++y) {
		for (int x = 0; x < BOARD_SIZE; ++x) {
			int drawX = bgX + x * CELL_SIZE;
			int drawY = bgY + y * CELL_SIZE;
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
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			int mouseX, mouseY;
			GetMousePoint(&mouseX, &mouseY);

			int gridX = (mouseX) / CELL_SIZE;
			int gridY = (mouseY) / CELL_SIZE;

			// �}�E�X�N���b�N�ŔՖʂ��X�V
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
				int mouseX, mouseY;
				GetMousePoint(&mouseX, &mouseY);

				// �w�i�摜�̕`��ʒu���l��
				int bgX = (WIN_WIDTH / 2) - (BOARD_SIZE * CELL_SIZE / 2);
				int bgY = (WIN_HEIGHT / 2) - (BOARD_SIZE * CELL_SIZE / 2) - 50;

				int gridX = (mouseX - bgX) / CELL_SIZE;
				int gridY = (mouseY - bgY) / CELL_SIZE;

				if (gridX >= 0 && gridX < BOARD_SIZE && gridY >= 0 && gridY < BOARD_SIZE) {
					if (board[gridY][gridX] == 0) {
						board[gridY][gridX] = currentPlayer;
						currentPlayer = (currentPlayer == 1) ? 2 : 1;
					}
				}
			}
		}

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}
