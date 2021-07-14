#pragma once

#define _WIN32_WINNT 0x0A00

#define ASIO_STANDALONE

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <json.hpp>

using json = nlohmann::json;

class OutPacket;

typedef void(*OnPacketCallback)(nlohmann::json);

class Connection {
public:
    static Connection* connect(unsigned short port);

    bool send(const char* data, size_t size);
    bool send(std::string str);
    
    bool send(OutPacket* packet);

    bool check_for_data();

    OnPacketCallback onPacket = 0;

private:
    asio::io_context ctx;

    asio::ip::tcp::endpoint* endpoint;
    asio::ip::tcp::socket*   socket;

    std::vector<char> current_data;

    bool got_packet_header;
    unsigned int packet_size;
};