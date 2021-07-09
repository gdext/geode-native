#include "connection.hpp"
#include "packets.hpp"

Connection* Connection::connect(unsigned short port) {
    auto c = new Connection();

    asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string("::1"), port);
    c->socket   = new asio::ip::tcp::socket(c->ctx);

    asio::error_code ec;

    c->got_packet_header = false;
    c->socket->connect(endpoint, ec);

    if (ec || !c->socket->is_open()) return nullptr;
    return c;
}

bool Connection::send(const char* data, size_t size) {
    asio::error_code ec;

    socket->write_some(asio::buffer(data, size), ec);

    if (ec) return false;
    else return true;
}

bool Connection::send(std::string str) {
    char* buffer = new char[str.length() + 4];
    *((unsigned int*)buffer) = str.length();

    for (int i = 0; i < str.length(); i++)
        buffer[i + 4] = str[i];

    bool success = send(buffer, str.length() + 4);
    delete buffer;

    return success;
}

bool Connection::send(OutPacket* packet) {
    send(packet->getJSONString());
}

void Connection::add_bytes_to_data(char* data, unsigned int size) {
    unsigned int av_space   = packet_size - current_size;
    unsigned int added_data = size > av_space ? av_space : size;

    memcpy(current_data + current_size, data, added_data);

    current_size += added_data;
}

bool Connection::check_for_data() {
    size_t avalible_bytes = socket->available();
    if (avalible_bytes == 0) return true;

    char* new_data = new char[avalible_bytes];

    int offset = 0;

    asio::error_code ec;

    socket->read_some(asio::buffer(new_data, avalible_bytes), ec);

    if (ec) {
        delete new_data;
        return false;
    }

    if (!got_packet_header && avalible_bytes >= 4) {
        packet_size = *(unsigned int*)new_data;

        got_packet_header = true;

        current_data = new char[packet_size];
        current_size = 0;

        offset += 4;
    }

    if (avalible_bytes - offset > 0) {
        add_bytes_to_data(new_data + offset, avalible_bytes - offset);

        if (current_size >= packet_size) {
            std::string str(current_data, packet_size);

            if (onPacket != 0) onPacket(json::parse(str));
            got_packet_header = false;
        }
    }
}