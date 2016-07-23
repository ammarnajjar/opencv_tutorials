#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
	Mat img = imread("../../malina.jpg", CV_LOAD_IMAGE_GRAYSCALE), img_filtered;

	// filter kernel for detecting vertical edges
	float vertical_fk[5][5] = {
		{0,0,0,0,0},
		{0,0,0,0,0},
		{-1,-2,6,-2,-1},
		{0,0,0,0,0},
		{0,0,0,0,0},
	};
	// filter kernel for detecting horizontal edges
	float horizontal_fk[5][5] = {
		{0,0,-1,0,0},
		{0,0,-2,0,0},
		{0,0,6,0,0},
		{0,0,-2,0,0},
		{0,0,-1,0,0},
	};
	Mat filter_kernel = Mat(5,5,CV_32FC1, horizontal_fk);
	// apply filter
	filter2D(img, img_filtered, -1, filter_kernel);

	namedWindow("Image");
	imshow("Image", img);

	namedWindow("Filtered Image");
	imshow("Filtered Image", img_filtered);

	while (char(waitKey()) != 'q') {}

	return 0;
}
