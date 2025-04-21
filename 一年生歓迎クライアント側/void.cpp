#include "Dxlib/DxLib.h"
#include <vector>
#include <string>
#include <iostream>

// ウィンドウサイズ
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;

// マス目のサイズ
const int CELL_SIZE = 23;
// 盤面のサイズ
const int BOARD_SIZE = 19;

// 画像ハンドル
int bgHandle, siroHandle, kuroHandle, blankHandle;

// 盤面データ (1=白, 2=黒, 0=空白)
std::vector<std::vector<int>> board(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0));

// プレイヤーターン (1=Player1, 2=Player2)
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

// 初期化関数
void InitGame() {
	// 画像の読み込み
	bgHandle = LoadGraph("bg.png");
	siroHandle = LoadGraph("siro.png");
	kuroHandle = LoadGraph("kuro.png");
	blankHandle = LoadGraph("blank.png");
}

// ターン情報の描画
void DrawTurnInfo() {
	std::string turnText = "Turn: Player " + std::to_string(currentPlayer);
	DrawString(600, 50, turnText.c_str(), GetColor(255, 255, 255));
}
// 盤面の描画
void DrawBoard() {
	// 背景画像のサイズを取得
	int bgWidth, bgHeight;
	GetGraphSize(bgHandle, &bgWidth, &bgHeight);

	// 背景を描画 (画面中央より少し上に配置)
	int bgX = (WIN_WIDTH / 2) - (bgWidth / 2);
	int bgY = (WIN_HEIGHT / 2) - (bgHeight / 2) - 50; // 少し上に配置
	DrawGraph(bgX, bgY, bgHandle, TRUE);

	// 盤面を描画


	bgX = (WIN_WIDTH / 2) - (bgWidth / 2)+65;
	bgY = (WIN_HEIGHT / 2) - (bgHeight / 2) -10; // 少し上に配置

	for (int y = 0; y < BOARD_SIZE; ++y) {
		for (int x = 0; x < BOARD_SIZE; ++x) {
			int drawX = bgX + x * CELL_SIZE;
			int drawY = bgY + y * CELL_SIZE;
			int drawX2 = drawX + CELL_SIZE; // 縮小後の右下X座標
			int drawY2 = drawY + CELL_SIZE; // 縮小後の右下Y座標

			// マスの状態に応じて画像を描画
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

// メイン関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// DxLib 初期化
	ChangeWindowMode(TRUE);
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);
	if (DxLib_Init() == -1) {
		return -1;
	}
	start_dxlib(WIN_WIDTH, WIN_HEIGHT, TITLE);

	// ゲーム初期化
	InitGame();

	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		// 盤面の描画
		DrawBoard();

		// ターン情報の描画
		DrawTurnInfo();

		// 入力処理
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}

		// マウスクリックで盤面を更新
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			int mouseX, mouseY;
			GetMousePoint(&mouseX, &mouseY);

			int gridX = (mouseX) / CELL_SIZE;
			int gridY = (mouseY) / CELL_SIZE;

			// マウスクリックで盤面を更新
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
				int mouseX, mouseY;
				GetMousePoint(&mouseX, &mouseY);

				// 背景画像の描画位置を考慮
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
