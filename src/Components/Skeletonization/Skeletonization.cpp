/*!
 * \file
 * \brief
 * \author Tomek Kornuta,,,
 */

#include <memory>
#include <string>

#include "Skeletonization.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace Skeletonization {

Skeletonization::Skeletonization(const std::string & name) :
		Base::Component(name)  {

}

Skeletonization::~Skeletonization() {
}

void Skeletonization::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	h_onNewImage.setup(this, &Skeletonization::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);

	// Add default dependency of the onNewImage.
	addDependency("onNewImage", &in_img);
}

bool Skeletonization::onInit() {

	return true;
}

bool Skeletonization::onFinish() {
	return true;
}

bool Skeletonization::onStop() {
	return true;
}

bool Skeletonization::onStart() {
	return true;
}


#define NORTH 1
#define SOUTH 3

// 1-neighbors of pixel.
int nays8(cv::Mat im, int r, int c) {
	cv::Scalar pixel;
	int blue, k = 0, i, j;

  for (i = r-1; i <= r+1; i++)
		for (j = c-1; j <= c+1; j++)
			if (i != r || c != j) {
				pixel = im.at<uchar>(j, i);
				blue = pixel.val[0];
				if (blue >= 1)
					k++;
			}

	return k;
}

int connectivity(cv::Mat im, int r, int c) {
	int N = 0, b1, b2;
	cv::Scalar pixel;

	pixel = im.at<uchar>(r, c+1);
	b1 = pixel.val[0];
	pixel = im.at<uchar>(r-1, c+1);
	b2 = pixel.val[0];
	if (b1 >= 1 && b2 == 0)
		N++;

	pixel = im.at<uchar>(r-1, c+1);
	b1 = pixel.val[0];
	pixel = im.at<uchar>(r-1, c);
	b2 = pixel.val[0];
	if (b1 >= 1 && b2 == 0)
		N++;

	pixel = im.at<uchar>(r-1, c);
	b1 = pixel.val[0];
	pixel = im.at<uchar>(r-1, c-1);
	b2 = pixel.val[0];
	if (b1 >= 1 && b2 == 0)
		N++;

	pixel = im.at<uchar>(r-1, c-1);
	b1 = pixel.val[0];
	pixel = im.at<uchar>(r, c-1);
	b2 = pixel.val[0];
	if (b1 >= 1 && b2 == 0)
		N++;

	pixel = im.at<uchar>(r, c-1);
	b1 = pixel.val[0];
	pixel = im.at<uchar>(r+1, c-1);
	b2 = pixel.val[0];
	if (b1 >= 1 && b2 == 0)
		N++;

	pixel = im.at<uchar>(r+1, c-1);
	b1 = pixel.val[0];
	pixel = im.at<uchar>(r+1, c);
	b2 = pixel.val[0];
	if (b1 >= 1 && b2 == 0)
		N++;

	pixel = im.at<uchar>(r+1, c);
	b1 = pixel.val[0];
	pixel = im.at<uchar>(r+1, c+1);
	b2 = pixel.val[0];
	if (b1 >= 1 && b2 == 0)
		N++;

	pixel = im.at<uchar>(r+1, c+1);
	b1 = pixel.val[0];
	pixel = im.at<uchar>(r, c+1);
	b2 = pixel.val[0];
	if (b1 >= 1 && b2 == 0)
		N++;

	return N;
}

void deleteCB(cv::Mat im, cv::Mat tmp) {
	int w, h, blue, i, j;
	cv::Scalar pixel;

	w = im.cols;
	h = im.rows;

	for (i = 1; i < h-1; i++)
		for (int j = 1; j < w-1; j++) {
			pixel = tmp.at<uchar>(i, j);
			blue = pixel.val[0];
			if (blue == 1) {
				//pixel.val[0] = 0;
				//cvSet2D(im, i, j, pixel);
				//cvSet2D(tmp, i, j, pixel);
				im.at<uchar>(i, j) = 0;
				tmp.at<uchar>(i, j) = 0;
			}
		}
}

