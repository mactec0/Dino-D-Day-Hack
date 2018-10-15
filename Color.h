#pragma once 
struct sColor {
	unsigned char RGBA[4];
};


class cColor {
public:

	unsigned char RGBA[4];

	cColor() {
		RGBA[0] = 0;
		RGBA[1] = 0;
		RGBA[2] = 0;
		RGBA[3] = 255;
	}

	cColor(int R, int G, int B, int A) {
		RGBA[0] = (unsigned char)R;
		RGBA[1] = (unsigned char)G;
		RGBA[2] = (unsigned char)B;
		RGBA[3] = (unsigned char)A;
	}

	cColor(unsigned char iRGBA[4]) {
		RGBA[0] = iRGBA[0];
		RGBA[1] = iRGBA[1];
		RGBA[2] = iRGBA[2];
		RGBA[3] = iRGBA[3];
	}
};
