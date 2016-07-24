#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat img;
std::vector<std::vector<cv::Point> > contours;
std::vector<cv::Vec4i> heirarchy;

int levels = 0;

void on_trackbar(int, void*)
{
	if (contours.empty()) {
		return;
	}
	cv::Mat img_show = img.clone();

	// Draw contours of the level indicated by slider
	cv::drawContours(img_show, contours, -1, cv::Scalar(0, 0, 255), 3, 8, heirarchy, levels);
	cv::imshow("Contours", img_show);
}

int main(int argc, char *argv[])
{
	img = cv::imread(argv[1]);
	cv::Mat img_b;
	cvtColor(img, img_b, CV_RGB2GRAY);

	cv::Mat edges;
	Canny(img_b, edges, 50, 100);

	// extract contours and heirarchy
	cv::findContours(edges, contours, heirarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

	cv::namedWindow("Contours");
	cv::createTrackbar("levels", "Contours", &levels, 15, on_trackbar);

	// Initialize by drawing the top level contours (as levels is initialized to 0)
	on_trackbar(0, 0);

	while( char(cv::waitKey(1)) != 'q') {}

	return 0;
}
