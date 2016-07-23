#include <iostream>
#include <opencv2/opencv.hpp>

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

	Mat image;
	image = imread(argv[1], 1);
	if (!image.data) {
		std::cout <<  "No image data" << std::endl;
		return -1;
	}
	// namedWindow("Display Image", CV_WINDOW_AUTOSIZE);
	imshow("DisplayImage", image);

	waitKey(0);
	return 0;
}
