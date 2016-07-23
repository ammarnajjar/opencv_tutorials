#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " <option(s)> SOURCES\n"
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-i,--input IMAGE\tSpecify the source image"
              << std::endl;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		show_usage(argv[0]);
		return 1;
	}
	Mat a, c;
	a = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	Mat b(a);
	c = a;
	Mat d(a, Rect(10, 10, 100, 100)); // Rectangle
	Mat e = a(Range::all(), Range(1, 3)); // using row and column boundaries

	Mat f = a.clone();
	Mat g;
	a.copyTo(g);

	Mat m(2, 2, CV_8UC3, Scalar(0, 0, 255));
	std::cout << "M = \n" << m << std::endl;
	return 0;
}
