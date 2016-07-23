#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(int argc, char *argv[])
{
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
		std::cout << "Cannot open the webcam" << std::endl;
		return -1;
	}
	cv::namedWindow("Control", CV_WINDOW_AUTOSIZE);
	int iLowH = 170;
	int iHighH = 179;
	int iLowS = 150;
	int iHighS = 255;
	int iLowV = 60;
	int iHighV = 255;

	// Create trackbar in control window
	cvCreateTrackbar("LowH", "Control", &iLowH, 179);
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255);
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255);
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);

	int iLastX = -1;
	int iLastY = -1;
	// capture a temporary image from camera
	cv::Mat imgTemp;
	cap.read(imgTemp);
	cv::Mat imgLines = cv::Mat::zeros(imgTemp.size(), CV_8UC3);

	while (true) {
		cv::Mat imgOriginal;
		bool bSuccess = cap.read(imgOriginal);
		if (!bSuccess) {
			std::cout << "canot read frame from video stream" << std::endl;
			break;
		}
		cv::Mat imgHSV;
		cv::cvtColor(imgOriginal, imgHSV, cv::COLOR_BGR2HSV);
		cv::Mat imgThresholded;
		cv::inRange(imgHSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imgThresholded);
		// Morpgological opening = remove small objects from background
		cv::erode(imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
		cv::dilate(imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

		// Morpgological closing = remove small objects from background
		cv::dilate(imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
		cv::erode(imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

		cv::Moments oMoments = cv::moments(imgThresholded);
		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		if (dArea > 1000) {
			// calculate the position of the object
			int posX = dM10 / dArea;
			int posY = dM01 / dArea;
			if (iLastX >= 0 && iLastY >=0 && posX >= 0 && posY >=0) {
				// draw a red line from previous point to the current point
				cv::line(imgLines, cv::Point(posX, posY), cv::Point(iLastX, iLastY), cv::Scalar(0, 0, 255), 2);
			}
			iLastX = posX;
			iLastY = posY;
		}

		cv::imshow("Thresholded Image", imgThresholded);
		imgOriginal += imgLines;
		cv::imshow("Original", imgOriginal);

		if (cv::waitKey(30) == 27) {
			std::cout << "ESC is pressed by user" << std::endl;
			break;
		}
	}
	return 0;
}
