#pragma once

#include"incl.h"

namespace nw
{
	using namespace nlohmann;


	extern int  net_handle;      // ネットワークハンドル
	extern char rcv_buf[1024];   // 受信用バッファ
	extern std::string snd_buf;   // 送信用バッファ
	extern int port;///ポート番号
	extern json j;

	extern std::array<std::string, 6> rsv_question();

	/*追加関数*/
	void send_game_start();
	std::string get_game_status();
	/**-----------**/

	/// <summary>
/// 問題の解答を送信する
/// ｒｙｏｔｔｉ１２１５よろしく
/// </summary>
/// <param name="answer">答え</param>
/// <returns></returns>
	int send(char answer);

/// <summary>
/// 盤面を書き換える
/// ｒｙｏｔｔｉ１２１５よろしく 
/// </summary>
/// <param name="board"></param>
/// <returns></returns>

int CustomSocketInit(int8_t IP1, int8_t IP2, int8_t IP3, int8_t IP4);

void CustomSocketEnd();

std::string utf8(std::u8string str);

void rsvmsg();

}






