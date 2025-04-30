#include "incl.h"


namespace nw
{
    int net_handle = 0;         // ネットワークハンドルの定義
    char rcv_buf[1024] = {0};   // 受信用バッファの定義
    char snd_buf[1024] = {0};   // 送信用バッファの定義

	int port = 11600;///ポート番号
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

    std::string answer_str = R"({"answer": ")"s + std::string(1, answer) + R"("})";

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

int nw::CustomSocketInit(int8_t IP1, int8_t IP2, int8_t IP3, int8_t IP4 ) {

	// 変数定義
	IPDATA ip;                   // 接続用IPアドレスデータ

	// DXライブ独自のsocket仕様の機能を使用しない
	SetUseDXNetWorkProtocol(FALSE);

	// IPアドレスを設定
	ip.d1 = IP1;
	ip.d2 = IP2;
	ip.d3 = IP3;
	ip.d4 = IP4;

	// 通信を確立
	net_handle = ConnectNetWork(ip, port);

	// ハンドルの正常性確認
	if (net_handle == -1) {
		// 異常終了
		return -1;
	}
	else {
		// データ送信
		sprintf_s(snd_buf, "接続要求\n");
		NetWorkSend(net_handle, snd_buf, lstrlen(snd_buf));
		// 正常終了
		return 0;
	}

}


void nw::CustomSocketEnd() {
	CloseNetWork(net_handle);
}


void nw::rsvmsg() {

	// 変数定義
	int data_size;               // 受信データ時のデータサイズ

	// 取得していない受信データ量を得る
	data_size = GetNetWorkDataLength(net_handle);

	// 受信データ量が0より大きい場合
	if (data_size > 0) {

		// バッファクリア
		memset(rcv_buf, 0, sizeof(rcv_buf));

		// 受信したデータをバッファに取得
		NetWorkRecv(net_handle, rcv_buf, sizeof(rcv_buf));

		// サーバ側にデータ送信

		NetWorkSend(net_handle, snd_buf, lstrlen(snd_buf));

	}

}

void nw::sndmsg() {
	// 変数定義
	int data_size;               // 受信データ時のデータサイズ
	// 取得していない受信データ量を得る
	data_size = GetNetWorkDataLength(net_handle);
	// 受信データ量が0より大きい場合
	if (data_size > 0) {
		NetWorkSend(net_handle, snd_buf, lstrlen(snd_buf));
	}
}

