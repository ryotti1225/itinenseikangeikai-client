#include "incl.h"


namespace nw
{
	int net_handle = 0;         // �l�b�g���[�N�n���h���̒�`
	char rcv_buf[1024] = {0};   // ��M�p�o�b�t�@�̒�`
	std::string snd_buf="";   // ���M�p�o�b�t�@�̒�`

	int port = 12345;///�|�[�g�ԍ�
	json j;
	
}

extern std::string rsv;



std::array<std::string, 5> nw::rsv_question() {
	return{ "A��I��ł�������" ,"a 1913","b","c","d" };
}

/// <summary>
/// ���̉𓚂𑗐M����
/// �������������P�Q�P�T��낵��
/// </summary>
/// <param name="answer">����</param>
/// <returns></returns>
int nw::send(char answer) {

	using namespace std::string_literals;

	json mapArray = json::array();

	for (const auto& row : board) {
		std::string line;
		for (int cell : row) {
			switch (cell) {
			case 0: line += 'o'; break;  // ��
			case 1: line += 'a'; break;  // ��
			case 2: line += 'b'; break;  // ��
			default: line += 'o'; break; // ���̑����󔒈���
			}
		}
		mapArray.push_back(line);
	}

	j["map"] = mapArray;

	if (answer<='a'&&answer>='d'||1)
	{
		j["replyAns"] = std::string(1, char(answer));

	}
	
	j["coordinate"] = json::array();
	j["coordinate"] = { { putx,puty } };

	j["takenResTime"] = 3;///��ŏ���

	snd_buf = j.dump();

	NetWorkRecv(net_handle, rcv_buf, sizeof(rcv_buf));

	rsv = rcv_buf;
	std::cout << rsv << std::endl;
	std::cout << j.dump() << std::endl;

	// ��M�f�[�^�ʂ�0���傫���ꍇ

	NetWorkSend(net_handle, snd_buf.c_str(), snd_buf.length());

	return 0;
}



/// <summary>
/// �Ֆʂ�����������
/// �������������P�Q�P�T��낵�� 
/// </summary>
/// <param name="board"></param>
/// <returns></returns>
int nw::update_board(std::vector<std::vector<int>>& board_old) {
	static std::vector<std::vector<int>> new_board = board_old;///�V�����Ֆ�
	std::vector<std::vector<int>> rsv_board;



	std::vector<std::vector<int>> board;
	if (!j.contains("map") || !j["map"].is_array()) {
		//return -1;  // ��̔Ֆʂ�Ԃ�
	}

	for (const auto& rowJson : j["map"]) {
		std::vector<int> row;
		if (!rowJson.is_string()) continue;

		std::string line = rowJson.get<std::string>();
		for (char c : line) {
			switch (c) {
			case 'o': row.push_back(0); break;  // ��
			case 'a': row.push_back(1); break;  // ��
			case 'b': row.push_back(2); break;  // ��
			default:  row.push_back(0); break;  // ���̑����󔒈���
			}
		}
		board.push_back(row);
	}




	///�ꎞ�I�ɍ��̔Ֆʂ����̂܂܎����Ƃ�
	new_board = board;
	///�Ֆʂ̎󂯎��̑���



	if (!rsv_board.empty()) {
		new_board = rsv_board;
	}
	board = new_board;
	return 0;
}

int nw::CustomSocketInit(int8_t IP1, int8_t IP2, int8_t IP3, int8_t IP4 ) {

	// �ϐ���`
	IPDATA ip;                   // �ڑ��pIP�A�h���X�f�[�^

	// DX���C�u�Ǝ���socket�d�l�̋@�\���g�p���Ȃ�
	SetUseDXNetWorkProtocol(FALSE);

	// IP�A�h���X��ݒ�
	ip.d1 = IP1;
	ip.d2 = IP2;
	ip.d3 = IP3;
	ip.d4 = IP4;

	// �ʐM���m��
	net_handle = ConnectNetWork(ip, port);

	// �n���h���̐��퐫�m�F
	if (net_handle == -1) {
		// �ُ�I��
		//return -1;
	}
	
	return 0;
}


void nw::CustomSocketEnd() {
	CloseNetWork(net_handle);
}


void nw::rsvmsg() {

	// �ϐ���`
	int data_size;               // ��M�f�[�^���̃f�[�^�T�C�Y

	// �擾���Ă��Ȃ���M�f�[�^�ʂ𓾂�
	data_size = GetNetWorkDataLength(net_handle);

	// ��M�f�[�^�ʂ�0���傫���ꍇ
	if (data_size > 0) {

		// �o�b�t�@�N���A
		memset(rcv_buf, 0, sizeof(rcv_buf));

		// ��M�����f�[�^���o�b�t�@�Ɏ擾

		// �T�[�o���Ƀf�[�^���M

		NetWorkSend(net_handle, snd_buf.c_str(), snd_buf.length());

	}

}

