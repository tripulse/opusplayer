#include <cstdio>
#include <cassert>
#include <string>
#include <array>
#include <opus/opusfile.h>
#include <pulse/simple.h>
#include <glob.h>
#include "memory.h"
#include <unistd.h>

/* Base size to decode the PCM data from the OPUS file. */
#define PCM_BASE_SIZE 480

/* Converts number of channels to a human readable text if possible */
std::string channel_name_h(uint8_t channels) {
    switch(channels) {
        case 1: return "mono";
        case 2: return "stereo";
        case 3: return "2.1";
        case 6: return "surround";
        default: return std::to_string(channels);
    }
}

int error = 0; // `0` no errors at all.
int main(int argc, char** argv) {
    glob_t opus_files;
    glob(argv[1], GLOB_TILDE, NULL, &opus_files);
    
    /* Configuration to play the audio back to the speaker. */
    pa_sample_spec playback_attrs = {
        .format = PA_SAMPLE_S16NE, // Decode 16-bit audio from input (system byte-order).
        .rate = 48000, // https://github.com/xiph/opusfile/blob/master/include/opusfile.h#L56
    };

    size_t file_idx = 0;
    player:
        OggOpusFile* opus_file_handle = op_open_file(opus_files.gl_pathv[file_idx], &error);
        assert(("File doesn't exist in the filesystem!", error == 0));

        /* Skip the current file if cannot be opened. */
        if(error != 0) {
            ++file_idx;
            fprintf(stderr, "%s: the file a invalid OPUS file or doesn't exist!", opus_files.gl_pathv[file_idx]);
            goto player;
        }

        const OpusHead* header = op_head(opus_file_handle, 0);
        playback_attrs.channels = header->channel_count;
        size_t PCM_SIZE = header->channel_count * PCM_BASE_SIZE;

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
        std::vector<short> opus_decoded;

        /**
         * Put a little bit of information to the STDERR.
         * This helps the end-user understand things little bit.
         */
        fprintf(stderr, 
            "* %s\n"
            "   - Duration: %.2fs\n"
            "   - Channels: %s\n"
            "   - Out Gain: %d dBFS\n",
            opus_files.gl_pathv[file_idx], (float)op_pcm_total(opus_file_handle, 0) / 48000.0f,
            channel_name_h(header->channel_count).c_str(),
            header->output_gain
        );

        /**
         * Decode the samples with libopus.
         * And play the back immedietaly with the libpulse
         * API which connects to libALSA.
         *
         * The more number of channels in the OPUS packet
         * the much time it would take to decode.
         */
        while(op_read(opus_file_handle, opus_decoded.data(), opus_decoded.size(), &error) > 0) {
            if (error != 0) break; // Break if opus cannot decode the bitstream from the file.
            pa_simple_write(playback, opus_decoded.data(), opus_decoded.size() * sizeof(short), &error);
            if (error != 0) break; // Break if something is wrong with the playback.
        }

        /* Cleanup the application instance from the PulseAudio server. 
        Because each audio file has different channels. */
        pa_simple_drain(playback, &error); // Wait unti all the data is finished to write.
        pa_simple_flush(playback, &error); // Flush all the reamaining data as samples.
        pa_simple_free(playback); // Free the instance.

        if(file_idx < opus_files.gl_pathc) goto player;
}