#include <cstdio>
#include <cassert>
#include <vector>
#include <opus/opusfile.h>
#include <portaudio.h>
#include "dsputils.hxx"

/* Stores error-values in this variable. This is very convinient to do this. */
OpusPlayerStatusCode error = 0;

int main(int argc, char** argv) {
	// Abort the program if the filename pointing to the file isn't specified in the arguments.
	if (argc < 2) abort();

	PaStream* playback_stream; // PortAudio's output stream to write into.
	OggOpusFile* opus_file_handle; // Decoder handle for OPUS files.

	std::vector<short> opus_decoded; // |
	opus_decoded.resize(PCM_SIZE);   // | *Temporary* storage of decoded 16-bit PCM data
									 // | held to playback.

	/* Setup the audio-device for playbacking PCM data into output. 
	   PortAudio: is a library for cross-compatible audio applications. */
	Pa_Initialize();
	Pa_OpenDefaultStream(
		&playback_stream,
		0, 2, paInt16, 48000.0, PCM_BASE_SIZE,
		NULL, NULL
	);
	Pa_StartStream(playback_stream);

	/* Setup the OPUS decoder and check for errors in library or invalid file. */
	opus_file_handle = op_open_file(argv[1], &error);
	if (error != 0) goto exit;

	/**
	 * Put a little bit of information to the STDERR.
	 * This helps the end-user understand things little bit.
	 */
	fprintf(stderr,
		"* %s [%.2fs]\n",
		argv[1], (float)op_pcm_total(opus_file_handle, 0) / 48000.0f
	);

	while (op_read_stereo(opus_file_handle, opus_decoded.data(), PCM_SIZE) > 0) {
		if (error != 0) break;
		/* Write the buffer to the PortAudio stream for Playback. */
		Pa_WriteStream(playback_stream, opus_decoded.data(), PCM_BASE_SIZE);
		if (error != paNoError) break;
	}

exit:
	Pa_StopStream(playback_stream);
	Pa_CloseStream(playback_stream);
	Pa_Terminate();
}