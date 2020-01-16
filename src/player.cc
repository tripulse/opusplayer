#include <cstdio>
#include <cassert>
#include <vector>
#include <opus/opusfile.h>
#include <portaudio.h>
#include <glob.h>
#include "dsputils.hxx"

OpusPlayerStatusCode error = 0;

int main(int argc, char** argv) {
    if(argc < 2) abort();

    glob_t opus_files;
    glob(argv[1], 0, NULL, &opus_files);

    PaStream *playback_stream;
    OggOpusFile *opus_file_handle;

    // Buffer to store the decoded data, and playback it.
    std::vector<short> opus_decoded;
    opus_decoded.resize(PCM_SIZE);

    /** 
     * Intialize the audio output system for output.
     * Configuration:
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

    size_t op_fidx = 0; // Index of the current OPUS file.
    player:
        if(opus_files.gl_pathc == 0) goto exit;
        opus_file_handle = op_open_file(opus_files.gl_pathv[op_fidx], &error);

        // Until everything playing every file finishes run this loop.
        // Skip files that can't be parsed with opusfile.
        if(error != 0 && op_fidx < opus_files.gl_pathc) {++op_fidx; goto player;}
        else if(error != 0) goto exit;

        /* Put some info on the Stderr. */
        fprintf(stderr, 
            "* %s [%.2fs]\n",
            opus_files.gl_pathv[opus_file_index], (float)op_pcm_total(opus_file_handle, 0) / 48000.0f
        );

        while(op_read_stereo(opus_file_handle, opus_decoded.data(), PCM_SIZE) > 0) {
            if (error != 0) break;  // Exit the loop if cannot decode further.
            Pa_WriteStream(playback_stream, opus_decoded.data(), PCM_BASE_SIZE);
            if (error != paNoError) break;
        }

        if(opus_file_index < opus_files.gl_pathc) ++opus_file_index; goto player;

    exit:
        Pa_StopStream(playback_stream);
        Pa_CloseStream(playback_stream);
        Pa_Terminate();
        globfree(&opus_files);
}
