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
    glob(argv[1], GLOB_TILDE, NULL, &opus_files);
    
    // /* Configuration to play the audio back to the speaker. */
    // pa_sample_spec playback_attrs = {
    //     .format = PA_SAMPLE_S16NE, // Decode 16-bit audio from input (system byte-order).
    //     .rate = 48000, // https://github.com/xiph/opusfile/blob/master/include/opusfile.h#L56
    // };

    /* Stream to send the decoded PCM data from the OPUS file to. */
    PaStream *playback_stream;
    const PaStreamInfo* playback_streaminfo;
    short* opus_decoded = (short*)malloc(sizeof(short) * PCM_SIZE);

    Pa_Initialize();
    Pa_OpenDefaultStream(
        &playback_stream, 
        0, 2, paInt16, 48000.0, PCM_BASE_SIZE, 
        NULL, NULL
    );
    playback_streaminfo = Pa_GetStreamInfo(playback_stream);
    Pa_StartStream(playback_stream);

    size_t file_idx = 0;
    player:
        OggOpusFile* opus_file_handle = op_open_file(opus_files.gl_pathv[file_idx], &error);
        if(error != 0) {
            ++file_idx;
            fprintf(stderr, "%s: the file a invalid OPUS file or doesn't exist!", opus_files.gl_pathv[file_idx]);
            goto player;
        }

        /**
         * Put a little bit of information to the STDERR.
         * This helps the end-user understand things little bit.
         */
        fprintf(stderr, 
            "* %s\n"
            "   - Duration: %.2fs\n",
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
        while(op_read_stereo(opus_file_handle, opus_decoded, PCM_SIZE) > 0) {
            // Exit the loop if cannot decode further.
            if (error != 0) break;

            /* Write the buffer to the PortAudio stream for Playback. */
            Pa_WriteStream(playback_stream, opus_decoded, PCM_BASE_SIZE);
            
            // Exit the loop if cannot playback PCM buffer.
            if (error != paNoError) break;
        }

        // If files are left to be played then again goto the current statement.
        if(file_idx < opus_files.gl_pathc) ++file_idx; goto player;

    Pa_StopStream(playback_stream);
    Pa_CloseStream(playback_stream);
    Pa_Terminate();
}