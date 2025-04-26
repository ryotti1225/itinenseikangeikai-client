#include "Dxlib/DxLib.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include<array>

bool put = true;

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


std::array<std::string,5> rsv_question(){
	return{ "A��I��ł�������" ,"a","b","c","d" };
}

/// <summary>
/// ���̉𓚂𑗐M����
/// �������������P�Q�P�T��낵��
/// </summary>
/// <param name="answer">����</param>
/// <returns></returns>
int send_answer(char answer) {
	return 0;
}

/// <summary>
/// �Ֆʂ�����������
/// �������������P�Q�P�T��낵�� 
/// </summary>
/// <param name="board"></param>
/// <returns></returns>
int update_board(std::vector<std::vector<int>>& board) {
	static std::vector<std::vector<int>> new_board = board;///�V�����Ֆ�
	std::vector<std::vector<int>> rsv_board;



	///�ꎞ�I�ɍ��̔Ֆʂ����̂܂܎����Ƃ�
	new_board = board;
	///�Ֆʂ̎󂯎��̑���



	if (!rsv_board.empty()){
		new_board = rsv_board;
	}
	board = new_board;
	return 0;
}

// �E�B���h�E�T�C�Y
const int WIN_WIDTH = 1280;
const int WIN_HEIGHT = 960;

// �}�X�ڂ̃T�C�Y
const int CELL_SIZE = 33;

// �Ֆʂ̃T�C�Y
const int BOARD_SIZE = 19;

// �摜�n���h��
int bgHandle, siroHandle, kuroHandle, blankHandle, kuro2Handle;///�}�E�X�I�[�o�[���̉摜

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
	kuro2Handle = LoadGraph("kuro2.png");
}

