#include "incl.h"


            #include <algorithm> // Ensure this header is included for std::min
#include <random>

extern bool put;
// ウィンドウサイズ
extern const int WIN_WIDTH;
extern const int WIN_HEIGHT;
// マス目のサイズ
extern const int CELL_SIZE;
// 盤面のサイズ
extern const int BOARD_SIZE;
// 画像ハンドル
extern int bgHandle, siroHandle, kuroHandle, blankHandle, kuro2Handle;///マウスオーバー時の画像
// 盤面データ (1=白, 2=黒, 0=空白)
extern std::vector<std::vector<int>> board;
// プレイヤーターン (1=Player1, 2=Player2)
extern int currentPlayer;
extern const char* TITLE;
extern char key;
extern const std::vector<unsigned int> cellColors;

extern int putx;
extern int puty;
extern std::string rsv;
extern bool taiki;


extern bool isTrue;
extern int putx;
extern int puty;
extern std::vector<std::vector<int>> board;
extern std::string rsv;//=std::string_literals::R("0iu");
extern int fremes;



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

extern bool isTrue;
extern int putx;
extern int puty;
extern std::vector<std::vector<int>> board;
extern std::string rsv;//=std::string_literals::R("0iu");
std::array<std::u8string, 6> result;


/*追加部分*/
void nw::send_game_start() {
	nlohmann::json j1 = {
		{"go","yes"}
	};

	snd_buf = j1.dump() + '\b';

	NetWorkSend(net_handle, snd_buf.c_str(), snd_buf.length());
	
}

std::string nw::get_game_status() {
	NetWorkRecv(net_handle, rcv_buf, sizeof(rcv_buf));
	rsv = rcv_buf;

	try {
		nlohmann::json j1 = nlohmann::json::parse(rsv);
		std::string status = j1["gameStatus"];
		return status;
	}
	catch (const std::exception& ex) {
		std::cerr << "ERROR : " << ex.what() << std::endl;
		return "";
	}


}
/*---------------------------------*/
std::array<std::u8string, 6> nw::rsv_question()
{
using namespace std::string_literals;
	int a = nw::send('a', "messagePleaseを含む文字"s);
	rsvmsg();

	if (j["choices"].is_array())
	{

		// 質問文を配列の最初に設定
		result[0] = j["question"].get<std::u8string>();

		// 選択肢を配列に設定
		auto &choices = j["choices"];


            // Fix the std::min usage by explicitly casting the arguments to matching types
            for (int i = 0; i < std::min(static_cast<size_t>(choices.size()), size_t(4)); ++i)
            
		{
			result[i + 1] = choices[i].get<std::u8string>();
		}

		return result;
	}

	// JSONデータが不正な場合のデフォルト値
	return {u8"問題を取得できませんでした", u8"A ", u8"B ", u8"C ", u8"D ",u8"A "};
}

/// <summary>
/// 問題の解答を送信する
/// ｒｙｏｔｔｉ１２１５よろしく
/// </summary>
/// <param name="answer">答え</param>
/// <returns></returns>
int nw::send(char answer,std::string str)
{
	using namespace std::string_literals;

	json empty;

	j = empty;

	int ans = answer - 'a';
	ans=abs(ans);

	ans++;

	std::array<std::u8string, 6> question;// = nw::rsv_question();


	try
	{
		// 受信したJSONデータ（rsv）をパース
		if (!rsv.empty())
		{
			j = json::parse(rsv);
		}

		// 以下、既存のコード
		json mapArray = json::array();

		

		try
		{
			
			j["replyAns"] = std::string(1, static_cast<char>(std::toupper(answer)));


		}
		catch (const std::exception&)
		{

		}

		if (question[ans]==question[5])
		{
			isTrue = true;
		}
		else
		{
			isTrue = false;
		}




		j["coordinate"] = json::array();
		j["coordinate"] = {{putx, puty}};

		j["takenResTime"] = 3; /// 後で書く

		snd_buf = j.dump()+"\n";
		std::cout << std::endl;
		std::cout << snd_buf << std::endl;

		///rsv_buf init
		rcv_buf[0] = 0;

		NetWorkRecv(net_handle, rcv_buf, sizeof(rcv_buf));

		rsv = rcv_buf;
		std::cout << rsv << std::endl;
		//std::cout << j.dump() << std::endl;

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

std::string nw::utf8(std::u8string str) {
	return reinterpret_cast<const char*>(str.c_str());
}

void nw::rsvmsg()
{
	NetWorkRecv(net_handle, rcv_buf, sizeof(rcv_buf));
	rsv = rcv_buf;

	if (rsv.empty())
	{
		std::cerr << "ERROR: rsv is empty" << std::endl;
		return;
	}
	try
	{
		j = json::parse(rsv);
	}
	catch (const json::parse_error& e)
	{
		std::cerr << "JSON parse error: " << e.what() << std::endl;
	}
}

/*

log

{"coordinate":[[-1,-1]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[8,10]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[7,9]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooobooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}





*/
