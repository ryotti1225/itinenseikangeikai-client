#include "incl.h"


namespace nw
{
    int net_handle = 0;         // �l�b�g���[�N�n���h���̒�`
    char rcv_buf[1024] = {0};   // ��M�p�o�b�t�@�̒�`
    char snd_buf[1024] = {0};   // ���M�p�o�b�t�@�̒�`
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

	std::string answer_str =R"{"" + std::string(1, answer);///char��string�ɕϊ�

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






