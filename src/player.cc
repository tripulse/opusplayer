#include <cstdio>
#include <cassert>
#include <vector>
#include <opus/opusfile.h>
#include <portaudio.h>
#include <glob.h>
#include "dsputils.hxx"

// This variable stores the error codes returned by various function
// called to perform tasks correspoding to them.
OpusPlayerStatusCode error = 0;

int main(int argc, char** argv) {
    // Abort if no file pattern is speicfied as the input parameter.
    if(argc < 2) abort();

    // Glob all the files matching the following pattern 
    // provided in the arguments.
    glob_t opus_files;
    glob(argv[1], 0, NULL, &opus_files);

    PaStream *playback_stream;      // Opaque instance to the Play stream to send data.
    OggOpusFile *opus_file_handle;  // File handle to grab the OPUS data from.

    /**
     * Temporarily holds the decoded PCM data for playback.
     * 16-bit signed, because every system's CPU doesn't have FPU.
     */
    std::vector<short> opus_decoded;
    opus_decoded.resize(PCM_SIZE);

    /** 
     * Intialize the audio output system for output.
     * Connects with the Host API and add layer of
     * abstraction to play/capture audio.
     * 
     * Output is always:
     *  ~ 16-bit Signed.
     *  ~ Stereo channels.
     *  ~ 48khZ samplerate.
     */
    Pa_Initialize();
    Pa_OpenDefaultStream(
        &playback_stream, 
        0, 2, paInt16, 48000.0, PCM_BASE_SIZE, 
        NULL, NULL
    );
    Pa_StartStream(playback_stream);

    size_t opus_file_index = 0; // Index of the current OPUS file.
    player:
        if(opus_files.gl_pathc == 0) goto exit;
        opus_file_handle = op_open_file(opus_files.gl_pathv[opus_file_index], &error);

        // If files are still left in the Path vector then rerun this statement again.
        // Until all the files are finished to playback, play them.
        if(error != 0 && opus_file_index < opus_files.gl_pathc) {++opus_file_index; goto player;}
        else if(error != 0) goto exit;

        /**
         * Put a little bit of information to the STDERR.
         * This helps the end-user understand things little bit.
         */
        fprintf(stderr, 
            "* %s [%.2fs]\n",
            opus_files.gl_pathv[opus_file_index], (float)op_pcm_total(opus_file_handle, 0) / 48000.0f
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

        if(opus_file_index < opus_files.gl_pathc) ++opus_file_index; goto player;

    exit:
        Pa_StopStream(playback_stream);
        Pa_CloseStream(playback_stream);
        Pa_Terminate();
        globfree(&opus_files);
}