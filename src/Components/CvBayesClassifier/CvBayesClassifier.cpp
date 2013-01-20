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
		cv::Mat train_mat(Size(24,training_dataset.size()),CV_32FC1);
		cv::Mat resp_mat(Size(1,training_dataset.size()),CV_8UC1);

		for(unsigned int i = 0; i < training_dataset.size(); i++ ){
			train_mat.at<float>(i, 0) = training_dataset[i].m00;
			train_mat.at<float>(i, 1) = training_dataset[i].m10;
			train_mat.at<float>(i, 2) = training_dataset[i].m01;
			train_mat.at<float>(i, 3) = training_dataset[i].m20;
			train_mat.at<float>(i, 4) = training_dataset[i].m11;
			train_mat.at<float>(i, 5) = training_dataset[i].m02;
			train_mat.at<float>(i, 6) = training_dataset[i].m30;
			train_mat.at<float>(i, 7) = training_dataset[i].m21;
			train_mat.at<float>(i, 8) = training_dataset[i].m12;
			train_mat.at<float>(i, 9) = training_dataset[i].m03;
			//! central moments
			train_mat.at<float>(i, 10) = training_dataset[i].mu20;
			train_mat.at<float>(i, 11) = training_dataset[i].mu11;
			train_mat.at<float>(i, 12) = training_dataset[i].mu02;
			train_mat.at<float>(i, 13) = training_dataset[i].mu30;
			train_mat.at<float>(i, 14) = training_dataset[i].mu21;
			train_mat.at<float>(i, 15) = training_dataset[i].mu12;
			train_mat.at<float>(i, 16) = training_dataset[i].mu03;
			//! central normalized moments
			train_mat.at<float>(i, 17) = training_dataset[i].nu20;
			train_mat.at<float>(i, 18) = training_dataset[i].nu11;
			train_mat.at<float>(i, 19) = training_dataset[i].nu02;
			train_mat.at<float>(i, 20) = training_dataset[i].nu30;
			train_mat.at<float>(i, 21) = training_dataset[i].nu21;
			train_mat.at<float>(i, 22) = training_dataset[i].nu12;
			train_mat.at<float>(i, 23) = training_dataset[i].nu03;

			// class
			resp_mat.at<uchar>(i, 0) = training_responses[i];
		}

		CLOG(LINFO) << "Training matrix:\n"<<train_mat;
		CLOG(LINFO) << "Training response:\n"<<resp_mat;

		// Train is coming! ;)
		//bayes.train(train_mat, resp_mat, Mat(), Mat(), false);

	} catch (...) {
		CLOG(LERROR) << "CvBayesClassifier::onTraining failed\n";
	}

}



} //: namespace CvBayesClassifier
} //: namespace Processors
