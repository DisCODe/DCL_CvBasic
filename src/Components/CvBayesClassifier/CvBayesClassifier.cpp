/*!
 * \file
 * \brief
 * \author Tomek Kornuta,,,
 */

#include <memory>
#include <string>

#include "CvBayesClassifier.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvBayesClassifier {

CvBayesClassifier::CvBayesClassifier(const std::string & name) :
		Base::Component(name),
		trainingClass("trainingClass", 1)
{
	// Register properties.
	registerProperty(trainingClass);

	add=false;
}

CvBayesClassifier::~CvBayesClassifier() {
}

void CvBayesClassifier::prepareInterface() {
	// Register handlers with their dependencies.
	h_onNewData.setup(this, &CvBayesClassifier::onNewData);
	registerHandler("onNewData", &h_onNewData);
	addDependency("onNewData", &in_moments);

	// Add to dataset.
	h_onAddToDataset.setup(this, &CvBayesClassifier::onAddToDataset);
	registerHandler("onAddToDataset", &h_onAddToDataset);

	// Clear dataset.
	h_onClearDataset.setup(this, &CvBayesClassifier::onClearDataset);
	registerHandler("onClearDataset", &h_onClearDataset);

	// Training.
	h_onTraining.setup(this, &CvBayesClassifier::onTraining);
	registerHandler("onTraining", &h_onTraining);

	// Input data stream.
	registerStream("in_moments", &in_moments);

}

bool CvBayesClassifier::onInit() {

	return true;
}

bool CvBayesClassifier::onFinish() {
	return true;
}

bool CvBayesClassifier::onStop() {
	return true;
}

bool CvBayesClassifier::onStart() {
	return true;
}


void CvBayesClassifier::onNewData()
{
	CLOG(LTRACE) << "CvBayesClassifier::onNewData\n";
	try {
		// Read input - moments.
		vector<Moments> m = in_moments.read();

		if (add)
		{
			add = false;
			// TODO
			for(std::vector<Moments>::iterator it = m.begin(); it != m.end(); ++it) {
			    /* std::cout << *it; ... */
				training_dataset.push_back((Moments)*it);
				training_responses.push_back(trainingClass);
				CLOG(LINFO) << "Set of moments added to training dataset";
			}
			CLOG(LINFO) << "Size of training dataset: "<<training_dataset.size();
		}



	} catch (...) {
		CLOG(LERROR) << "CvBayesClassifier::onNewData failed\n";
	}
}

void CvBayesClassifier::onAddToDataset()
{
	CLOG(LTRACE) << "CvBayesClassifier::onAddToDataset\n";
	add = true;
}

void CvBayesClassifier::onClearDataset()
{
	CLOG(LTRACE) << "CvBayesClassifier::onClearDataset\n";
	training_dataset.clear();
	training_responses.clear();
	bayes.clear();
	CLOG(LINFO) << "Training dataset cleared";
}


void CvBayesClassifier::onTraining()
{
	CLOG(LTRACE) << "CvBayesClassifier::onTraining\n";
	try {
		// Train in here...
		//cv::Mat img(Size(320,240),CV_8UC3)
		cv::Mat train_mat = cv::Mat::zeros( training_dataset.size(), 10, CV_32FC1);
		cv::Mat resp_mat = cv::Mat::zeros( 1, training_dataset.size(), CV_32FC1);

		for(unsigned int i = 0; i < training_dataset.size(); i++ ){
/*			train_mat.at<double>(i, 0) = (double) training_dataset[i].m00;
			train_mat.at<double>(i, 1) = (double) training_dataset[i].m10;
			train_mat.at<double>(i, 2) = (double) training_dataset[i].m01;
			train_mat.at<double>(i, 3) = (double) training_dataset[i].m20;
			train_mat.at<double>(i, 4) = (double) training_dataset[i].m11;
			train_mat.at<double>(i, 5) = (double) training_dataset[i].m02;
			train_mat.at<double>(i, 6) = (double) training_dataset[i].m30;
			train_mat.at<double>(i, 7) = (double) training_dataset[i].m21;
			train_mat.at<double>(i, 8) = (double) training_dataset[i].m12;
			train_mat.at<double>(i, 9) = (double) training_dataset[i].m03;
			//! central moments
			train_mat.at<double>(i, 10) = (double) training_dataset[i].mu20;
			train_mat.at<double>(i, 11) = (double) training_dataset[i].mu11;
			train_mat.at<double>(i, 12) = (double) training_dataset[i].mu02;
			train_mat.at<double>(i, 13) = (double) training_dataset[i].mu30;
			train_mat.at<double>(i, 14) = (double) training_dataset[i].mu21;
			train_mat.at<double>(i, 15) = (double) training_dataset[i].mu12;
			train_mat.at<double>(i, 16) = (double) training_dataset[i].mu03;
			//! central normalized moments
			train_mat.at<double>(i, 17) = (double) training_dataset[i].nu20;
			train_mat.at<double>(i, 18) = (double) training_dataset[i].nu11;
			train_mat.at<double>(i, 19) = (double) training_dataset[i].nu02;
			train_mat.at<double>(i, 20) = (double) training_dataset[i].nu30;
			train_mat.at<double>(i, 21) = (double) training_dataset[i].nu21;
			train_mat.at<double>(i, 22) = (double) training_dataset[i].nu12;
			train_mat.at<double>(i, 23) = (double) training_dataset[i].nu03;*/

			// class
			resp_mat.at<double>(0,i) = (double) training_responses[i];
		}

		CLOG(LINFO) << "Training matrix:\n"<<train_mat;
		CLOG(LINFO) << "Training response:\n"<<resp_mat;

		// Train is coming! ;)
//		bayes.train(train_mat, resp_mat);

		cv::Mat train = cv::Mat::zeros( 100, 32, CV_32FC1);//, cv::Scalar(CV_VAR_ORDERED));
		train.at<double>(0, 0) = (double) 2;
		train.at<double>(0, 1) = (double) 5;
		train.at<double>(1, 17) = (double) 12;
		train.at<double>(1, 9) = (double) 235;
		train.at<double>(29, 1) = (double) 645;
		train.at<double>(34, 12) = (double) 65;
		train.at<double>(23, 3) = (double) 2.64;
		train.at<double>(27, 8) = (double) 5443;
		train.at<double>(3, 7) = (double) 125432;
		train.at<double>(67, 14) = (double) 6533;
		train.at<double>(78, 18) = (double) 43265;
		train.at<double>(92, 12) = (double) 65.543;
		cv::Mat res = cv::Mat::ones( 1, 100, CV_32FC1);//, cv::Scalar(CV_VAR_CATEGORICAL) );
		res.at<double>(0, 0) = (double) 2;
		res.at<double>(1, 0) = (double) 2;
		res.at<double>(29, 0) = (double) 2;

/*		CLOG(LINFO) << "train matrix:\n"<<train;
		CLOG(LINFO) << "res response:\n"<<res;*/

		//cvSet( train, cvScalarAll(CV_VAR_ORDERED));
		//cvSet( res, cvScalarAll(CV_VAR_CATEGORICAL));
		bayes.train(train, res);
		CLOG(LINFO) << "Training successful";

	} catch (...) {
		CLOG(LERROR) << "CvBayesClassifier::onTraining failed\n";
	}

}



} //: namespace CvBayesClassifier
} //: namespace Processors
