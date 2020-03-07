/**
 * Copyright (C) 2020 Shantanu Biswas.
 * See the LICENSE file for more information.
 *
 * This program decodes OGGOpus files from the current directory
 * namespace (recursively) and plays them in no particular order,
 * with PortAudio (universal audio capture/playback API).
 */

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <opus/opusfile.h>
#include <portaudio.h>
#include <glob.h>

int main(int argc, char** argv) {
    if(argc < 2) abort();

    glob_t infiles;
    glob(argv[1], 0, NULL, &infiles);

    PaStream    *audio_ctx;
    short        pcmdata[960];
    OggOpusFile *op_parser;
    int          err; // store the error code in it.

    Pa_Initialize();
    Pa_OpenDefaultStream(
        &audio_ctx,
        0, 2, paInt16, 48000.0, 480, 
        NULL, NULL);
    Pa_StartStream(audio_ctx);

    for(size_t fid = 0; fid < infiles.gl_pathc; ++fid) {
        op_parser = op_open_file(infiles.gl_pathv[fid], NULL);
        if(op_parser == NULL) continue;

        fprintf(stderr,
            "* %s [%.2fs]\n",
            infiles.gl_pathv[fid],
            (float)op_pcm_total(op_parser, 0) / 48000.0f);

        while((err = op_read_stereo(op_parser, pcmdata, 960)) > 0) {
            if(!err) break;
            if(Pa_WriteStream(audio_ctx, pcmdata, 480)
               != paNoError) break;
        }
    }

    Pa_StopStream(audio_ctx);
    Pa_CloseStream(audio_ctx);
    Pa_Terminate();
    globfree(&infiles);
}
