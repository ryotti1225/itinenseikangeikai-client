#include "incl.h"

namespace nw
{
	int net_handle = 0;		  // �l�b�g���[�N�n���h���̒�`
	char rcv_buf[1024] = {0}; // ��M�p�o�b�t�@�̒�`
	std::string snd_buf = ""; // ���M�p�o�b�t�@�̒�`

	int port = 12345; /// �|�[�g�ԍ�
	json j;

}


/// <summary>
/// isTrue=���̐���
/// true����
/// </summary>

extern std::string rsv;//=std::string_literals::R("0iu");
		std::array<std::string, 5> result;

std::array<std::string, 5> nw::rsv_question()
{
	if (j.contains("question") && j["question"].contains("question") &&
		j["question"].contains("choices") && j["question"]["choices"].is_array())
	{

		// ���╶��z��̍ŏ��ɐݒ�
		result[0] = j["question"]["question"].get<std::string>();

		char choise = 'A';
		// �I������z��ɐݒ�
		auto &choices = j["question"]["choices"];
		for (int i = 0; i < std::min(choices.size(), size_t(4)); ++i)
		{
			result[i + 1] = choise + " " + choices[i].get<std::string>();
			choise++;
		}

		return result;
	}

	// JSON�f�[�^���s���ȏꍇ�̃f�t�H���g�l
	return {"�����擾�ł��܂���ł���", "A ", "B ", "C ", "D "};
}

/// <summary>
/// ���̉𓚂𑗐M����
/// �������������P�Q�P�T��낵��
/// </summary>
/// <param name="answer">����</param>
/// <returns></returns>
int nw::send(char answer)
{
	using namespace std::string_literals;

	json empty;

	j = empty;

	try
	{
		// ��M����JSON�f�[�^�irsv�j���p�[�X
		if (!rsv.empty())
		{
			j = json::parse(rsv);
		}

		// �ȉ��A�����̃R�[�h
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
					break; // ��
				case 1:
					line += 'a';
					break; // ��
				case 2:
					line += 'b';
					break; // ��
				default:
					line += 'o';
					break; // ���̑����󔒈���
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

		j["takenResTime"] = 3; /// ��ŏ���

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
/// �Ֆʂ�����������
/// �������������P�Q�P�T��낵��
/// </summary>
/// <param name="board"></param>
/// <returns></returns>
int nw::update_board(std::vector<std::vector<int>> &board_old)
{
	static std::vector<std::vector<int>> new_board = board_old; /// �V�����Ֆ�

	std::vector<std::vector<int>> board;
	if (!j.contains("map") || !j["map"].is_array())
	{
		return -1; // ��̔Ֆʂ�Ԃ�
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
				break; // ��
			case 'a':
				row.push_back(1);
				break; // ��
			case 'b':
				row.push_back(2);
				break; // ��
			default:
				row.push_back(0);
				break; // ���̑����󔒈���
			}
		}
		board.push_back(row);
	}

	board_old = board;

	return 0;
}

int nw::CustomSocketInit(int8_t IP1, int8_t IP2, int8_t IP3, int8_t IP4)
{

	// �ϐ���`
	IPDATA ip; // �ڑ��pIP�A�h���X�f�[�^

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
	if (net_handle == -1)
	{
		// �ُ�I��
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
