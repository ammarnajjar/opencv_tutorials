#include <iostream>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// function headers
void detectAndDisplay(cv::Mat frame);

// Global variables
cv::String face_cascade_name = "../haarcascade_frontalface_alt.xml";
cv::String eyes_cascade_name = "../haarcascade_eye_tree_eyeglasses.xml";
cv::CascadeClassifier face_cascade;
cv::CascadeClassifier eyes_cascade;
std::string window_name = "Capture - Face detection";
cv::RNG rng(12345);

// @function main
int main(int argc, char *argv[])
{
	CvCapture* capture;
	cv::Mat frame;

	// 1. load the cascades
	if (!face_cascade.load(face_cascade_name)) {
		std::cout << "! Error loading face cascade.. " << std::endl;
		return -1;
	}
	if (!eyes_cascade.load(eyes_cascade_name)) {
		std::cout << "! Error loading eye cascade.. " << std::endl;
		return -1;
	}
	// 2. Read the video stream
	capture = cvCaptureFromCAM(0);
	if (capture) {
		while (true) {
			frame = cvQueryFrame(capture);
			// 3. apply the classifier to the frame
			if (!frame.empty()) {
				detectAndDisplay(frame);
			} else {
				std::cout << "No captured frame" << std::endl;
				break;
			}
			if (char(cv::waitKey(10)) == 'q') {
				break;
			}
		}
	}
	return 0;
}

// @function detectAndDisplay
void detectAndDisplay(cv::Mat frame) {
	std::vector<cv::Rect> faces;
	cv::Mat frame_gray;
	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	// detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
	for (size_t i = 0; i < faces.size(); ++i) {
		cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		cv::ellipse(frame, center, cv::Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);
		cv::Mat faceROI = frame_gray(faces[i]);
		std::vector<cv::Rect> eyes;
		// in each face detect the eyes
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
		for (size_t i = 0; i < eyes.size(); ++i) {
			cv::Point center(faces[i].x + eyes[i].x + eyes[i].width * 0.5, faces[i].x + eyes[i].y + eyes[i].height * 0.5);
			int radius = cvRound((eyes[i].width + eyes[i].height)*0.25);
			cv::circle(frame, center, radius, cv::Scalar(255, 0, 0), 4, 8, 0);
		}
	}
	cv::imshow(window_name, frame);
}
