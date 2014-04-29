/*
 * CvFindCirclesGrid_Processor.cpp
 *
 *  Created on: 29-04-2014
 *      Author: lzmuda
 */

#include "CvFindCirclesGrid_Processor.hpp"
#include <boost/bind.hpp>

namespace Processors {

namespace CvFindCirclesGrid {

using namespace std;
using namespace boost;
using namespace cv;
using namespace Types::Objects3D;
static int counter= 0;

CvFindCirclesGrid_Processor::CvFindCirclesGrid_Processor(const std::string & name) :
	Component(name),
	prop_subpix("subpix.subpix", true),
	prop_subpix_window("subpix.window_size", 9, "range"),
	prop_scale("scale.scale", true),
	prop_scale_factor("scale.scale_factor", 2, "range"),

	prop_width("chessboard.board_width", 9),
	prop_height("chessboard.board_height", 6),
	prop_square_width("chessboard.square_width", 1),
	prop_square_height("chessboard.square_height", 1),

	prop_fastCheck("flags.fast_check", true),
	prop_filterQuads("flags.filter_quads", true),
	prop_adaptiveThreshold("flags.adaptive_treshold", true),
	prop_normalizeImage("flags.normalize_image", true),

	prop_interpolation_type("scale.interpolation_type", INTER_NEAREST, "combo")
{

	findChessboardCornersFlags = 0;

	registerProperty(prop_subpix);

	prop_subpix_window.addConstraint("3");
	prop_subpix_window.addConstraint("11");
	registerProperty(prop_subpix_window);

	registerProperty(prop_scale);

	prop_scale_factor.addConstraint("1");
	prop_scale_factor.addConstraint("8");
	registerProperty(prop_scale_factor);

	prop_width.setToolTip("Board width in squares");

	registerProperty(prop_width);
	registerProperty(prop_height);
	registerProperty(prop_square_width);
	registerProperty(prop_square_height);

	prop_fastCheck.setCallback(boost::bind(&CvFindCirclesGrid_Processor::flagsCallback, this, _1, _2));
	registerProperty(prop_fastCheck);

	prop_filterQuads.setCallback(boost::bind(&CvFindCirclesGrid_Processor::flagsCallback, this, _1, _2));
	registerProperty(prop_filterQuads);

	prop_adaptiveThreshold.setCallback(boost::bind(&CvFindCirclesGrid_Processor::flagsCallback, this, _1, _2));
	registerProperty(prop_adaptiveThreshold);

	prop_normalizeImage.setCallback(boost::bind(&CvFindCirclesGrid_Processor::flagsCallback, this, _1, _2));
	registerProperty(prop_normalizeImage);

	prop_width.setCallback(boost::bind(&CvFindCirclesGrid_Processor::sizeCallback, this, _1, _2));
	prop_height.setCallback(boost::bind(&CvFindCirclesGrid_Processor::sizeCallback, this, _1, _2));


	prop_interpolation_type.setToolTip("Interpolation type");
	PROP_ADD_COMBO_ITEMS(prop_interpolation_type, ELEMS);
	registerProperty(prop_interpolation_type);
}

CvFindCirclesGrid_Processor::~CvFindCirclesGrid_Processor()
{
}

void CvFindCirclesGrid_Processor::prepareInterface() {
	CLOG(LTRACE) << "CvFindCirclesGrid_Processor::prepareInterface\n";

	h_onNewImage.setup(this, &CvFindCirclesGrid_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);
	registerStream("out_chessboard", &out_chessboard);
	registerStream("out_imagePosition", &out_imagePosition);

	addDependency("onNewImage", &in_img);
}



bool CvFindCirclesGrid_Processor::onFinish()
{
	return true;
}

bool CvFindCirclesGrid_Processor::onStop()
{
	return true;
}

bool CvFindCirclesGrid_Processor::onInit()
{
	initChessboard();

	LOG(LTRACE) << "component initialized\n";
	return true;
}

void CvFindCirclesGrid_Processor::initChessboard() {
	LOG(LINFO) << "CvFindCirclesGrid_Processor: width: " << prop_width << "\n";
	LOG(LINFO) << "CvFindCirclesGrid_Processor: height: " << prop_height << "\n";
	LOG(LINFO) << "CvFindCirclesGrid_Processor: squareSize: " << prop_square_width << "x" << prop_square_height << "m\n";

	// Create chessboard object.
	chessboard = boost::shared_ptr <Chessboard>(new Chessboard(cv::Size(prop_width, prop_height)));

	// Initialize modelPoints - localization of the chessboard corners in Cartesian space.
	vector <Point3f> modelPoints;
	//chessboard
	/*for (int i = 0; i < prop_height; ++i) {
		for (int j = 0; j < prop_width; ++j) {
			modelPoints.push_back(Point3f(-j * prop_square_height, -i * prop_square_width, 0));
		}
	}*/
	// dots
	for( int i = 0; i < prop_height; i++ )
            for( int j = 0; j < prop_width; j++ )
                modelPoints.push_back(Point3f(float((2*j + i % 2)*prop_square_height),float(i*prop_square_width), 0));

	// Set model points.
	chessboard->setModelPoints(modelPoints);
}

void CvFindCirclesGrid_Processor::sizeCallback(int old_value, int new_value) {
	initChessboard();
}

void CvFindCirclesGrid_Processor::flagsCallback(bool old_value, bool new_value) {
	// Set flags.
	if(prop_fastCheck){
		findChessboardCornersFlags |= CV_CALIB_CB_FAST_CHECK;
	} else {
		findChessboardCornersFlags &= ~CV_CALIB_CB_FAST_CHECK;
	}

	if(prop_filterQuads){
		findChessboardCornersFlags |= CV_CALIB_CB_FILTER_QUADS;
	} else {
		findChessboardCornersFlags &= ~CV_CALIB_CB_FILTER_QUADS;
	}

	if(prop_adaptiveThreshold){
		findChessboardCornersFlags |= CV_CALIB_CB_ADAPTIVE_THRESH;
	} else {
		findChessboardCornersFlags &= ~CV_CALIB_CB_ADAPTIVE_THRESH;
	}

	if(prop_normalizeImage){
		findChessboardCornersFlags |= CV_CALIB_CB_NORMALIZE_IMAGE;
	} else {
		findChessboardCornersFlags &= ~CV_CALIB_CB_NORMALIZE_IMAGE;
	}
}

bool CvFindCirclesGrid_Processor::onStart()
{
	return true;
}

bool CvFindCirclesGrid_Processor::onStep()
{
	return true;
}

void CvFindCirclesGrid_Processor::onNewImage()
{
	LOG(LTRACE) << "void CvFindChessboardCorners_Processor::onNewImage() begin\n";
	try {
		if(in_img.empty()){
			return;
		}
		// Retrieve image from the stream.
		Mat image = in_img.read();

		timer.restart();

		cv::resize(image, sub_img, Size(), 1.0 / prop_scale_factor, 1.0 / prop_scale_factor, prop_interpolation_type);

		bool found;

		// Initialize chessboard size.
		cv::Size chessboardSize(prop_width, prop_height);

		// Find chessboard corners.
		if (prop_scale) {
			found = findCirclesGrid(sub_img, chessboardSize, corners, CALIB_CB_ASYMMETRIC_GRID);
			//found = findChessboardCorners(sub_img, chessboardSize, corners, findChessboardCornersFlags);
			for (size_t i = 0; i < corners.size(); ++i) {
				corners[i].x *= prop_scale_factor;
				corners[i].y *= prop_scale_factor;
			}

		} else {
			found = findCirclesGrid(image, chessboardSize, corners, CALIB_CB_ASYMMETRIC_GRID);
			//found = findChessboardCorners(image, chessboardSize, corners, findChessboardCornersFlags);
		}

		// check if found, if not-invert colors and try again
		if(found){
			std::cout<<"Dots founded!!!\n\n\n";
		}/*
		else{
			// invert colors
			cv::Size size = image.size();
			std::cout<<"Clone to tmp\n";
			Mat tmp_img=image.clone();
			if (image.isContinuous() && tmp_img.isContinuous()) {
        			size.width *= size.height;
        			size.height = 1;
        		}
        		size.width *= 3;
			for (int i = 0; i < size.height; i++)
			{
			    const uchar* rgb_p = image.ptr <uchar> (i);
			    uchar* tmp_p = tmp_img.ptr <uchar> (i);
			    int j, k = 0;
			    for (j = 0; j < size.width; j += 3)
			    {
				tmp_p[j] = 255-rgb_p[j];
				tmp_p[j+1]  = 255-rgb_p[j + 1];
				tmp_p[j+2]  = 255-rgb_p[j + 2];
			    }
			}
			//std::cout<<"Write\n";
			//out_img.write(tmp_img);

			// try again
			if (prop_scale) {
			found = findCirclesGrid(sub_img, chessboardSize, corners, CALIB_CB_ASYMMETRIC_GRID);
			//found = findChessboardCorners(sub_img, chessboardSize, corners, findChessboardCornersFlags);
			for (size_t i = 0; i < corners.size(); ++i) {
				corners[i].x *= prop_scale_factor;
				corners[i].y *= prop_scale_factor;
			}

			} else {
				found = findCirclesGrid(image, chessboardSize, corners, CALIB_CB_ASYMMETRIC_GRID);
				//found = findChessboardCorners(image, chessboardSize, corners, findChessboardCornersFlags);
			}

			std::cout<<"D\n";
		}
		*/
		LOG(LTRACE) << "findChessboardCorners() execution time: " << timer.elapsed() << " s\n";

		if (found) {
			LOG(LTRACE) << "chessboard found\n";

			// Perform subpix pose update.
			if (prop_subpix) {
				cornerSubPix(image, corners, Size(prop_subpix_window, prop_subpix_window), Size(1, 1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 50, 1e-3));
			}

			// Set image points and write the result to output stream.

			/*if(counter==5)
			{
				for(unsigned int i = 0; i < corners.size(); ++i)
				{
					std::cout << "Image corner: " << corners[i] << std::endl;
				}
				counter=0;
					
			}
			*/
			
			chessboard->setImagePoints(corners);
			out_chessboard.write(*chessboard);

			//std::vector <cv::Point2f> corners2 = chessboard->getImagePoints();
			//counter++;

			// Recalculate the image chessboard pose in form of <X,Y,0,alpha>.
			Types::ImagePosition imagePosition;
			double maxPixels = std::max(image.size().width, image.size().height);
			imagePosition.elements[0] = (corners[0].x - image.size().width / 2) / maxPixels;
			imagePosition.elements[1] = (corners[0].y - image.size().height / 2) / maxPixels;
			imagePosition.elements[2] = 0;
			imagePosition.elements[3] = - atan2(corners[1].y - corners[0].y, corners[1].x - corners[0].x);
			// Write output stream.
			out_imagePosition.write(imagePosition);

			// TODO: add unit type: found
		} else {
			LOG(LTRACE) << "chessboard not found\n";
			// TODO: add unit type: not found

		}
	} catch (const Exception& e) {
		LOG(LERROR) << e.what() << "\n";
	}

	
	
	LOG(LTRACE) << "void CvFindCirclesGrid_Processor::onNewImage() end\n";
}

} // namespace CvFindCirclesGrid{
} // namespace Processors {