void stair(cv::Mat im, cv::Mat tmp, int dir) {
	int i, j, b1, b2, b3, b4, b5, b6, b7, b8, b9, w, h;
	cv::Scalar pixel;
	int N, S, E, W, NE, NW, SE, SW, C;

	w = im.cols;
	h = im.rows;

	if (dir == NORTH)
		for (i = 1; i < h-1; i++)
			for (j = 1; j < w-1; j++) {
				pixel = im.at<uchar>(i-1, j-1);
				b1 = pixel.val[0];
				pixel = im.at<uchar>(i-1, j);
				b2 = pixel.val[0];
				pixel = im.at<uchar>(i-1, j+1);
				b3 = pixel.val[0];
				pixel = im.at<uchar>(i, j-1);
				b4 = pixel.val[0];
				pixel = im.at<uchar>(i, j);
				b5 = pixel.val[0];
				pixel = im.at<uchar>(i, j+1);
				b6 = pixel.val[0];
				pixel = im.at<uchar>(i+1, j-1);
				b7 = pixel.val[0];
				pixel = im.at<uchar>(i+1, j);
				b8 = pixel.val[0];
				pixel = im.at<uchar>(i+1, j+1);
				b9 = pixel.val[0];
				if (b1 == 1)
					NW = 1;
				else
					NW = 0;
				if (b2 == 1)
					N = 1;
				else
					N = 0;
				if (b3 == 1)
					NE = 1;
				else
					NE = 0;
				if (b4 == 1)
					W = 1;
				else
					W = 0;
				if (b5 == 1)
					C = 1;
				else
					C = 0;
				if (b6 == 1)
					E = 1;
				else
					E = 0;
				if (b7 == 1)
					SW = 1;
				else
					SW = 0;
				if (b8 == 1)
					S = 1;
				else
					S = 0;
				if (b9 == 1)
					SE = 1;
				else
					SE = 0;

				if (dir == NORTH) {
					if (C && !(N && ((E && !NE && !SW && (!W || !S)) ||
						 (W && !NW && !SE && (!E || !S))))) {
						//pixel.val[0] = 0;
						//cvSet2D(tmp, i, j, pixel);
						tmp.at<uchar>(i, j) = 0;
					} else {
						//pixel.val[0] = 1;
						//cvSet2D(tmp, i, j, pixel);
						tmp.at<uchar>(i, j) = 1;
					}
				} else if (dir == SOUTH) {
					if (C && !(S && ((E && !SE && !NW && (!W || !N)) ||
						 (W && !SW && !NE && (!E || !N))))) {
						//pixel.val[0] = 0;
						//cvSet2D(tmp, i, j, pixel);
						tmp.at<uchar>(i, j) = 0;
					} else {
						//pixel.val[0] = 1;
						//cvSet2D(tmp, i, j, pixel);
						tmp.at<uchar>(i, j) = 1;
					}
				}
			}
}


