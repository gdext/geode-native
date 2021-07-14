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
    char* buffer = new char[str.length() + 1];

    for (int i = 0; i < str.length(); i++)
        buffer[i] = str[i];

    buffer[str.length()] = 0x17;

    bool success = send(buffer, str.length() + 1);
    delete buffer;

    return success;
}

bool Connection::send(OutPacket* packet) {
    send(packet->getJSONString());
}

bool Connection::check_for_data() {
    size_t avalible_bytes = socket->available();
    if (avalible_bytes == 0) return true;

    std::vector<char> new_data(avalible_bytes);

    int offset = 0;

    asio::error_code ec;

    socket->read_some(asio::buffer(new_data.data(), new_data.size()), ec);

    if (ec)
        return false;

    std::stringstream ss;
    for(int i = 0; i < new_data.size(); i++)
        ss << std::hex << (int) new_data[i] << " ";

    current_data.insert(current_data.end(), new_data.begin(), new_data.end());

    std::vector<char> replacement;

    bool success = true;
    while (success) {
        success = false;
        for (int i = 0; i < current_data.size(); i++) {
            if (current_data[i] == 0x17) {
                std::string str(current_data.data(), i);
                if (onPacket != 0) onPacket(json::parse(str));

                for (int j = i + 1; j < current_data.size(); j++)
                    replacement.push_back(current_data[j]);

                current_data = replacement;
                replacement.clear();
                success = true;
                break;
            }
        }
    }
}