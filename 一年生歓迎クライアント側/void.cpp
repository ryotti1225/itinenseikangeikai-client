#include "incl.h"

bool put = false;
// ウィンドウサイズ
const int WIN_WIDTH = 1280;
const int WIN_HEIGHT = 960;
// マス目のサイズ
const int CELL_SIZE = 33;
// 盤面のサイズ
const int BOARD_SIZE = 19;
// 画像ハンドル
int bgHandle, siroHandle, kuroHandle, blankHandle, kuro2Handle; /// マウスオーバー時の画像
int SEputHandle, SEtrueHandle, SEfalseHandle; /// 置いたときの音.etc
int GEtrueHandle, GEfalseHandle; /// 
// 盤面データ (1=白, 2=黒, 0=空白)
std::vector<std::vector<int>> board(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0));
// プレイヤーターン (1=Player1, 2=Player2)
int currentPlayer = 1;
const char *TITLE = "Untitled_Dxlib_window";
char key = 0;

int putx = -1;
int puty = -1;
std::string rsv = "";

bool taiki = true;
bool isTrue = false;
int fremes = 0;

const std::vector<unsigned int> cellColors = {
	GetColor(255, 230, 230),
	GetColor(230, 255, 230),
	GetColor(230, 230, 255),
	GetColor(255, 255, 230)};

void start_dxlib(int WIN_WIDTH, int WIN_HEIGHT, const char *TITLE);


/// ret true 五目並べ成立
///		false game続行
bool Judge(int x, int y, std::vector<std::vector<int>> &board)
{
	int col = board[x][y];
	if (col == 0)
		return false;

	// 8方向のチェック用の配列
	const int dx[] = {1, 0, 1, -1}; // 縦、横、斜め（右下）、斜め（左下）
	const int dy[] = {0, 1, 1, 1};

	for (int dir = 0; dir < 4; dir++)
	{
		int count = 1; // 現在の位置を1からカウント開始

		// 正方向
		for (int i = 1; i < 5; i++)
		{
			int nx = x + dx[dir] * i;
			int ny = y + dy[dir] * i;

			if (nx < 0 || nx >= board.size() || ny < 0 || ny >= board[0].size() || board[nx][ny] != col)
			{
				break;
			}
			count++;
		}
		// 逆方向
		for (int i = 1; i < 5; i++)
		{
			int nx = x - dx[dir] * i;
			int ny = y - dy[dir] * i;

			if (nx < 0 || nx >= board.size() || ny < 0 || ny >= board[0].size() || board[nx][ny] != col)
			{
				break;
			}
			count++;
		}

		if (count >= 5)
		{
			return true;
		}
	}
	return false;
}

// ターン情報の描画
void DrawTurnInfo()
{
	std::string turnText = "Turn: Player " + std::to_string(currentPlayer);
	DrawString(600, 50, turnText.c_str(), GetColor(255, 255, 255));
}
// 盤面の描画
void DrawBoard(int ms_x, int ms_y)
{
	// 背景画像の幅と高さを取得
	int bgWidth, bgHeight;
	GetGraphSize(bgHandle, &bgWidth, &bgHeight);

	// 背景を画面中央に描画
	int bgX = (WIN_WIDTH - bgWidth) / 2;
	int bgY = ((WIN_HEIGHT - bgHeight) / 2) - 100;
	DrawGraph(bgX, bgY, bgHandle, TRUE);

	// 盤面の幅と高さを計算
	int boardWidth = BOARD_SIZE * CELL_SIZE;
	int boardHeight = BOARD_SIZE * CELL_SIZE;

	// 盤面の左上の描画開始位置を計算
	int startX = (WIN_WIDTH - boardWidth) / 2;
	int startY = ((WIN_HEIGHT - boardHeight) / 2) - 100;

	// 盤面を描画
	for (int y = 0; y < BOARD_SIZE; ++y)
	{
		for (int x = 0; x < BOARD_SIZE; ++x)
		{
			int drawX = startX + x * CELL_SIZE;
			int drawY = startY + y * CELL_SIZE;
			int drawX2 = drawX + CELL_SIZE; // 縮小後の右下X座標
			int drawY2 = drawY + CELL_SIZE; // 縮小後の右下Y座標

			// マスの状態に応じて画像を描画
			if (board[y][x] == 1)
			{
				DrawExtendGraph(drawX, drawY, drawX2, drawY2, siroHandle, TRUE);
			}
			else if (board[y][x] == 2)
			{
				DrawExtendGraph(drawX, drawY, drawX2, drawY2, kuroHandle, TRUE);
			}
			else
			{

				if (ms_x == x && ms_y == y)
				{
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					DrawExtendGraph(drawX, drawY, drawX2, drawY2, kuro2Handle, TRUE);
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
				}

				// DrawExtendGraph(drawX, drawY, drawX2, drawY2, blankHandle, TRUE);
			}
		}
	}
}

