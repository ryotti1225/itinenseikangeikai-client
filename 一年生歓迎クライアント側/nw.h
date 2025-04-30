#pragma once

namespace nw
{

	extern int  net_handle;      // ネットワークハンドル
	extern char rcv_buf[1024];   // 受信用バッファ
	extern char snd_buf[1024];   // 送信用バッファ

	std::array<std::string, 5> rsv_question();

	/// <summary>
/// 問題の解答を送信する
/// ｒｙｏｔｔｉ１２１５よろしく
/// </summary>
/// <param name="answer">答え</param>
/// <returns></returns>
	int send_answer(char answer);

/// <summary>
/// 盤面を書き換える
/// ｒｙｏｔｔｉ１２１５よろしく 
/// </summary>
/// <param name="board"></param>
/// <returns></returns>
int update_board(std::vector<std::vector<int>>& board);




}