// Zhang-Suen algorithm.
void skeletonize(cv::Mat im) {
	int janelaAH[][2] = {
		{1, 0}, {0, -1}, {-1, 0}, {0, 1}
	};
	int janelaH[][2] = {
		{0, -1}, {1, 0}, {0, 1}, {-1, 0}
	};
	int aBlue[6];
	int w, h, i, v, j, k, blue, lin, col, iJanela, again = 1;
	cv::Scalar pixel, pixOut;

	w = im.cols;
	h = im.rows;

	//cv::Mat tmp;
	//tmp = cvCreateImage(cvGetSize(im), IPL_DEPTH_8U, 1);
	cv::Mat tmp(im.size(), CV_8UC1, cv::Scalar(0));

  for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			pixel = im.at<uchar>(i, j);
			blue = pixel.val[0];
			if (blue > 0)
				pixel.val[0] = 0;
			else
				pixel.val[0] = 1;
			//cvSet2D(im, i, j, pixel);
			im.at<uchar>(i, j) = pixel.val[0];
			pixOut.val[0] = 0;
			//cvSet2D(tmp, i, j, pixOut);
			tmp.at<uchar>(i, j) = pixOut.val[0];
		}
	}

	while (again) {
		again = 0;
  	for (i = 1; i < h-1; i++)
			for (j = 1; j < w-1; j++) {
				pixel = im.at<uchar>(i, j);
				blue = pixel.val[0];
				if (blue != 1)
					continue;
				k = nays8(im, i, j);
				iJanela = 0;
				if ((k >= 2 && k <= 6) && connectivity(im, i, j) == 1) {
					for (v = 0; v < 6; v++) {
						col = j + janelaAH[iJanela][0];
						lin = i + janelaAH[iJanela][1];
						pixel = im.at<uchar>(lin, col);
						aBlue[v] = pixel.val[0];
						iJanela++;
						if (v == 2)
							iJanela = 1;
					}
					if (aBlue[0]*aBlue[1]*aBlue[2] == 0 &&
							aBlue[3]*aBlue[4]*aBlue[5] == 0) {
						pixOut.val[0] = 1;
						//cvSet2D(tmp, i, j, pixOut);
						tmp.at<uchar>(i, j) = pixOut.val[0];
						again = 1;
					}
				}		// if ((k >= 2...
			}		// for (j = 1;...

			deleteCB(im, tmp);
			if (!again)
				break;

  	for (i = 1; i < h-1; i++)
			for (j = 1; j < w-1; j++) {
				pixel = im.at<uchar>(i, j);
				blue = pixel.val[0];
				if (blue != 1)
					continue;
				k = nays8(im, i, j);
				iJanela = 0;
				if ((k >= 2 && k <= 6) && connectivity(im, i, j) == 1) {
					for (v = 0; v < 6; v++) {
						col = j + janelaH[iJanela][0];
						lin = i + janelaH[iJanela][1];
						pixel = im.at<uchar>(lin, col);
						aBlue[v] = pixel.val[0];
						iJanela++;
						if (v == 2)
							iJanela = 1;
					}
					if (aBlue[0]*aBlue[1]*aBlue[2] == 0 &&
							aBlue[3]*aBlue[4]*aBlue[5] == 0) {
						pixOut.val[0] = 1;
						//cvSet2D(tmp, i, j, pixOut);
						tmp.at<uchar>(i, j) = pixOut.val[0];
						again = 1;
					}
				}		// if ((k >= 2...
			}		// for (j = 1;...

		deleteCB(im, tmp);
	}		// while

	stair(im, tmp, NORTH);
	deleteCB(im, tmp);
	stair(im, tmp, SOUTH);
	deleteCB(im, tmp);

  for (i = 1; i < h-1; i++)
		for (j = 1; j < w-1; j++) {
			pixel = im.at<uchar>(i, j);
			blue = pixel.val[0];
			if (blue > 0)
				pixel.val[0] = 0;
			else
				pixel.val[0] = 255;
			//cvSet2D(im, i, j, pixel);
			im.at<uchar>(i, j) = pixel.val[0];
		}
}		// End skeletonize



void Skeletonization::onNewImage()
{
	CLOG(LTRACE) << "Skeletonization::onNewImage\n";
	try {
		cv::Mat img = in_img.read().clone();

		skeletonize(img);

/*		cv::threshold(img, img, 127, 255, cv::THRESH_BINARY);

		cv::Mat skel(img.size(), CV_8UC1, cv::Scalar(0));
		cv::Mat temp;
		cv::Mat eroded;

		cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));

		bool done;
		do
		{
		  cv::morphologyEx(img, temp, cv::MORPH_OPEN, element);
		  cv::bitwise_not(temp, temp);
		  cv::bitwise_and(img, temp, temp);
		  cv::bitwise_or(skel, temp, skel);
		  cv::erode(img, img, element);

		  double max;
		  cv::minMaxLoc(img, 0, &max);
		  done = (max == 0);
		} while (!done);
*/
		// Write output to stream.
		out_img.write(img);
	}
	catch (Common::DisCODeException& ex) {
		CLOG(LERROR) << ex.what() << "\n";
		ex.printStackTrace();
		exit(EXIT_FAILURE);
	}
	catch (const char * ex) {
		CLOG(LERROR) << ex;
	}
	catch (...) {
		CLOG(LERROR) << "Skeletonization::onNewImage failed\n";
	}
}


} //: namespace Skeletonization
} //: namespace Processors
