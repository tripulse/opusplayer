#include <cstdio>
#include <cassert>
#include <vector>
#include <opus/opusfile.h>
#include <portaudio.h>
#include <glob.h>
#include "dsputils.hxx"

OpusPlayerStatusCode error = 0; // `0` no errors at all.
int main(int argc, char** argv) {
    glob_t opus_files;
    glob(argv[1], 0, NULL, &opus_files);

    /* Stream to send the decoded PCM data from the OPUS file to. */
    PaStream *playback_stream;
    OggOpusFile *opus_file_handle;
    std::vector<short> opus_decoded;
    opus_decoded.resize(PCM_SIZE);

    Pa_Initialize();
    Pa_OpenDefaultStream(
        &playback_stream, 
        0, 2, paInt16, 48000.0, PCM_BASE_SIZE, 
        NULL, NULL
    );
    Pa_StartStream(playback_stream);

    size_t file_idx = 0;
    player:
        if(opus_files.gl_pathc == 0) goto exit;
        opus_file_handle = op_open_file(opus_files.gl_pathv[file_idx], &error);

        // If files are still left in the Path vector then rerun this statement again.
        // Until all the files are finished to playback, play them.
        if(error != 0 && file_idx < opus_files.gl_pathc) {++file_idx; goto player;}
        else if(error != 0) goto exit;

        /**
         * Put a little bit of information to the STDERR.
         * This helps the end-user understand things little bit.
         */
        fprintf(stderr, 
            "* %s [%.2fs]\n",
            opus_files.gl_pathv[file_idx], (float)op_pcm_total(opus_file_handle, 0) / 48000.0f
        );

        /**
         * Decode the samples with libopus.
         * And play the back immedietaly with the libpulse
         * API which connects to libALSA.
         *
         * The more number of channels in the OPUS packet
         * the much time it would take to decode.
         */
        while(op_read_stereo(opus_file_handle, opus_decoded.data(), PCM_SIZE) > 0) {
            // Exit the loop if cannot decode further.
            if (error != 0) break;
            /* Write the buffer to the PortAudio stream for Playback. */
            Pa_WriteStream(playback_stream, opus_decoded.data(), PCM_BASE_SIZE);
            // Exit the loop if cannot playback PCM buffer.
            if (error != paNoError) break;
        }

        if(file_idx < opus_files.gl_pathc) ++file_idx; goto player;

    exit:
        Pa_StopStream(playback_stream);
        Pa_CloseStream(playback_stream);
        Pa_Terminate();
        globfree(&opus_files);
}