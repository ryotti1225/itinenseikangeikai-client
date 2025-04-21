#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

void read_messages(tcp::socket& socket) {
    try {
        for (;;) {
            char data[1024];
            std::memset(data, 0, sizeof(data));

            // サーバーからのメッセージを受信
            size_t length = socket.read_some(boost::asio::buffer(data));
            std::cout << "Server: " << std::string(data, length) << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Connection closed: " << e.what() << std::endl;
    }
}

int main() {
    try {
        boost::asio::io_context io_context;

        // サーバーに接続
        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve("127.0.0.1", "12345"));
        std::cout << "Connected to the server!" << std::endl;

        // 別スレッドでサーバーからのメッセージを受信
        std::thread reader_thread(read_messages, std::ref(socket));

        // メッセージを送信
        for (;;) {
            std::string message;
            std::cout << "You: ";
            std::getline(std::cin, message);
            boost::asio::write(socket, boost::asio::buffer(message));
        }

        reader_thread.join();
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}


const char* TITLE = "Untitled_Dxlib_window";
int WIN_WIDTH = 640;
int WIN_HEIGHT = 480;


void start_dxlib(int WIN_WIDTH, int WIN_HEIGHT, const char* TITLE)
{
	ChangeWindowMode(true);
	SetWindowSizeChangeEnableFlag(false, false);
	SetMainWindowText((const TCHAR*)TITLE);
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);
	SetWindowSizeExtendRate(1.0);
	SetBackgroundColor(0, 128, 128);
	SetDrawScreen(DX_SCREEN_BACK);

	if (DxLib_Init() == -1)
	{
		std::cout << "#WARNIN# DxLib init FAILED #WARNIN#";
	}
}


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	start_dxlib(WIN_WIDTH, WIN_HEIGHT, TITLE);

	int kazu = 249849, a = 0, b = 0, c = 0;
	int x = 0, y = 0, speed_x = 1, speed_y = 1, size = 65, speed = 5;
	char keys[256];


	while (true)
	{
		ClearDrawScreen();

		DrawFormatString(315, 220, GetColor(255, 54, 153), (const TCHAR*)"%d", kazu);
		DrawBox(a, a + 50, a + 15, 300, GetColor(0, 52, 138), true);
		DrawCircle(165 + b, 45 + b, 75 + c, GetColor(9, 225, 87), true);
		a++;
		b += 2;
		c++;
		GetHitKeyStateAll(keys);

		if (x <= 0 - size) {
			speed_x = -speed_x;
		}
		else if (x >= WIN_WIDTH - size) {
			speed_x = -speed_x;
		}
		if (y <= 0 - size) {
			speed_y = -speed_y;
		}
		else if (y >= WIN_HEIGHT - size) {
			speed_y = -speed_y;
		}

		if (CheckHitKey(KEY_INPUT_SPACE)) {
			x += speed_x;
			y += speed_y;
		}

		if (keys[KEY_INPUT_A]) {
			x -= speed;
		}
		else if (keys[KEY_INPUT_D]) {
			x += speed;
		}

		if (keys[KEY_INPUT_W]) {
			y -= speed;
		}
		else if (keys[KEY_INPUT_S]) {
			y += speed;
		}


		DrawCircle(x + size, y + size, size, GetColor(75, 45, 85), true);


		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1)
		{
			break;
		}
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}

	}

	DxLib_End();

	return 0;
}


