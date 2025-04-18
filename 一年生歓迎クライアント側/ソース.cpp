#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>

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