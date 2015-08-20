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

	prop_width("grid.width", 9),
	prop_height("grid.height", 6),
	prop_size("grid.size", 1),
	
	//you should use 0/1 not true/false in *.xml task file
	prop_inverse("image.inverse",false),

	prop_interpolation_type("scale.interpolation_type", INTER_NEAREST, "combo")
{
	registerProperty(prop_width);
	registerProperty(prop_height);
	registerProperty(prop_size);
	registerProperty(prop_inverse);

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

	// Register data streams.
	registerStream("in_img", &in_img);
	registerStream("out_pattern", &out_gridPattern);
	registerStream("out_imagePosition", &out_imagePosition);

	// Register handler responsible for finding grid pattern.
	registerHandler("onNewImage", boost::bind(&CvFindCirclesGrid_Processor::onNewImage, this));
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
	initGridPattern();

	CLOG(LTRACE) << "component initialized\n";
	return true;
}

void CvFindCirclesGrid_Processor::initGridPattern() {
	CLOG(LINFO) << "CvFindCirclesGrid_Processor: width: " << prop_width;
	CLOG(LINFO) << "CvFindCirclesGrid_Processor: height: " << prop_height;
	CLOG(LINFO) << "CvFindCirclesGrid_Processor: size: " << prop_size;

	// Create grid object.
	gridPattern = boost::shared_ptr <GridPattern>(new GridPattern(cv::Size(prop_width, prop_height)));

	// Initialize modelPoints - localization of the grid points in Cartesian space.
	vector <Point3f> modelPoints;

	for( int i = 0; i < prop_height; i++ )
            for( int j = 0; j < prop_width; j++ )
                modelPoints.push_back(Point3f(float((2*j + i % 2)*prop_size/2),float(i*prop_size/2), 0));

	// Set model points.
	gridPattern->setModelPoints(modelPoints);
}

void CvFindCirclesGrid_Processor::sizeCallback(int old_value, int new_value) {
	initGridPattern();
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
	CLOG(LTRACE) << "void CvFindCirclesGrid_Processor::onNewImage() begin\n";
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

		// Initialize grid size.
		cv::Size gridSize(prop_width, prop_height);

		found = findCirclesGrid(image, gridSize, gridPoints, CALIB_CB_ASYMMETRIC_GRID|CALIB_CB_CLUSTERING);

		// check if found, if not-invert colors and try again
		if(found){
			//LOG(LWARNING)<<"Dots found!!!\n\n\n";
			gridPattern->setImagePoints(gridPoints);
			out_gridPattern.write(*gridPattern);

		} else {
			CLOG(LWARNING) << "Pattern not found\n";
			// TODO: add unit type: not found
		}
		
	} catch (const Exception& e) {
		CLOG(LERROR) << e.what() << "\n";
	}
	
	LOG(LTRACE) << "void CvFindCirclesGrid_Processor::onNewImage() end\n";
}

} // namespace CvFindCirclesGrid{
} // namespace Processors {
