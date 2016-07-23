#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
// Global variables
const int SLIDE_MAX = 1;
int slider;
Mat img;

// callback function for trackbar event
void on_trackbar(int pos, void *) {
	Mat img_converted;
	if (pos > 0) {
		cvtColor(img, img_converted, CV_RGB2GRAY);
	} else {
		img_converted = img;
	}
	imshow("Trackbar app", img_converted);
}

void color2gray(Mat img) {
	namedWindow("Trackbar app");
	imshow("Trackbar app", img);

	slider = 0;

	createTrackbar("RGB <-> Grayscale", "Trackbar app", &slider, SLIDE_MAX, on_trackbar);
	while (char(waitKey(1)) != 'q') {}
}

// flags updated according to left mouse button activity
bool ldown = false, lup = false;

Point corner1, corner2;
Rect box;

static void mouse_callback(int event, int x, int y, int, void *) {
	// when the moust button is pressed, record its position and save it in corner1
	if (event == EVENT_LBUTTONDOWN) {
		ldown = true;
		corner1.x = x;
		corner1.y = y;
		std::cout << "Corner 1 recorded at " << corner1 << std::endl;
	}
	if (event == EVENT_LBUTTONUP) {
		// also check if user selection is bigger than 20 pixels (for fun)
		if (abs(x - corner1.x) > 20 && abs(y - corner1.y) > 20) {
			lup = true;
			corner2.x = x;
			corner2.y = y;
			std::cout << "Corner 2 recorded at " << corner2 << std::endl;
		} else {
			std::cout << "Please select a bigger region" << std::endl;
			ldown = false;
		}
	}
	// update the box showing the selected region as the user drags the mous
	if (ldown == true && lup == false) {
		Point pt;
		pt.x = x;
		pt.y = y;
		Mat local_img = img.clone();
		rectangle(local_img, corner1, pt, Scalar(0, 0, 255));
		imshow("Cropping app", local_img);
	}

	// Define ROI and crop it out when both corners have been selected
	if (ldown == true && lup == true) {
		box.width = abs(corner1.x - corner2.x);
		box.height = abs(corner1.y - corner2.y);
		box.x = min(corner1.x, corner2.x);
		box.y = min(corner1.y, corner2.y);

		// make an image out of just the selected ROI and display it in the new window
		Mat crop(img, box);
		namedWindow("Crop");
		imshow("Crop", crop);

		ldown = false;
		lup = false;
	}
}

void crop_image(Mat img) {
	namedWindow("Cropping app");
	imshow("Cropping app", img);

	//set the mouse event callback function
	setMouseCallback("Cropping app", mouse_callback);
	while (char(waitKey(1)) != 'q') {}
}

void access_pixel(Mat img, int x, int y){
	Vec3b v = img.at<Vec3b>(x, y);
	std::cout <<  (int)( v[1] ) << std::endl;
}

int main(int argc, char *argv[])
{
	if (argc < 1) {
		std::cout << "Insuffecient arguments" << std::endl;
		return 1;
	}
	img = imread(argv[1]);
	// color2gray(img);
	// crop_image(img);
	access_pixel(img, 200, 200);
	return 0;
}
