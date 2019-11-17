#include <cstdint>
#include <vector>

inline uint8_t global_packet_number() {
    static uint8_t number = 0xFF;
    return (++number & 0x0F);
}

inline std::vector<uint8_t> make_data() {
    std::vector<uint8_t> buf {
        uint8_t(0x80), 0x92, 0x00, 0x31, 0x00, 0x00, 0x00, 0x00,
        0x01, //command
        global_packet_number(), //Global packet number (0-f)
        0x00, 0x01, 0x40, 0x40, 0x00, 0x01, 0x40, 0x40, //Rumble Data
        0x07,  //command(Reset pairing info)
    };

    return buf;
}
