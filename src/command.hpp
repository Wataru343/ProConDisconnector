#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <cstdint>
#include <array>

namespace command {
inline uint8_t global_packet_number() {
    static uint8_t number = 0xFF;
    return (++number & 0x0F);
}

inline auto reset_pairing() {
    std::array<uint8_t, 20> buf {
        0x80, 0x92, 0x00, 0x31, 0x00, 0x00, 0x00, 0x00,
        0x01, //command
        0x05, //Global packet number (0-f)
        0x00, 0x01, 0x40, 0x40, 0x00, 0x01, 0x40, 0x40, //Rumble Data
        0x07,  //command(Reset pairing info)
        0x00
    };

    return buf;
}

inline auto reboot() {
    std::array<uint8_t, 20> buf {
        0x80, 0x92, 0x00, 0x31, 0x00, 0x00, 0x00, 0x00,
        0x01, //command
        0x05, //Global packet number (0-f)
        0x00, 0x01, 0x40, 0x40, 0x00, 0x01, 0x40, 0x40, //Rumble Data
        0x06,  //command(Reset pairing info)
        0x01
    };

    return buf;
}

constexpr inline auto status() {
    std::array<uint8_t, 2> buf { 0x80, 0x01 };

    return buf;
}

constexpr inline auto handshake() {
    std::array<uint8_t, 2> buf { 0x80, 0x02 };

    return buf;
}
}

#endif // COMMAND_HPP
