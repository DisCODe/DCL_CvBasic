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

	prop_width("chessboard.board_width", 9),
	prop_height("chessboard.board_height", 6),
	prop_square_size("chessboard.square_size", 1),

	prop_inverse("image.inverse",false),

	prop_interpolation_type("scale.interpolation_type", INTER_NEAREST, "combo")
{

	findChessboardCornersFlags = 0;

	prop_width.setToolTip("Board width in squares");

	registerProperty(prop_width);
	registerProperty(prop_height);
	registerProperty(prop_square_size);
	registerProperty(prop_inverse);

	prop_width.setCallback(boost::bind(&CvFindCirclesGrid_Processor::sizeCallback, this, _1, _2));
	prop_height.setCallback(boost::bind(&CvFindCirclesGrid_Processor::sizeCallback, this, _1, _2));


#include "Property.hpp"
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
	registerStream("out_img", &out_img);

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
	LOG(LINFO) << "CvFindCirclesGrid_Processor: squareSize: " << prop_square_size << "x" << prop_square_size << "m\n";

	// Create chessboard object.
	chessboard = boost::shared_ptr <Chessboard>(new Chessboard(cv::Size(prop_width, prop_height)));

	// Initialize modelPoints - localization of the chessboard corners in Cartesian space.
	vector <Point3f> modelPoints;

	for( int i = 0; i < prop_height; i++ )
            for( int j = 0; j < prop_width; j++ )
                modelPoints.push_back(Point3f(float((2*j + i % 2)*prop_square_size/2),float(i*prop_square_size/2), 0));

	// Set model points.
	chessboard->setModelPoints(modelPoints);
	LOG(LWARNING) << chessboard->getModelPoints();
}

void CvFindCirclesGrid_Processor::sizeCallback(int old_value, int new_value) {
	initChessboard();
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
		Mat image = in_img.read().clone();

		if(prop_inverse){
			image =  cv::Scalar::all(255) - image;
		}

		bool found;

		// Initialize chessboard size.
		cv::Size chessboardSize(prop_width, prop_height);


		CLOG(LTRACE) << "Searching";
		found = findCirclesGrid(image, chessboardSize, corners, CALIB_CB_ASYMMETRIC_GRID|CALIB_CB_CLUSTERING);
		CLOG(LTRACE) << "Finished";

		// check if found, if not-invert colors and try again
		if(found){
			//LOG(LWARNING)<<"Dots found!!!\n\n\n";
			chessboard->setImagePoints(corners);
			out_chessboard.write(*chessboard);

		} else {
			LOG(LWARNING) << "chessboard not found\n";
			// TODO: add unit type: not found
		}
		out_img.write(image);
	} catch (const Exception& e) {
		LOG(LERROR) << e.what() << "\n";
	}


	
	LOG(LTRACE) << "void CvFindCirclesGrid_Processor::onNewImage() end\n";
}

} // namespace CvFindCirclesGrid{
} // namespace Processors {
