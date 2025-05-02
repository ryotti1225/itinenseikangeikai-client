#include "incl.h"

namespace nw
{
	int net_handle = 0;		  // ネットワークハンドルの定義
	char rcv_buf[1024] = {0}; // 受信用バッファの定義
	std::string snd_buf = ""; // 送信用バッファの定義

	int port = 12345; /// ポート番号
	json j;

}


/// <summary>
/// isTrue=問題の正誤
/// true正解
/// </summary>

extern std::string rsv;//=std::string_literals::R("0iu");
		std::array<std::string, 5> result;

std::array<std::string, 5> nw::rsv_question()
{
	if (j.contains("question") && j["question"].contains("question") &&
		j["question"].contains("choices") && j["question"]["choices"].is_array())
	{

		// 質問文を配列の最初に設定
		result[0] = j["question"]["question"].get<std::string>();

		char choise = 'A';
		// 選択肢を配列に設定
		auto &choices = j["question"]["choices"];
		for (int i = 0; i < std::min(choices.size(), size_t(4)); ++i)
		{
			result[i + 1] = choise + " " + choices[i].get<std::string>();
			choise++;
		}

		return result;
	}

	// JSONデータが不正な場合のデフォルト値
	return {"問題を取得できませんでした", "A ", "B ", "C ", "D "};
}

/// <summary>
/// 問題の解答を送信する
/// ｒｙｏｔｔｉ１２１５よろしく
/// </summary>
/// <param name="answer">答え</param>
/// <returns></returns>
int nw::send(char answer)
{
	using namespace std::string_literals;

	json empty;

	j = empty;

	try
	{
		// 受信したJSONデータ（rsv）をパース
		if (!rsv.empty())
		{
			j = json::parse(rsv);
		}

		// 以下、既存のコード
		json mapArray = json::array();

		for (const auto &row : board)
		{
			std::string line;
			for (int cell : row)
			{
				switch (cell)
				{
				case 0:
					line += 'o';
					break; // 空白
				case 1:
					line += 'a';
					break; // 黒
				case 2:
					line += 'b';
					break; // 白
				default:
					line += 'o';
					break; // その他も空白扱い
				}
			}
			mapArray.push_back(line);
		}

		j["map"] = mapArray;

		try
		{
			
			j["replyAns"] = result[answer - 'a' + 1];


		}
		catch (const std::exception&)
		{

		}

		j["coordinate"] = json::array();
		j["coordinate"] = {{putx, puty}};

		j["takenResTime"] = 3; /// 後で書く

		snd_buf = j.dump()+"\0";

		///rsv_buf init
		rcv_buf[0] = 0;

		NetWorkRecv(net_handle, rcv_buf, sizeof(rcv_buf));

		rsv = rcv_buf;
		std::cout << rsv << std::endl;
		//std::cout << j.dump() << std::endl;

		update_board(board);

		NetWorkSend(net_handle, snd_buf.c_str(), snd_buf.length());

		return 0;
	}
	catch (const json::parse_error &e)
	{
		std::cerr << "JSON parse error: " << e.what() << std::endl;
		return -1;
	}
}

/// <summary>
/// 盤面を書き換える
/// ｒｙｏｔｔｉ１２１５よろしく
/// </summary>
/// <param name="board"></param>
/// <returns></returns>
int nw::update_board(std::vector<std::vector<int>> &board_old)
{
	static std::vector<std::vector<int>> new_board = board_old; /// 新しい盤面

	std::vector<std::vector<int>> board;
	if (!j.contains("map") || !j["map"].is_array())
	{
		return -1; // 空の盤面を返す
	}

	for (const auto &rowJson : j["map"])
	{
		std::vector<int> row;
		if (!rowJson.is_string())
			continue;

		std::string line = rowJson.get<std::string>();
		for (char c : line)
		{
			switch (c)
			{
			case 'o':
				row.push_back(0);
				break; // 空白
			case 'a':
				row.push_back(1);
				break; // 黒
			case 'b':
				row.push_back(2);
				break; // 白
			default:
				row.push_back(0);
				break; // その他も空白扱い
			}
		}
		board.push_back(row);
	}

	board_old = board;

	return 0;
}

int nw::CustomSocketInit(int8_t IP1, int8_t IP2, int8_t IP3, int8_t IP4)
{

	// 変数定義
	IPDATA ip; // 接続用IPアドレスデータ

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
	if (net_handle == -1)
	{
		// 異常終了
		return -1;
	}

	return 0;
}

void nw::CustomSocketEnd()
{
	CloseNetWork(net_handle);
}

/*

log

{"coordinate":[[-1,-1]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[8,10]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[7,9]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooobooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}





*/
