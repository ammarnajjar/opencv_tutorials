#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


cv::Mat edges, edges_thresholded;
int slider = 50;

void on_slider(int, void*) {
	if (!edges.empty()) {
		cv::threshold(edges, edges_thresholded, slider, 255, cv::THRESH_TOZERO);
		cv::imshow("Thresholded Scharr edges", edges_thresholded);
	}
}

int main(int argc, char *argv[])
{
	cv::Mat image = cv::imread("../../lena.jpg"), image_blurred;

	// blur image with a gaussian kernel to remove edge noise
	cv::GaussianBlur(image, image_blurred, cv::Size(3, 3), 0, 0);

	// convert to gray
	cv::Mat image_gray;
	cv::cvtColor(image_blurred, image_gray, CV_RGB2GRAY);

	// gradient in X and Y directions
	cv::Mat grad_x, grad_y;

	cv::Scharr(image_gray, grad_x, CV_32F, 1, 0);
	cv::Scharr(image_gray, grad_y, CV_32F, 0, 1);

	cv::Mat grad = grad_x + grad_y;
	sqrt(grad, grad);

	//display
	cv::namedWindow("Original Image");
	cv::namedWindow("Thresholded Scharr edges");

	// convert to 8bit  depth for displaying
	grad.convertTo(edges, CV_8U);
	cv::threshold(edges, edges_thresholded, slider, 255, cv::THRESH_TOZERO);

	cv::imshow("Original Image", image);
	cv::imshow("Thresholded Scharr edges", edges_thresholded);

	cv::createTrackbar("Threshold", "Thresholded Scharr edges", &slider, 255, on_slider);

	while(char(cv::waitKey(1)) != 'q') {}
	return 0;
}
