#include "incl.h"


namespace nw
{
    int net_handle = 0;         // �l�b�g���[�N�n���h���̒�`
    char rcv_buf[1024] = {0};   // ��M�p�o�b�t�@�̒�`
    char snd_buf[1024] = {0};   // ���M�p�o�b�t�@�̒�`

	int port = 11600;///�|�[�g�ԍ�
}




std::array<std::string, 5> nw::rsv_question() {
	return{ "A��I��ł�������" ,"a 1913","b","c","d" };
}

/// <summary>
/// ���̉𓚂𑗐M����
/// �������������P�Q�P�T��낵��
/// </summary>
/// <param name="answer">����</param>
/// <returns></returns>
int nw::send_answer(char answer) {

	using namespace std::string_literals;

    std::string answer_str = R"({"answer": ")"s + std::string(1, answer) + R"("})";

	return 0;
}



/// <summary>
/// �Ֆʂ�����������
/// �������������P�Q�P�T��낵�� 
/// </summary>
/// <param name="board"></param>
/// <returns></returns>
int nw::update_board(std::vector<std::vector<int>>& board) {
	static std::vector<std::vector<int>> new_board = board;///�V�����Ֆ�
	std::vector<std::vector<int>> rsv_board;



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
		return -1;
	}
	else {
		// �f�[�^���M
		sprintf_s(snd_buf, "�ڑ��v��\n");
		NetWorkSend(net_handle, snd_buf, lstrlen(snd_buf));
		// ����I��
		return 0;
	}

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
		NetWorkRecv(net_handle, rcv_buf, sizeof(rcv_buf));

		// �T�[�o���Ƀf�[�^���M

		NetWorkSend(net_handle, snd_buf, lstrlen(snd_buf));

	}

}

void nw::sndmsg() {
	// �ϐ���`
	int data_size;               // ��M�f�[�^���̃f�[�^�T�C�Y
	// �擾���Ă��Ȃ���M�f�[�^�ʂ𓾂�
	data_size = GetNetWorkDataLength(net_handle);
	// ��M�f�[�^�ʂ�0���傫���ꍇ
	if (data_size > 0) {
		NetWorkSend(net_handle, snd_buf, lstrlen(snd_buf));
	}
}

