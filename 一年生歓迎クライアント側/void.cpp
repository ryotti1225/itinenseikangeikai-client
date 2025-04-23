#include "Dxlib/DxLib.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include<array>

///ret true 五目並べ成立
///		false game続行
bool Judge(int x, int y, std::vector<std::vector<int>>& board)
{
	int col = board[x][y];
	if (col == 0) return false;

	// 8方向のチェック用の配列
	const int dx[] = { 1, 0, 1, -1 };  // 縦、横、斜め（右下）、斜め（左下）
	const int dy[] = { 0, 1, 1, 1 };

	for (int dir = 0; dir < 4; dir++) {
		int count = 1;  // 現在の位置を1からカウント開始

		// 正方向
		for (int i = 1; i < 5; i++) {
			int nx = x + dx[dir] * i;
			int ny = y + dy[dir] * i;

			if (nx < 0 || nx >= board.size() || ny < 0 || ny >= board[0].size() || board[nx][ny] != col) {
				break;
			}
			count++;
		}
		// 逆方向
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


std::array<std::string,5> rsv_question(){
	return{ "Aを選んでください" ,"a","b","c","d" };
}

/// <summary>
/// 問題の解答を送信する
/// ｒｙｏｔｔｉ１２１５よろしく
/// </summary>
/// <param name="answer">答え</param>
/// <returns></returns>
int send_answer(char answer) {
	return 0;
}

/// <summary>
/// 盤面を書き換える
/// ｒｙｏｔｔｉ１２１５よろしく 
/// </summary>
/// <param name="board"></param>
/// <returns></returns>
int update_board(std::vector<std::vector<int>>& board) {
	static std::vector<std::vector<int>> new_board = board;///新しい盤面
	std::vector<std::vector<int>> rsv_board;



	///一時的に今の盤面をそのまま持っとく
	new_board = board;
	///盤面の受け取りの代わり

	if (!rsv_board.empty()){
		new_board = rsv_board;
	}
	board = new_board;
	return 0;
}

// ウィンドウサイズ
const int WIN_WIDTH = 1280;
const int WIN_HEIGHT = 960;

// マス目のサイズ
const int CELL_SIZE = 33;

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
	// 背景画像の幅と高さを取得
	int bgWidth, bgHeight;
	GetGraphSize(bgHandle, &bgWidth, &bgHeight);

	// 背景を画面中央に描画
	int bgX = (WIN_WIDTH - bgWidth) / 2;
	int bgY = ((WIN_HEIGHT - bgHeight) / 2)-80;
	DrawGraph(bgX, bgY, bgHandle, TRUE);

	// 盤面の幅と高さを計算
	int boardWidth = BOARD_SIZE * CELL_SIZE;
	int boardHeight = BOARD_SIZE * CELL_SIZE;

	// 盤面の左上の描画開始位置を計算
	int startX = (WIN_WIDTH - boardWidth) / 2;
	int startY =  ((WIN_HEIGHT - boardHeight) / 2)-80;

	// 盤面を描画
	for (int y = 0; y < BOARD_SIZE; ++y) {
		for (int x = 0; x < BOARD_SIZE; ++x) {
			int drawX = startX + x * CELL_SIZE;
			int drawY = startY + y * CELL_SIZE;
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


const std::vector<unsigned int> cellColors = {
	GetColor(255, 230, 230),
	GetColor(230, 255, 230),
	GetColor(230, 230, 255),
	GetColor(255, 255, 230)
};

/// マウスオーバー時のハイライト色
const unsigned int hoverColor = GetColor(255, 255, 255);

void DrawRefinedStringTable(int left, int top, int right, int bottom, const std::vector<std::vector<std::string>>& table, int fontHandle = -1) {
	size_t rows = table.size();
	size_t cols = 0;
	for (const auto& row : table) {
		if (row.size() > cols) cols = row.size();
	}

	int tableWidth = right - left;
	int tableHeight = bottom - top;

	int columnWidth = tableWidth / (int)cols;
	int lineHeight = tableHeight / (int)rows;

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	for (size_t row = 0; row < rows; ++row) {
		for (size_t col = 0; col < table[row].size(); ++col) {
			int cellX = left + col * columnWidth;
			int cellY = top + row * lineHeight;

			bool isHover = mouseX >= cellX && mouseX < cellX + columnWidth &&
				mouseY >= cellY && mouseY < cellY + lineHeight;

			int bgColor = isHover ? GetColor(255, 255, 255) : cellColors[(row + col) % cellColors.size()];
			int borderColor = GetColor(60, 60, 60);  // 濃い目のグレー

			/// セル背景
			DrawBox(cellX, cellY, cellX + columnWidth, cellY + lineHeight, bgColor, TRUE);
			/// セル縁取り
			DrawBox(cellX, cellY, cellX + columnWidth, cellY + lineHeight, borderColor, FALSE);

			/// 中央揃え＋影
			const std::string& text = table[row][col];
			int textWidth = GetDrawStringWidthToHandle(text.c_str(), text.size(), fontHandle);
			int textHeight = GetFontSize();

			int centerX = cellX + (columnWidth - textWidth) / 2;
			int centerY = cellY + (lineHeight - textHeight) / 2;

			DrawStringToHandle(centerX + 2, centerY + 2, text.c_str(), GetColor(100, 100, 100), fontHandle);
			DrawStringToHandle(centerX, centerY, text.c_str(), GetColor(0, 0, 0), fontHandle);
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





	int fontHandle = CreateFontToHandle("MS 明朝", 28, 6, DX_FONTTYPE_ANTIALIASING);

	std::vector<std::vector<std::string>> table = {
		{"std::stringの内容"},
		{"赤", "緑"},
		{"青", "黄"}
	};

	int left = 100, top = 100, right = 500, bottom = 300;

	
	
	
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
		// マウスクリックで盤面を更新
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			int mouseX, mouseY;
			GetMousePoint(&mouseX, &mouseY);

			// 盤面の左上の描画開始位置を計算
			int boardWidth = BOARD_SIZE * CELL_SIZE;
			int boardHeight = BOARD_SIZE * CELL_SIZE;
			int startX = (WIN_WIDTH - boardWidth) / 2;
			int startY = ((WIN_HEIGHT - boardHeight) / 2) - 80;

			// マウス座標を盤面の座標に変換
			int gridX = (mouseX - startX) / CELL_SIZE;
			int gridY = (mouseY - startY) / CELL_SIZE;

			// 盤面の範囲内か確認
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

		if (CheckHitKey(KEY_INPUT_LEFT) == 1) left -= 1;
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1) left += 1;
		if (CheckHitKey(KEY_INPUT_UP) == 1) top -= 1;
		if (CheckHitKey(KEY_INPUT_DOWN) == 1) top += 1;

		DrawRefinedStringTable(left, top, right, bottom, table, fontHandle);




		ScreenFlip();
	}

	DxLib_End();
	return 0;
}