void DrawRefinedStringTable(int left, int top, int right, int bottom, const std::vector<std::vector<std::string>> &table, int fontHandle = -1)
{
	size_t rows = table.size();
	size_t cols = 0;
	/*for (const auto& row : table) {
		if (row.size() > cols) cols = row.size();
	}*/

	int tableWidth = right - left;
	int tableHeight = bottom - top;


	char choise = ' ';

	for (size_t row = 0; row < rows; ++row)
	{
		cols = table[row].size(); // 各行の列数を取得

		int columnWidth = tableWidth / (int)cols;
		int lineHeight = tableHeight / (int)rows;

		for (size_t col = 0; col < table[row].size(); ++col)
		{
			int cellX = left + col * columnWidth;
			int cellY = top + row * lineHeight;

			int bgColor = cellColors[(row + col) % cellColors.size()];
			int borderColor = GetColor(150, 150, 150); // 濃い目のグレー

			/// セル縁取り
			DrawBox(cellX - 3, cellY - 3, cellX + columnWidth + 3, cellY + lineHeight + 3, borderColor, true);
			/// セル背景
			DrawBox(cellX, cellY, cellX + columnWidth, cellY + lineHeight, bgColor, TRUE);

			/// 中央揃え＋影
			const std::string text = std::string(1,choise) + " " + table[row][col];
			
			int textWidth = GetDrawStringWidthToHandle(text.c_str(), text.size(), fontHandle) + 10;
			int textHeight = GetFontSize() + 10;

			if (text.find("\n") != -1)
			{
				textHeight *= 3;
				textHeight -= 10;
			}

			int centerX = cellX + (columnWidth - textWidth) / 2;
			int centerY = cellY + (lineHeight - textHeight) / 2;

			DrawStringToHandle(centerX + 2, centerY + 2, text.c_str(), GetColor(100, 100, 100), fontHandle);
			DrawStringToHandle(centerX, centerY, text.c_str(), GetColor(0, 0, 0), fontHandle);
			std::string tmp = "left:" + std::to_string(left) + " top:" + std::to_string(top) + " right:" + std::to_string(right) + " bottom:" + std::to_string(bottom);
			DrawStringToHandle(0, 0, tmp.c_str(), GetColor(0, 0, 0), fontHandle);
			if (choise==' ')
			{
				choise = 'A' - 1;
			}
		choise++;
		}
	}
}