void nw::sndmsg() {
}


/*

log

{"coordinate":[[-1,-1]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[8,10]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[7,9]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooobooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[6,9]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooabooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[6,8]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooboooooooooooo","ooooooabooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[5,8]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooaboooooooooooo","ooooooabooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[5,7]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooobooooooooooooo","oooooaboooooooooooo","ooooooabooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[7,6]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooaooooooooooo","ooooobooooooooooooo","oooooaboooooooooooo","ooooooabooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[8,5]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooboooooooooo","oooooooaooooooooooo","ooooobooooooooooooo","oooooaboooooooooooo","ooooooabooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[9,5]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooooooooooo","ooooobooooooooooooo","oooooaboooooooooooo","ooooooabooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[10,6]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","ooooobooooooooooooo","oooooaboooooooooooo","ooooooabooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[11,7]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooooboooooaooooooo","oooooaboooooooooooo","ooooooabooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[12,8]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooooboooooaooooooo","oooooaboooooboooooo","ooooooabooooooooooo","ooooooooaoooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[12,10]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooooboooooaooooooo","oooooaboooooboooooo","ooooooabooooooooooo","ooooooooaoooaoooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[12,11]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooooboooooaooooooo","oooooaboooooboooooo","ooooooabooooooooooo","ooooooooaoooaoooooo","ooooooooooooboooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[11,12]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooooboooooaooooooo","oooooaboooooboooooo","ooooooabooooooooooo","ooooooooaoooaoooooo","ooooooooooooboooooo","oooooooooooaooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[11,13]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooooboooooaooooooo","oooooaboooooboooooo","ooooooabooooooooooo","ooooooooaoooaoooooo","ooooooooooooboooooo","oooooooooooaooooooo","ooooooooooobooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[10,14]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooooboooooaooooooo","oooooaboooooboooooo","ooooooabooooooooooo","ooooooooaoooaoooooo","ooooooooooooboooooo","oooooooooooaooooooo","ooooooooooobooooooo","ooooooooooaoooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[7,15]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooooboooooaooooooo","oooooaboooooboooooo","ooooooabooooooooooo","ooooooooaoooaoooooo","ooooooooooooboooooo","oooooooooooaooooooo","ooooooooooobooooooo","ooooooooooaoooooooo","ooooooobooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[4,15]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooooboooooaooooooo","oooooaboooooboooooo","ooooooabooooooooooo","ooooooooaoooaoooooo","ooooooooooooboooooo","oooooooooooaooooooo","ooooooooooobooooooo","ooooooooooaoooooooo","ooooaoobooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[2,14]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooooboooooaooooooo","oooooaboooooboooooo","ooooooabooooooooooo","ooooooooaoooaoooooo","ooooooooooooboooooo","oooooooooooaooooooo","ooooooooooobooooooo","ooboooooooaoooooooo","ooooaoobooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[0,12]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooooboooooaooooooo","oooooaboooooboooooo","ooooooabooooooooooo","ooooooooaoooaoooooo","ooooooooooooboooooo","aooooooooooaooooooo","ooooooooooobooooooo","ooboooooooaoooooooo","ooooaoobooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[0,11]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooooboooooaooooooo","oooooaboooooboooooo","ooooooabooooooooooo","ooooooooaoooaoooooo","boooooooooooboooooo","aooooooooooaooooooo","ooooooooooobooooooo","ooboooooooaoooooooo","ooooaoobooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[2,9]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooooboooooaooooooo","oooooaboooooboooooo","ooaoooabooooooooooo","ooooooooaoooaoooooo","boooooooooooboooooo","aooooooooooaooooooo","ooooooooooobooooooo","ooboooooooaoooooooo","ooooaoobooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[4,7]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobboooooaooooooo","oooooaboooooboooooo","ooaoooabooooooooooo","ooooooooaoooaoooooo","boooooooooooboooooo","aooooooooooaooooooo","ooooooooooobooooooo","ooboooooooaoooooooo","ooooaoobooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[10,7]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoooabooooooooooo","ooooooooaoooaoooooo","boooooooooooboooooo","aooooooooooaooooooo","ooooooooooobooooooo","ooboooooooaoooooooo","ooooaoobooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[13,10]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoooabooooooooooo","ooooooooaoooabooooo","boooooooooooboooooo","aooooooooooaooooooo","ooooooooooobooooooo","ooboooooooaoooooooo","ooooaoobooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[14,12]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoooabooooooooooo","ooooooooaoooabooooo","boooooooooooboooooo","aooooooooooaooaoooo","ooooooooooobooooooo","ooboooooooaoooooooo","ooooaoobooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[13,13]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoooabooooooooooo","ooooooooaoooabooooo","boooooooooooboooooo","aooooooooooaooaoooo","ooooooooooobobooooo","ooboooooooaoooooooo","ooooaoobooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[11,15]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoooabooooooooooo","ooooooooaoooabooooo","boooooooooooboooooo","aooooooooooaooaoooo","ooooooooooobobooooo","ooboooooooaoooooooo","ooooaooboooaooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[6,16]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoooabooooooooooo","ooooooooaoooabooooo","boooooooooooboooooo","aooooooooooaooaoooo","ooooooooooobobooooo","ooboooooooaoooooooo","ooooaooboooaooooooo","ooooooboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[4,16]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoooabooooooooooo","ooooooooaoooabooooo","boooooooooooboooooo","aooooooooooaooaoooo","ooooooooooobobooooo","ooboooooooaoooooooo","ooooaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[2,15]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoooabooooooooooo","ooooooooaoooabooooo","boooooooooooboooooo","aooooooooooaooaoooo","ooooooooooobobooooo","ooboooooooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[0,13]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoooabooooooooooo","ooooooooaoooabooooo","boooooooooooboooooo","aooooooooooaooaoooo","aoooooooooobobooooo","ooboooooooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[1,12]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoooabooooooooooo","ooooooooaoooabooooo","boooooooooooboooooo","aboooooooooaooaoooo","aoooooooooobobooooo","ooboooooooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[2,11]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoooabooooooooooo","ooooooooaoooabooooo","boaoooooooooboooooo","aboooooooooaooaoooo","aoooooooooobobooooo","ooboooooooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[5,9]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoobabooooooooooo","ooooooooaoooabooooo","boaoooooooooboooooo","aboooooooooaooaoooo","aoooooooooobobooooo","ooboooooooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[11,10]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoobabooooooooooo","ooooooooaooaabooooo","boaoooooooooboooooo","aboooooooooaooaoooo","aoooooooooobobooooo","ooboooooooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[13,12]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoobabooooooooooo","ooooooooaooaabooooo","boaoooooooooboooooo","aboooooooooaobaoooo","aoooooooooobobooooo","ooboooooooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[7,14]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoobabooooooooooo","ooooooooaooaabooooo","boaoooooooooboooooo","aboooooooooaobaoooo","aoooooooooobobooooo","oobooooaooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[5,14]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoobabooooooooooo","ooooooooaooaabooooo","boaoooooooooboooooo","aboooooooooaobaoooo","aoooooooooobobooooo","oobooboaooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[3,12]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoobabooooooooooo","ooooooooaooaabooooo","boaoooooooooboooooo","aboaoooooooaobaoooo","aoooooooooobobooooo","oobooboaooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[3,11]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoobabooooooooooo","ooooooooaooaabooooo","boabooooooooboooooo","aboaoooooooaobaoooo","aoooooooooobobooooo","oobooboaooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[4,9]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooooooo","oooooaboooooboooooo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooooo","aboaoooooooaobaoooo","aoooooooooobobooooo","oobooboaooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[14,7]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooboooo","oooooaboooooboooooo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooooo","aboaoooooooaobaoooo","aoooooooooobobooooo","oobooboaooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[16,8]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooooo","aboaoooooooaobaoooo","aoooooooooobobooooo","oobooboaooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[16,11]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaoooo","aoooooooooobobooooo","oobooboaooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[15,12]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","ooboaooboooaooooooo","ooooaoboooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[12,16]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","ooboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","ooooooooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[5,18]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","ooboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[0,15]],"map":["ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[3,0]],"map":["oooaooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[5,1]],"map":["oooaooooooooooooooo","ooooobooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oooooooobaooooooooo","oooooooaooboooooooo","oooobbooooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[6,5]],"map":["oooaooooooooooooooo","ooooobooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooaobaooooooooo","oooooooaooboooooooo","oooobbooooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[6,6]],"map":["oooaooooooooooooooo","ooooobooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooaobaooooooooo","oooooobaooboooooooo","oooobbooooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[6,7]],"map":["oooaooooooooooooooo","ooooobooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooaobaooooooooo","oooooobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[4,6]],"map":["oooaooooooooooooooo","ooooobooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[1,4]],"map":["oooaooooooooooooooo","ooooobooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oaooooooooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[0,0]],"map":["booaooooooooooooooo","ooooobooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oaooooooooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[2,0]],"map":["boaaooooooooooooooo","ooooobooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oaooooooooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[12,0]],"map":["boaaooooooooboooooo","ooooobooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oaooooooooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[13,0]],"map":["boaaoooooooobaooooo","ooooobooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oaooooooooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[14,0]],"map":["boaaoooooooobaboooo","ooooobooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oaooooooooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[15,0]],"map":["boaaoooooooobabaooo","ooooobooooooooooooo","ooooooooooooooooooo","ooooooooooooooooooo","oaooooooooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[16,1]],"map":["boaaoooooooobabaooo","ooooobooooooooooboo","ooooooooooooooooooo","ooooooooooooooooooo","oaooooooooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[17,1]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooooooooo","ooooooooooooooooooo","oaooooooooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[12,2]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","ooooooooooooooooooo","oaooooooooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[5,3]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oooooaooooooooooooo","oaooooooooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[3,3]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oooboaooooooooooooo","oaooooooooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[1,3]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oaoboaooooooooooooo","oaooooooooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[7,4]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oaoboaooooooooooooo","oaooooobooooooooooo","ooooooaobaooooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[10,5]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oaoboaooooooooooooo","oaooooobooooooooooo","ooooooaobaaoooooooo","oooobobaooboooooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[13,6]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oaoboaooooooooooooo","oaooooobooooooooooo","ooooooaobaaoooooooo","oooobobaooboobooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooo","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[18,12]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oaoboaooooooooooooo","oaooooobooooooooooo","ooooooaobaaoooooooo","oooobobaooboobooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooa","aoooooooooobobooooo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[17,13]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oaoboaooooooooooooo","oaooooobooooooooooo","ooooooaobaaoooooooo","oooobobaooboobooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooa","aoooooooooobobooobo","oobooboaooaoooooooo","boboaooboooaooooooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[14,15]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oaoboaooooooooooooo","oaooooobooooooooooo","ooooooaobaaoooooooo","oooobobaooboobooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooa","aoooooooooobobooobo","oobooboaooaoooooooo","boboaooboooaooaoooo","ooooaoboooooboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[10,16]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oaoboaooooooooooooo","oaooooobooooooooooo","ooooooaobaaoooooooo","oooobobaooboobooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooa","aoooooooooobobooobo","oobooboaooaoooooooo","boboaooboooaooaoooo","ooooaoboooboboooooo","ooooooooooooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[8,17]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oaoboaooooooooooooo","oaooooobooooooooooo","ooooooaobaaoooooooo","oooobobaooboobooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooa","aoooooooooobobooobo","oobooboaooaoooooooo","boboaooboooaooaoooo","ooooaoboooboboooooo","ooooooooaoooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[7,17]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oaoboaooooooooooooo","oaooooobooooooooooo","ooooooaobaaoooooooo","oooobobaooboobooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooa","aoooooooooobobooobo","oobooboaooaoooooooo","boboaooboooaooaoooo","ooooaoboooboboooooo","ooooooobaoooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[6,17]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oaoboaooooooooooooo","oaooooobooooooooooo","ooooooaobaaoooooooo","oooobobaooboobooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooa","aoooooooooobobooobo","oobooboaooaoooooooo","boboaooboooaooaoooo","ooooaoboooboboooooo","ooooooabaoooooooooo","oooooaooooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[6,18]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oaoboaooooooooooooo","oaooooobooooooooooo","ooooooaobaaoooooooo","oooobobaooboobooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooa","aoooooooooobobooobo","oobooboaooaoooooooo","boboaooboooaooaoooo","ooooaoboooboboooooo","ooooooabaoooooooooo","oooooaboooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[7,18]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oaoboaooooooooooooo","oaooooobooooooooooo","ooooooaobaaoooooooo","oooobobaooboobooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooa","aoooooooooobobooobo","oobooboaooaoooooooo","boboaooboooaooaoooo","ooooaoboooboboooooo","ooooooabaoooooooooo","oooooabaooooooooooo"],"replyAns":"a","takenResTime":3}

{"coordinate":[[18,17]],"map":["boaaoooooooobabaooo","oooooboooooooooobao","ooooooooooooboooooo","oaoboaooooooooooooo","oaooooobooooooooooo","ooooooaobaaoooooooo","oooobobaooboobooooo","oooobbaoooaaooboooo","oooooaboooooboooaoo","ooaoababooooooooooo","ooooooooaooaabooooo","boabooooooooboooboo","aboaoooooooaobaaooa","aoooooooooobobooobo","oobooboaooaoooooooo","boboaooboooaooaoooo","ooooaoboooboboooooo","ooooooabaooooooooob","oooooabaooooooooooo"],"replyAns":"a","takenResTime":3}



*/
