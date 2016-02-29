#include <memory>
#include <iostream>
#include <fstream>
#include "dsp/core/dynamic_range_compression.h"

#define FRAME 1024

int main(int argc, char **argv)
{
	if (argc != 3) {
		std::cerr << "usage: adrc input.pcm output.pcm" << std::endl;
		return -1;
	}

	std::ifstream in(argv[1], std::ios::binary);
	if (in == NULL) {
		std::cerr << "open input file error: " << argv[1] << std::endl;
		return -1;
	}

	std::ofstream out(argv[2], std::ios::binary);
	if (out == NULL) {
		std::cerr << "open input file error: " << argv[2] << std::endl;
		return -1;
	}

	//int sample_num;
	short buffer[FRAME];

	float mTargetGainmB = 100;
	float targetAmp = pow(10, mTargetGainmB/2000.0f); // mB to linear amplification
	float samplingRate = 44100;
	auto mCompressor = std::make_shared<le_fx::AdaptiveDynamicRangeCompression>();
	mCompressor->Initialize(targetAmp, samplingRate);

	while (!in.eof()) {
		in.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
		for (int inIdx = 0; inIdx < FRAME / 2; inIdx += 2) {
			float left = buffer[2 * inIdx];
			float right = buffer[2 * inIdx + 1];
			mCompressor->Compress(&left, &right);
			buffer[2 * inIdx] = (short)left;
			buffer[2 * inIdx + 1] = (short)right;
		}
		out.write(reinterpret_cast<char*>(buffer), sizeof(buffer));
	}

	std::cout << "Done!" << std::endl;

	out.close();
	in.close();

	return 0;
}