// �^�[�����̕`��
void DrawTurnInfo() {
	std::string turnText = "Turn: Player " + std::to_string(currentPlayer);
	DrawString(600, 50, turnText.c_str(), GetColor(255, 255, 255));
}
// �Ֆʂ̕`��
void DrawBoard(int ms_x,int ms_y) {
	// �w�i�摜�̕��ƍ������擾
	int bgWidth, bgHeight;
	GetGraphSize(bgHandle, &bgWidth, &bgHeight);

	// �w�i����ʒ����ɕ`��
	int bgX = (WIN_WIDTH - bgWidth) / 2;
	int bgY = ((WIN_HEIGHT - bgHeight) / 2)-100;
	DrawGraph(bgX, bgY, bgHandle, TRUE);

	// �Ֆʂ̕��ƍ������v�Z
	int boardWidth = BOARD_SIZE * CELL_SIZE;
	int boardHeight = BOARD_SIZE * CELL_SIZE;

	// �Ֆʂ̍���̕`��J�n�ʒu���v�Z
	int startX = (WIN_WIDTH - boardWidth) / 2;
	int startY =  ((WIN_HEIGHT - boardHeight) / 2)-100;

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
			}else if (board[y][x] == 2) {
				DrawExtendGraph(drawX, drawY, drawX2, drawY2, kuroHandle, TRUE);
			}else {

				if ( ms_x==x&&ms_y==y)
				{
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
					DrawExtendGraph(drawX, drawY, drawX2, drawY2, kuro2Handle, TRUE);
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
				}

				//DrawExtendGraph(drawX, drawY, drawX2, drawY2, blankHandle, TRUE);
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

/// �}�E�X�I�[�o�[���̃n�C���C�g�F
const unsigned int hoverColor = GetColor(255, 255, 255);

void DrawRefinedStringTable(int left, int top, int right, int bottom, const std::vector<std::vector<std::string>>& table, int fontHandle = -1) {
	size_t rows = table.size();
	size_t cols = 0;
	/*for (const auto& row : table) {
		if (row.size() > cols) cols = row.size();
	}*/

	int tableWidth = right - left;
	int tableHeight = bottom - top;
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	for (size_t row = 0; row < rows; ++row) {
		cols = table[row].size(); // �e�s�̗񐔂��擾

	int columnWidth = tableWidth / (int)cols;
	int lineHeight = tableHeight / (int)rows;


		for (size_t col = 0; col < table[row].size(); ++col) {
			int cellX = left + col * columnWidth;
			int cellY = top + row * lineHeight;

			bool isHover = mouseX >= cellX && mouseX < cellX + columnWidth &&
				mouseY >= cellY && mouseY < cellY + lineHeight;

			int bgColor = isHover ? GetColor(255, 255, 255) : cellColors[(row + col) % cellColors.size()];
			int borderColor = GetColor(150, 150, 150);  // �Z���ڂ̃O���[

			/// �Z�������
			DrawBox(cellX-3, cellY-3, cellX + columnWidth+3, cellY + lineHeight+3, borderColor, true);
			/// �Z���w�i
			DrawBox(cellX, cellY, cellX + columnWidth, cellY + lineHeight, bgColor, TRUE);
			
			/// ���������{�e
			const std::string& text = table[row][col];
			int textWidth = GetDrawStringWidthToHandle(text.c_str(), text.size(), fontHandle)+10;
			int textHeight = GetFontSize()+10;

			if (text.find("\n")!=-1)
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





	int fontHandle = CreateFontToHandle("UD �f�W�^�� ���ȏ��� N", 28, 6, DX_FONTTYPE_ANTIALIASING_8X8);
	int left = 112, top = 714, right = 1168, bottom = 957;

	
	
	
	while (ProcessMessage() == 0) {
		ClearDrawScreen();


		// �^�[�����̕`��
		DrawTurnInfo();

		// ���͏���
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}

		// �}�E�X�N���b�N�ŔՖʂ��X�V
		// �}�E�X�N���b�N�ŔՖʂ��X�V
			int mouseX, mouseY;
			GetMousePoint(&mouseX, &mouseY);

			// �Ֆʂ̍���̕`��J�n�ʒu���v�Z
			int boardWidth = BOARD_SIZE * CELL_SIZE;
			int boardHeight = BOARD_SIZE * CELL_SIZE;
			int startX = (WIN_WIDTH - boardWidth) / 2;
			int startY = ((WIN_HEIGHT - boardHeight) / 2) - 100;
			// �}�E�X���W��Ֆʂ̍��W�ɕϊ�
			int gridX = (mouseX - startX) / CELL_SIZE;
			int gridY = (mouseY - startY) / CELL_SIZE;

			// �Ֆʂ͈͓̔����m�F
			if (gridX >= 0 && gridX < BOARD_SIZE && gridY >= 0 && gridY < BOARD_SIZE) {





			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0&&) {
			///�����ĂȂ�������u��������
			if (board[gridY][gridX] == 0) {
				board[gridY][gridX] = currentPlayer;
				currentPlayer = (currentPlayer == 1) ? 2 : 1;

				if (Judge(gridY, gridX, board))
				{
					std::string turnText = "winner: Player " + std::to_string(currentPlayer);
					DrawString(300, 50, turnText.c_str(), GetColor(255, 255, 255));

				}
				put = false;
			}
		}
			}
			else {
				gridX = -1;
				gridY = -1;
			}
		// �Ֆʂ̕`��
		DrawBoard(gridX,gridY);

		//if (CheckHitKey(KEY_INPUT_LEFT) == 1) left -= 1;
		//if (CheckHitKey(KEY_INPUT_RIGHT) == 1) left += 1;
		//if (CheckHitKey(KEY_INPUT_UP) == 1) top -= 1;
		//if (CheckHitKey(KEY_INPUT_DOWN) == 1) top += 1;
		//if (CheckHitKey(KEY_INPUT_A) == 1) right -= 1;
		//if (CheckHitKey(KEY_INPUT_D) == 1) right += 1;
		//if (CheckHitKey(KEY_INPUT_W) == 1) bottom -= 1;
		//if (CheckHitKey(KEY_INPUT_S) == 1) bottom += 1;

		if (!put)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox(0, 0, WIN_WIDTH, WIN_HEIGHT, GetColor(0, 0, 0), true);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}

		auto question = rsv_question();
	std::vector<std::vector<std::string>> table = {
		{question.at(0)},
		{question.at(1), question.at(2)},
		{question.at(3), question.at(4)}
	};


		DrawRefinedStringTable(left, top, right, bottom, table, fontHandle);




		ScreenFlip();
	}

	DxLib_End();
	return 0;
}
