#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>

cv::Mat image, image_gray;
int max_corners = 20;

void on_slider(int, void*)
{
	if (image_gray.empty()) {
		return;
	}
	max_corners = std::max(1, max_corners);
	cv::setTrackbarPos("Max no. of corners", "Corners", max_corners);
	float quality = 0.01;
	int min_distance = 10;

	std::vector<cv::Point2d> corners;
	cv::goodFeaturesToTrack(image_gray, corners, max_corners, quality, min_distance);

	// Draw the corners as little circles
	cv::Mat image_corners = image.clone();
	for (size_t i = 0; i < corners.size(); ++i) {
		cv::circle(image_corners, corners[i], 4, CV_RGB(255, 0, 0), -1);
	}
	cv::imshow("Corners", image_corners);
}

int main(int argc, char *argv[])
{
	image = cv::imread(argv[1]);
	cvtColor(image, image_gray, CV_RGB2GRAY);
	cv::namedWindow("Corners");
	cv::createTrackbar("Max no. of corners", "Corners", &max_corners, 250, on_slider);

	on_slider(0, 0);
	while(char(cv::waitKey(1)) != 'q') {}

	return 0;
}
