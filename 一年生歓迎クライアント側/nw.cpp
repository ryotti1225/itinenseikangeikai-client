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
		j["replyAns"] = char(answer);

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

