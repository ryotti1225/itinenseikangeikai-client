#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

int main() {
    try {
        // io_contextの作成
        boost::asio::io_context io_context;

        // リゾルバでホスト名とポートを解決
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "12345");

        // ソケットの作成
        tcp::socket socket(io_context);

        // サーバに接続
        boost::asio::connect(socket, endpoints);

        std::cout << "Connected to the server!" << std::endl;

        // サーバに送信するメッセージ
        std::string message = "Hello from client!";
        boost::asio::write(socket, boost::asio::buffer(message));

        // サーバからの応答を受信
        char reply[1024];
        size_t reply_length = socket.read_some(boost::asio::buffer(reply));
        std::cout << "Reply from server: ";
        std::cout.write(reply, reply_length);
        std::cout << std::endl;

    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}