#include <string>
#define PCM_BASE_SIZE 480
#define PCM_SIZE PCM_BASE_SIZE * 2

typedef int OpusPlayerStatusCode;

/**
 * function: channel_layout_name()
 * description: converts number of channels to human readble text.
 * params:
 *      - channels: u8  --  Number of channels exist in the audio stream.
 */
std::string channel_layout_name(uint8_t channels) {
    switch(channels) {
        case 1: return "mono";
        case 2: return "stereo";
        case 3: return "2.1";
        case 6: return "surround";
        default: return std::to_string(channels);
    }
}