// メイン関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// DxLib 初期化

	AllocConsole();
	FILE *stream;
	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w", stdout);

	start_dxlib(WIN_WIDTH, WIN_HEIGHT, TITLE);

	int fontHandle = CreateFontToHandle("UD デジタル 教科書体 N", 28, 6, DX_FONTTYPE_ANTIALIASING_8X8);
	int fontHandle2 = CreateFontToHandle("UD デジタル 教科書体 N", 100, 6, DX_FONTTYPE_ANTIALIASING_8X8);
	int left = 112, top = 714, right = 1168, bottom = 957;

	int effectfream = 0;
	int trueorfalseornone = 0;

	bool cric = false;
	

	while (ProcessMessage() == 0)
	{
		ClearDrawScreen();

		nw::rsvmsg();

		// ターン情報の描画
		DrawTurnInfo();

		// 入力処理
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// マウスクリックで盤面を更新
		// マウスクリックで盤面を更新
		int mouseX, mouseY;
		GetMousePoint(&mouseX, &mouseY);

		// 盤面の左上の描画開始位置を計算
		int boardWidth = BOARD_SIZE * CELL_SIZE;
		int boardHeight = BOARD_SIZE * CELL_SIZE;
		int startX = (WIN_WIDTH - boardWidth) / 2;
		int startY = ((WIN_HEIGHT - boardHeight) / 2) - 100;
		// マウス座標を盤面の座標に変換
		int gridX = (mouseX - startX) / CELL_SIZE;
		int gridY = (mouseY - startY) / CELL_SIZE;

		if (!(GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
		{
			cric = false;
		}

		// 盤面の範囲内か確認
		if (gridX >= 0 && gridX < BOARD_SIZE && gridY >= 0 && gridY < BOARD_SIZE&&put)
		{

			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && put&&cric==false)
			{
				/// おいてなかったら置く＆判定
				if (board[gridY][gridX] == 0)
				{
					cric = true;
					board[gridY][gridX] = currentPlayer;
					putx = gridX;
					puty = gridY;
					currentPlayer = (currentPlayer == 1) ? 2 : 1;

					if (Judge(gridY, gridX, board))
					{
						std::string turnText = "winner: Player " + std::to_string(currentPlayer);
						DrawString(300, 50, turnText.c_str(), GetColor(255, 255, 255));

						std::u8string winnerText = u8"の勝利:";
						if (currentPlayer==2)
						{
							winnerText += u8"白";
						}
						else {
							winnerText += u8"黒";
						}

						while (true)
						{

							// ターン情報の描画
							DrawTurnInfo();
							DrawBoard(gridX, gridY);

							DrawStringToHandle(150, 480, toU8(winnerText).c_str(), GetColor(20, 20, 20), fontHandle2, currentPlayer);

							// 入力処理
							if (CheckHitKey(KEY_INPUT_ESCAPE))
							{
								break;
							}

						}

					}

					nw::send('a');

					PlaySoundMem(SEputHandle, DX_PLAYTYPE_BACK);

					put = false;
				}
			}
		}else{
			gridX = -1;
			gridY = -1;
		}


		/*--------------------------多分問題の回答用↓-------------------------*/

		if (!put)///問題のとき
		{
			key = 0;

			if (CheckHitKey(KEY_INPUT_A) == 1)
			{
				key = 'a';
			}
			if (CheckHitKey(KEY_INPUT_B) == 1)
			{
				key = 'b';
			}
			if (CheckHitKey(KEY_INPUT_C) == 1)
			{
				key = 'c';
			}
			if (CheckHitKey(KEY_INPUT_D) == 1)
			{
				key = 'd';
			}
			if (key != 0)
			{
				nw::send(key); // 押されたキーコードを送信

			nw::rsvmsg();
			 {
				if (isTrue ) {
					std::cout << "正解の模様！おめでとう！" << std::endl;
					PlaySoundMem(SEtrueHandle, DX_PLAYTYPE_BACK);
					trueorfalseornone = 1;
					put = true;	
					effectfream = fremes;
				}
				else {
					std::cout << "ハズレの模様！" << std::endl;
					PlaySoundMem(SEfalseHandle, DX_PLAYTYPE_BACK);
					trueorfalseornone = 2;
					effectfream = fremes;
					currentPlayer = (currentPlayer == 1) ? 2 : 1;
				}
				// isTrue の値を使用
			}

			}



		}
		DrawExtendGraph(left, top, right, bottom, GEtrueHandle, TRUE);
		/*----------------------------------------------*/

		// 盤面の描画
		DrawBoard(gridX, gridY);


		int x1 = 0, y1 = 0, x2 = WIN_HEIGHT, y2 = WIN_WIDTH;

		///*auto question = nw::rsv_question();*/
		//std::vector<std::vector<std::string>> table = {
		//	{"aaaabbbbww"},
		//	{"",""},
		//	{"",""}
		//};

		auto question = nw::rsv_question();
		std::vector<std::vector<std::string>> table = {
			{toU8(question.at(0))},
			{toU8(question.at(1)), toU8(question.at(2))},
			{toU8(question.at(3)), toU8(question.at(4))} };

		if (put)
		{
			DrawRefinedStringTable(left, top, right, bottom, table, fontHandle);
			x1 = top;
			y1 = left;
			x2 = bottom;
			y2 = right;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox(y1, x1, y2, x2, GetColor(0, 0, 0), true);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}
		if (!put)
		{

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox(y1, x1, y2, x2, GetColor(0, 0, 0), true);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

			DrawRefinedStringTable(left, top, right, bottom, table, fontHandle);
		}
		std::string tmp = "key input:" + std::to_string(key);

		DrawStringToHandle(30, 20, tmp.c_str(), GetColor(200, 200, 200), fontHandle);


		if (trueorfalseornone != 0)
		{
			int handle = (trueorfalseornone == 1) ? GEtrueHandle : GEfalseHandle;
			if (effectfream >= fremes - 90)
			{

				DrawExtendGraph(452, bottom - 300, 752, bottom, handle, TRUE);

			}

		}


		ScreenFlip();
		fremes++;
	}
	nw::CustomSocketEnd();
	DxLib_End();
	return 0;
}

