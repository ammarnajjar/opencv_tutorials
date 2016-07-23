#include <iostream>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
		std::cout << "Cannot open video camera" << std::endl;
		return -1;
	}
	double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	std::cout << "Frame size: " << width << "x" << height << std::endl;

	cv::namedWindow("My Video", CV_WINDOW_AUTOSIZE);

	while (cv::waitKey(30) != 27 && cap.isOpened()) {
		cv::Mat frame;
		bool success = cap.read(frame);
		if (!success) {
			std::cout << "cannot read frame from video stream" << std::endl;
			break;
		}
		cv::imshow("My Video", frame);
	}
	return 0;
}
