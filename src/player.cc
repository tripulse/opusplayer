#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <opus/opusfile.h>
#include <pulse/simple.h>
#include "memory.h"
#include <unistd.h>

#define PCM_SIZE 960 // I think this is right? ;)
#define PCM_BYTES PCM_SIZE * sizeof(short)

int error = 0; // `0` no errors at all.
int main(int argc, char** argv) {
    OggOpusFile* opus_file_handle = op_open_file(argv[1], &error);
    assert(("File doesn't exist in the filesystem!", error == 0));

    const OpusHead* header = op_head(opus_file_handle, 0);
    pa_sample_spec playback_attrs = {
        .format = PA_SAMPLE_S16NE, // Decode 16-bit audio from input (system byte-order).
        .rate = 48000, // https://github.com/xiph/opusfile/blob/master/include/opusfile.h#L56
        .channels = header->channel_count // 0-255 number of channels.
    };

    /**
     * Intialise a PulseAudio sever for playing back the audio.
     * Returns a opaque instance to the handle to write data into.
     * 
     * This works only in Linux, on windows it wouldn't work.
     */
    pa_simple *playback = pa_simple_new(
        NULL, argv[0], 
        PA_STREAM_PLAYBACK, NULL, 
        argv[0], &playback_attrs, 
        NULL, NULL, 
        &error);

    assert(("PulseAudio server cannot be initialized!", error == 0));

    /* Decoded raw PCM data from the arbitary OPUS file */
    Memory* opus_decoded = (Memory*)malloc(sizeof(Memory)); 
    opus_decoded->data = malloc(sizeof(short) * PCM_SIZE);
    opus_decoded->size = sizeof(short) * PCM_SIZE;

    /**
     * Put a little bit of information to the STDERR.
     * This helps the end-user understand things little bit.
     */
    fprintf(stderr, 
        "%s (Duration: %.2fs, Channels: %d, Out Gain: %d dBFS)",
        argv[1], (float)op_pcm_total(opus_file_handle, 0) / 48000.0f,
        header->channel_count,
        header->output_gain
    );

    /**
     * Decode the samples with libopus.
     * And play the back immedietaly with the libpulse
     * API which connects to libALSA.
     *
     * The more the buffer size the more time it would
     * take to decode the Opus data. So lesser, sizes
     * is preffered for PCs that don't have much processing power.
     */
    while(op_read(opus_file_handle, (short*)opus_decoded->data, PCM_SIZE, &error) > 0) {
        if (error != 0) break; // Break if opus cannot decode the bitstream from the file.
        pa_simple_write(playback, opus_decoded->data, opus_decoded->size, &error);
        if (error != 0) break; // Break if something is wrong with the playback.
    }
}