#include "Utilities.h"


void test_ppm(void)
{
	const int w = 250;
	const int h = 250;

	std::ofstream out("img.ppm", std::ofstream::binary);

	out << "P3\n" << w << " " << h << "\n255\n";
	for (int j = h - 1; j >= 0; j--) {
		for (int i = 0; i < w; i++) {
			float r = float(i) / float(w);
			float g = float(j) / float(h);
			float b = 0.2f;

			int ir = int(255.99 * r);
			int ig = int(255.99 * g);
			int ib = int(255.99 * b);

			out << ir << " " << ig << " " << ib << "\n";
		}
	}
}

/*
void write_ppm(const int width, const int height, const std::string & name, const std::vector<rgb>& img) {
	const int w = width;
	const int h = height;

	std::ofstream out(name, std::ofstream::binary);

	out << "P3\n" << w << " " << h << "\n255\n";
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {

			int r = int(img[w * j + i].r);
			int g = int(img[w * j + i].g);
			int b = int(img[w * j + i].b);

			out << r << " " << g << " " << b << "\n";
		}
	}
}*/