void start_dxlib(int WIN_WIDTH, int WIN_HEIGHT, const char *TITLE)
{
	std::cout << "#INFO# setting DxLib..." << std::endl;

	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	SetUseDirect3DVersion(DX_DIRECT3D_9);
	SetFullSceneAntiAliasingMode(16, 2);
	ChangeWindowMode(true);
	SetWindowSizeChangeEnableFlag(true, true);
	SetMainWindowText((const TCHAR *)TITLE);
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);
	SetWindowSizeExtendRate(1.0);
	SetBackgroundColor(0, 128, 128);
	SetDrawScreen(DX_SCREEN_BACK);
	ChangeWindowMode(true);
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);
	SetDoubleStartValidFlag(true);

	// stdoutとstdinをUTF-8に
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	std::ios::sync_with_stdio(false);
	std::cout.tie(nullptr);

	std::cout << "#INFO# DxLib setting..." << std::endl;
	if (DxLib_Init() == -1)
	{
		std::cout << "#WARNIN# DxLib init FAILED #WARNIN#";
	}
	// 画像の読み込み

	std::cout << "#INFO# Loading images..." << std::endl;
	bgHandle = LoadGraph("bg.png");
	siroHandle = LoadGraph("siro.png");
	kuroHandle = LoadGraph("kuro.png");
	blankHandle = LoadGraph("blank.png");
	kuro2Handle = LoadGraph("kuro2.png");
	SEputHandle = LoadSoundMem("put.wav");
	SEfalseHandle = LoadSoundMem("false.wav");///不正解
	SEtrueHandle = LoadSoundMem("true.wav");///(正解の音)
	GEfalseHandle = LoadGraph("fuseikai.png");
	GEtrueHandle = LoadGraph("seikai.png");

	std::cout << "#info# Trying to connect to server..." << std::endl;
	if (nw::CustomSocketInit(127, 0, 0,1) == -1)
	{
		std::cout << "#WARNIN# Socket init FAILED #WARNIN#";
		//exit(1);
	}
	else
	{
		std::cout << "#INFO# Socket init SUCCESS #INFO#";

		system("chcp 65001 > nul");
		
		std::cout << "aaaあああ" << std::endl;
		system("chcp 65001 > nul");

		nw::send_game_start();

		system("chcp 932 > nul");
		std::cout << "サーバーに送信したで" << std::endl;
	}

}
		// if (CheckHitKey(KEY_INPUT_LEFT) == 1) left -= 1;
		// if (CheckHitKey(KEY_INPUT_RIGHT) == 1) left += 1;
		// if (CheckHitKey(KEY_INPUT_UP) == 1) top -= 1;
		// if (CheckHitKey(KEY_INPUT_DOWN) == 1) top += 1;
		// if (CheckHitKey(KEY_INPUT_A) == 1) right -= 1;
		// if (CheckHitKey(KEY_INPUT_D) == 1) right += 1;
		// if (CheckHitKey(KEY_INPUT_W) == 1) bottom -= 1;
		// if (CheckHitKey(KEY_INPUT_S) == 1) bottom += 1;






