// Program to illustrate line and circle detection using High Transform

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat img;
int shape = 0;
int thresh = 100;

void on_trackbar(int, void*)
{
	if (shape == 1) {
		cv::Mat img_gray;
		cvtColor(img, img_gray, CV_RGB2GRAY);
		// find circles
		std::vector<cv::Vec3f> circles;
		cv::HoughCircles(img_gray, circles, CV_HOUGH_GRADIENT, 1, 10, 100, thresh, 5);
		// Draw circles
		cv::Mat img_show = img.clone();
		for (size_t i = 0; i < circles.size(); ++i) {
			cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// Draw the circle center
			cv::circle(img_show, center, 3, cv::Scalar(0, 0, 255), -1);
			// Draw the circle outline;
			cv::circle(img_show, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);
		}
		cv::imshow("Shapes", img_show);
	} else if (shape == 0) {
		cv::Mat edges;
		cv::Canny(img, edges, 50, 100);
		// find lines
		std::vector<cv::Vec2f> lines;
		cv::HoughLines(edges, lines, 1, CV_PI/180.f, thresh);
		// Draw lines
		cv::Mat img_show = img.clone();
		for (size_t i = 0; i < lines.size(); ++i) {
			float rho = lines[i][0];
			float theta = lines[i][1];
			double a = cv::cos(theta), b = cv::sin(theta);
			double x0 = a * rho, y0 = b* rho;
			cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
			cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
			cv::line(img_show, pt1, pt2, cv::Scalar(0, 0, 255));
		}
		cv::imshow("Shapes", img_show);
	}
}

int main(int argc, char *argv[])
{
	img = cv::imread(argv[1]);
	cv::namedWindow("Shapes");

	// sliders
	cv::createTrackbar("Lines/Circles", "Shapes", &shape, 1, on_trackbar);
	cv::createTrackbar("Acc. Thresh", "Shapes", &thresh, 300, on_trackbar);

	// Initialize window
	on_trackbar(0, 0);

	while (char(cv::waitKey(1)) != 'q') {};

	return 0;
}
