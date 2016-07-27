// file: point_polygon.cpp
// Program to find the smallest contour that surrounds the clicked point
//
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat img_all_contours;
std::vector< std::vector<cv::Point> > closed_contours;
std::vector<cv::Vec4i> heirarchy;

// @function to approximate contours by closed contours
std::vector<std::vector<cv::Point> > make_contours_closed(std::vector<std::vector<cv::Point> > contours)
{
	std::vector<std::vector<cv::Point> > closed_contours;
	closed_contours.resize(contours.size());
	for (size_t i = 0; i < contours.size(); ++i) {
		cv::approxPolyDP(contours[i], closed_contours[i], 0.1, true);
	}
	return closed_contours;
}

int smallest_contour(cv::Point p, std::vector<std::vector<cv::Point> > contours, std::vector<cv::Vec4i> heirarchy)
{
	int idx = 0, prev_idx = -1;
	while (idx >= 0) {
		std::vector<cv::Point> c = contours[idx];
		// Point-polygon test
		double d = cv::pointPolygonTest(c, p, false);
		// if point is inside the contour, check its children for an even smaller contour
		if (d > 0) {
			prev_idx = idx;
			idx = heirarchy[idx][2];
		}
		// else check the next contour on the same level
		else idx = heirarchy[idx][0];
	}
	return prev_idx;
}

void on_mouse(int event, int x, int y, int, void*)
{
	if (event != cv::EVENT_LBUTTONDOWN) {
		return;
	}
	// clicked point
	cv::Point p(x, y);

	// find index of smallest enclosing contour
	int contour_show_idx = smallest_contour(p, closed_contours, heirarchy);
	// if no such a contour, user clicked outside all contours, hense clear image
	if (contour_show_idx < 0) {
		cv::imshow("Contours", img_all_contours);
		return;
	}
	// Draw the smallest contour using a thick red line
	std::vector< std::vector<cv::Point> > contour_show;
	contour_show.push_back(closed_contours[contour_show_idx]);

	if (!contour_show.empty()) {
		cv::Mat img_show = img_all_contours.clone();
		cv::drawContours(img_show, contour_show, -1, cv::Scalar(0, 0, 255), 3);
		cv::imshow("Contours", img_show);
	}
}

int main(int argc, char *argv[])
{
	cv::Mat img = cv::imread(argv[1]);
	img_all_contours = img.clone();
	cv::Mat img_b;
	cvtColor(img, img_b, CV_RGB2GRAY);
	cv::Mat edges;
	cv::Canny(img_b, edges, 50, 100);
	// extract contours and heirarchy
	std::vector< std::vector<cv::Point> > contours;
	cv::findContours(edges, contours, heirarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	// make contours closed so point-polygon test is valid
	closed_contours = make_contours_closed(contours);

	// Draw all contours using a thin green line
	cv::drawContours(img_all_contours, closed_contours, -1, cv::Scalar(0, 255, 0));
	cv::imshow("Contours", img_all_contours);

	// mouse callback
	cv::setMouseCallback("Contours", on_mouse);

	while (char(cv::waitKey(1)) != 'q') {};

	return 0;
}


