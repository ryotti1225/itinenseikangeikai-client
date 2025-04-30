#include "incl.h"


namespace nw
{
    int net_handle = 0;         // ネットワークハンドルの定義
    char rcv_buf[1024] = {0};   // 受信用バッファの定義
    char snd_buf[1024] = {0};   // 送信用バッファの定義
}




std::array<std::string, 5> nw::rsv_question() {
	return{ "Aを選んでください" ,"a 1913","b","c","d" };
}

/// <summary>
/// 問題の解答を送信する
/// ｒｙｏｔｔｉ１２１５よろしく
/// </summary>
/// <param name="answer">答え</param>
/// <returns></returns>
int nw::send_answer(char answer) {

	using namespace std::string_literals;

	std::string answer_str =R"{"" + std::string(1, answer);///charをstringに変換

	return 0;
}



/// <summary>
/// 盤面を書き換える
/// ｒｙｏｔｔｉ１２１５よろしく 
/// </summary>
/// <param name="board"></param>
/// <returns></returns>
int nw::update_board(std::vector<std::vector<int>>& board) {
	static std::vector<std::vector<int>> new_board = board;///新しい盤面
	std::vector<std::vector<int>> rsv_board;



	///一時的に今の盤面をそのまま持っとく
	new_board = board;
	///盤面の受け取りの代わり



	if (!rsv_board.empty()) {
		new_board = rsv_board;
	}
	board = new_board;
	return 0;
}






