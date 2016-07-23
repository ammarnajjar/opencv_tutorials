#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// Global variables
cv::Mat src, src_gray;
cv::Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const MAX_LOWTHRESHOLD = 100;
int ratio = 3;
int kernel_size = 3;
std::string window_name = "Edge Map";

/*
 * @function CannyThreshold
 */
void CannyThreshold(int, void*) {
	// reduce noise with a kernel 3x3
	cv::blur(src_gray, detected_edges, cv::Size(3, 3));

	// canny detector
	cv::Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratio, kernel_size);

	// using canny's output as a mask, we display our result
	dst = cv::Scalar::all(0);

	src.copyTo(dst, detected_edges);
	cv::imshow(window_name, dst);
}

int main(int argc, char *argv[])
{
	src = cv::imread(argv[1]);
	if (!src.data) {
		return -1;
	}
	dst.create(src.size(), src.type());

	cvtColor(src, src_gray, CV_BGR2GRAY);
	cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	cv::createTrackbar("Min Threshold:", window_name, &lowThreshold, MAX_LOWTHRESHOLD, CannyThreshold);
	CannyThreshold(0, 0);

	while (cv::waitKey(10) != 'q') {}
	return 0;
}
