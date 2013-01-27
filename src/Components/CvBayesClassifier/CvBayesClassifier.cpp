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
	predict=false;
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
	
	// Predict data.
	h_onPredict.setup(this, &CvBayesClassifier::onPredict);
	registerHandler("onPredict", &h_onPredict);

	// Clear dataset.
	h_onClearDataset.setup(this, &CvBayesClassifier::onClearDataset);
	registerHandler("onClearDataset", &h_onClearDataset);

	// Training.
	h_onTraining.setup(this, &CvBayesClassifier::onTraining);
	registerHandler("onTraining", &h_onTraining);
	
	// Save.
	h_onSaveParams.setup(this, &CvBayesClassifier::onSaveParams);
	registerHandler("onSaveParams", &h_onSaveParams);
	
	// Load.
	h_onLoadParams.setup(this, &CvBayesClassifier::onLoadParams);
	registerHandler("onLoadParams", &h_onLoadParams);

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

		if (add) {
			add = false;
			// TODO
			for(std::vector<Moments>::iterator it = m.begin(); it != m.end(); ++it) {
			    /* std::cout << *it; ... */
				training_dataset.push_back((Moments)*it);
				training_responses.push_back(trainingClass);
				CLOG(LINFO) << "Set of moments added to training dataset";
			}
			CLOG(LINFO) << "Size of training dataset: "<<training_dataset.size();
		} else
		if (predict) {
			predict = false;
			cv::Mat test_mat = cv::Mat::zeros( 1, 24, CV_32FC1);
			
			test_mat.at<float>(0, 0) = (float) m[0].m00;
			test_mat.at<float>(0, 1) = (float) m[0].m10;
			test_mat.at<float>(0, 2) = (float) m[0].m01;
			test_mat.at<float>(0, 3) = (float) m[0].m20;
			test_mat.at<float>(0, 4) = (float) m[0].m11;
			test_mat.at<float>(0, 5) = (float) m[0].m02;
			test_mat.at<float>(0, 6) = (float) m[0].m30;
			test_mat.at<float>(0, 7) = (float) m[0].m21;
			test_mat.at<float>(0, 8) = (float) m[0].m12;
			test_mat.at<float>(0, 9) = (float) m[0].m03;
			//! central moments
			test_mat.at<float>(0, 10) = (float) m[0].mu20;
			test_mat.at<float>(0, 11) = (float) m[0].mu11;
			test_mat.at<float>(0, 12) = (float) m[0].mu02;
			test_mat.at<float>(0, 13) = (float) m[0].mu30;
			test_mat.at<float>(0, 14) = (float) m[0].mu21;
			test_mat.at<float>(0, 15) = (float) m[0].mu12;
			test_mat.at<float>(0, 16) = (float) m[0].mu03;
			//! central normalized moments
			test_mat.at<float>(0, 17) = (float) m[0].nu20;
			test_mat.at<float>(0, 18) = (float) m[0].nu11;
			test_mat.at<float>(0, 19) = (float) m[0].nu02;
			test_mat.at<float>(0, 20) = (float) m[0].nu30;
			test_mat.at<float>(0, 21) = (float) m[0].nu21;
			test_mat.at<float>(0, 22) = (float) m[0].nu12;
			test_mat.at<float>(0, 23) = (float) m[0].nu03;
			
			float resp = bayes.predict(test_mat);
			CLOG(LNOTICE) << "CvBayesClassifier::prediction: " << resp;
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
		cv::Mat train_mat = cv::Mat::zeros( training_dataset.size(), 24, CV_32FC1);
		cv::Mat resp_mat = cv::Mat::zeros( 1, training_dataset.size(), CV_32FC1);

		for(unsigned int i = 0; i < training_dataset.size(); i++ ){
			train_mat.at<float>(i, 0) = (float) training_dataset[i].m00;
			train_mat.at<float>(i, 1) = (float) training_dataset[i].m10;
			train_mat.at<float>(i, 2) = (float) training_dataset[i].m01;
			train_mat.at<float>(i, 3) = (float) training_dataset[i].m20;
			train_mat.at<float>(i, 4) = (float) training_dataset[i].m11;
			train_mat.at<float>(i, 5) = (float) training_dataset[i].m02;
			train_mat.at<float>(i, 6) = (float) training_dataset[i].m30;
			train_mat.at<float>(i, 7) = (float) training_dataset[i].m21;
			train_mat.at<float>(i, 8) = (float) training_dataset[i].m12;
			train_mat.at<float>(i, 9) = (float) training_dataset[i].m03;
			//! central moments
			train_mat.at<float>(i, 10) = (float) training_dataset[i].mu20;
			train_mat.at<float>(i, 11) = (float) training_dataset[i].mu11;
			train_mat.at<float>(i, 12) = (float) training_dataset[i].mu02;
			train_mat.at<float>(i, 13) = (float) training_dataset[i].mu30;
			train_mat.at<float>(i, 14) = (float) training_dataset[i].mu21;
			train_mat.at<float>(i, 15) = (float) training_dataset[i].mu12;
			train_mat.at<float>(i, 16) = (float) training_dataset[i].mu03;
			//! central normalized moments
			train_mat.at<float>(i, 17) = (float) training_dataset[i].nu20;
			train_mat.at<float>(i, 18) = (float) training_dataset[i].nu11;
			train_mat.at<float>(i, 19) = (float) training_dataset[i].nu02;
			train_mat.at<float>(i, 20) = (float) training_dataset[i].nu30;
			train_mat.at<float>(i, 21) = (float) training_dataset[i].nu21;
			train_mat.at<float>(i, 22) = (float) training_dataset[i].nu12;
			train_mat.at<float>(i, 23) = (float) training_dataset[i].nu03;

			// class
			resp_mat.at<float>(0,i) = (float) training_responses[i];
		}

		CLOG(LINFO) << "Training matrix:\n"<<train_mat;
		CLOG(LINFO) << "Training response:\n"<<resp_mat;

		// Train is coming! ;)
		bayes.train(train_mat, resp_mat);

/*		cv::Mat train = cv::Mat::zeros( 100, 32, CV_32FC1);//, cv::Scalar(CV_VAR_ORDERED));
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
		res.at<double>(29, 0) = (double) 2;*/

/*		CLOG(LINFO) << "train matrix:\n"<<train;
		CLOG(LINFO) << "res response:\n"<<res;*/

		//cvSet( train, cvScalarAll(CV_VAR_ORDERED));
		//cvSet( res, cvScalarAll(CV_VAR_CATEGORICAL));
//		bayes.train(train, res);
		CLOG(LINFO) << "Training successful";

	} catch (...) {
		CLOG(LERROR) << "CvBayesClassifier::onTraining failed\n";
	}

}

void CvBayesClassifier::onSaveParams() {
	bayes.save("test.yml");
}

void CvBayesClassifier::onLoadParams() {
	bayes.load("test.yml");
	bayes.save("test2.yml");
}

void CvBayesClassifier::onPredict() {
	predict = true;
}


} //: namespace CvBayesClassifier
} //: namespace Processors
