#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

Mat image, image_blurred;
int slider = 5;
float sigma = 0.3 * ((slider - 1) * 0.5 - 1) + 0.8;

void on_trackbar(int, void*) {
	int k_size = max(1, slider);
	k_size = k_size %2 == 0 ? k_size + 1: k_size;
	setTrackbarPos("Kernel Size", "Blurred Image", k_size);
	sigma = 0.3 * ((k_size - 1) * 0.5 - 1) + 0.8;
	GaussianBlur(image, image_blurred, Size(k_size, k_size), sigma);
	imshow("Blurred Image", image_blurred);
}

int main(int argc, char *argv[])
{
	image = imread("../../malina.jpg");
	namedWindow("Original Image");
	namedWindow("Blurred Image");

	imshow("Original Image", image);
	sigma = 0.3 * ((slider - 1) * 0.5 - 1) + 0.8;
	GaussianBlur(image, image_blurred, Size(slider, slider), sigma);
	imshow("Blurred Image", image_blurred);

	createTrackbar("Kernel Size", "Blurred Image", &slider, 21, on_trackbar);

	while (char(waitKey(1)) != 'q') {}

	return 0;
}
