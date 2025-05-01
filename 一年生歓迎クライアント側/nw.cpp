#include "incl.h"


namespace nw
{
	int net_handle = 0;         // ネットワークハンドルの定義
	char rcv_buf[1024] = {0};   // 受信用バッファの定義
	std::string snd_buf="";   // 送信用バッファの定義

	int port = 12345;///ポート番号
	json j;
	
}

extern std::string rsv;



std::array<std::string, 5> nw::rsv_question() {
	return{ "Aを選んでください" ,"a 1913","b","c","d" };
}

/// <summary>
/// 問題の解答を送信する
/// ｒｙｏｔｔｉ１２１５よろしく
/// </summary>
/// <param name="answer">答え</param>
/// <returns></returns>
int nw::send(char answer) {

	using namespace std::string_literals;

	json mapArray = json::array();

	for (const auto& row : board) {
		std::string line;
		for (int cell : row) {
			switch (cell) {
			case 0: line += 'o'; break;  // 空白
			case 1: line += 'a'; break;  // 黒
			case 2: line += 'b'; break;  // 白
			default: line += 'o'; break; // その他も空白扱い
			}
		}
		mapArray.push_back(line);
	}

	j["map"] = mapArray;

	if (answer<='a'&&answer>='d'||1)
	{
		j["replyAns"] = char(answer);

	}
	
	j["coordinate"] = json::array();
	j["coordinate"] = { { putx,puty } };

	j["takenResTime"] = 3;///後で書く

	snd_buf = j.dump();

	NetWorkRecv(net_handle, rcv_buf, sizeof(rcv_buf));

	rsv = rcv_buf;
	std::cout << rsv << std::endl;
	std::cout << j.dump() << std::endl;

	// 受信データ量が0より大きい場合

	NetWorkSend(net_handle, snd_buf.c_str(), snd_buf.length());

	return 0;
}



/// <summary>
/// 盤面を書き換える
/// ｒｙｏｔｔｉ１２１５よろしく 
/// </summary>
/// <param name="board"></param>
/// <returns></returns>
int nw::update_board(std::vector<std::vector<int>>& board_old) {
	static std::vector<std::vector<int>> new_board = board_old;///新しい盤面
	std::vector<std::vector<int>> rsv_board;



	std::vector<std::vector<int>> board;
	if (!j.contains("map") || !j["map"].is_array()) {
		//return -1;  // 空の盤面を返す
	}

	for (const auto& rowJson : j["map"]) {
		std::vector<int> row;
		if (!rowJson.is_string()) continue;

		std::string line = rowJson.get<std::string>();
		for (char c : line) {
			switch (c) {
			case 'o': row.push_back(0); break;  // 空白
			case 'a': row.push_back(1); break;  // 黒
			case 'b': row.push_back(2); break;  // 白
			default:  row.push_back(0); break;  // その他も空白扱い
			}
		}
		board.push_back(row);
	}




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
		//return -1;
	}
	
	return 0;
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

		// サーバ側にデータ送信

		NetWorkSend(net_handle, snd_buf.c_str(), snd_buf.length());

	}

}

void nw::sndmsg() {
}

