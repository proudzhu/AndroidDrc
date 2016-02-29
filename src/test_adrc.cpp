#include <stdio.h>
#include "dsp/core/dynamic_range_compression.h"

using namespace le_fx;

#define FRAME 1024

int main(int argc, char **argv)
{
	FILE *in, *out;
	int sample_num;
	short inbuf[FRAME];

	if (argc != 3) {
		fprintf(stderr, "usage: adrc input.pcm output.pcm\n");
		return -1;
	}

	in = fopen(argv[1], "rb");
	if (in == NULL) {
		fprintf(stderr, "open input file error: %s\n", argv[1]);
		return -1;
	}

	out = fopen(argv[2], "wb");
	if (out == NULL) {
		fprintf(stderr, "open input file error: %s\n", argv[2]);
		return -1;
	}

	float mTargetGainmB = 10;
	float targetAmp = pow(10, mTargetGainmB/2000.0f); // mB to linear amplification
	float samplingRate = 44100;
	le_fx::AdaptiveDynamicRangeCompression* mCompressor = new le_fx::AdaptiveDynamicRangeCompression();
	mCompressor->Initialize(targetAmp, samplingRate);

	while ((sample_num = fread(inbuf, sizeof(short), FRAME, in)) > 0) {
		for (int inIdx = 0; inIdx < sample_num / 2; inIdx += 2) {
			float left = inbuf[2 * inIdx];
			float right = inbuf[2 * inIdx + 1];
			mCompressor->Compress(&left, &right);
			inbuf[2 * inIdx] = (short)left;
			inbuf[2 * inIdx + 1] = (short)right;
		}
		fwrite(inbuf, sizeof(short), sample_num, out);
		fflush(out);
	}

	printf("Done!\n");

	delete mCompressor;

	fclose(out);
	fclose(in);

	return 0;
}
