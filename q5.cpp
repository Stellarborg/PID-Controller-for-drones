#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;

void getContours(Mat imgDil, Mat img) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;

		vector<vector<Point>> conPoly(contours.size());
		vector<Rect> boundRect(contours.size());
		string objectType;

		if (area > 1000)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);

			int objCor = conPoly[i].size();

			if (objCor == 3) { objectType = "Triangle"; }
			if (objCor == 4) {
				float aspRatio = boundRect[i].width / (float)boundRect[i].height;
				if (aspRatio > 0.95 && aspRatio < 1.05) {
					objectType = "Square";
				}
				else {
					objectType = "Rectangle";
				}
			}
			if (objCor > 4) { objectType = "Circle"; }

			drawContours(img, contours, -1, Scalar(255, 0, 255), 2);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), FILLED);
			putText(img, objectType, { boundRect[i].x,boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN, 1, Scalar(0, 69, 255), 1);
		}
	}
}

void main() {
	string path = "Resources/shapes.png";
	Mat img = imread(path);

	// Preprocessing of image
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(img, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);

	getContours(imgDil, img);

	imshow("Image", img);
	waitKey(0);

}

