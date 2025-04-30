#pragma once

namespace nw
{

	extern int  net_handle;      // �l�b�g���[�N�n���h��
	extern char rcv_buf[1024];   // ��M�p�o�b�t�@
	extern char snd_buf[1024];   // ���M�p�o�b�t�@
	extern int port;///�|�[�g�ԍ�

	std::array<std::string, 5> rsv_question();

	/// <summary>
/// ���̉𓚂𑗐M����
/// �������������P�Q�P�T��낵��
/// </summary>
/// <param name="answer">����</param>
/// <returns></returns>
	int send_answer(char answer);

/// <summary>
/// �Ֆʂ�����������
/// �������������P�Q�P�T��낵�� 
/// </summary>
/// <param name="board"></param>
/// <returns></returns>
int update_board(std::vector<std::vector<int>>& board);

int CustomSocketInit(int8_t IP1, int8_t IP2, int8_t IP3, int8_t IP4);

void CustomSocketEnd();

void rsvmsg();
void sndmsg();

}






