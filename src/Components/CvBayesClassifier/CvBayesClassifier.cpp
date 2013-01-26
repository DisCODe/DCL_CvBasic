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
		recognize("recognize", true),
		trainingClass("trainingClass", 1),
		filename("filename", boost::bind(&CvBayesClassifier::onFilenameChanged, this, _1, _2), name)
{
	// Register properties.
	registerProperty(recognize);
	registerProperty(trainingClass);
	registerProperty(filename);

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

	// Display.
	h_onDisplayDataset.setup(this, &CvBayesClassifier::onDisplayDataset);
	registerHandler("onDisplayDataset", &h_onDisplayDataset);

	// Save.
	h_onSaveBayes.setup(this, &CvBayesClassifier::onSaveBayes);
	registerHandler("onSaveBayes", &h_onSaveBayes);

	// Load.
	h_onLoadBayes.setup(this, &CvBayesClassifier::onLoadBayes);
	registerHandler("onLoadBayes", &h_onLoadBayes);

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
				CLOG(LNOTICE) << "Set of moments added to training dataset";
			}
			CLOG(LINFO) << "Size of training dataset: "<<training_dataset.size();
		}

		if(recognize)
		{
			// Prepare data structures for prediction.
			cv::Mat test_mat = cv::Mat::zeros( m.size(), 24, CV_32FC1);
			prepareSampleMatrix(m, test_mat);
			cv::Mat prediction = cv::Mat::zeros( 1, m.size(), CV_32FC1);
			CLOG(LNOTICE) << "Test matrix:\n"<<test_mat;
			// Predict!
			bayes.predict(test_mat, &prediction);
			CLOG(LNOTICE) << "Recognized classes: " << prediction;
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

void CvBayesClassifier::prepareSampleMatrix(const vector<Moments>& vector_, cv::Mat& mat_)
{
//	mat_ = cv::Mat::zeros( vector_.size(), 10, CV_32FC1);
	for(unsigned int i = 0; i < vector_.size(); i++ ){
		mat_.at<float>(i, 0) = (float) vector_[i].m00;
		mat_.at<float>(i, 1) = (float) vector_[i].m10;
		mat_.at<float>(i, 2) = (float) vector_[i].m01;
		mat_.at<float>(i, 3) = (float) vector_[i].m20;
		mat_.at<float>(i, 4) = (float) vector_[i].m11;
		mat_.at<float>(i, 5) = (float) vector_[i].m02;
		mat_.at<float>(i, 6) = (float) vector_[i].m30;
		mat_.at<float>(i, 7) = (float) vector_[i].m21;
		mat_.at<float>(i, 8) = (float) vector_[i].m12;
		mat_.at<float>(i, 9) = (float) vector_[i].m03;
			//! central moments
		mat_.at<float>(i, 10) = (float) vector_[i].mu20;
		mat_.at<float>(i, 11) = (float) vector_[i].mu11;
		mat_.at<float>(i, 12) = (float) vector_[i].mu02;
		mat_.at<float>(i, 13) = (float) vector_[i].mu30;
		mat_.at<float>(i, 14) = (float) vector_[i].mu21;
		mat_.at<float>(i, 15) = (float) vector_[i].mu12;
		mat_.at<float>(i, 16) = (float) vector_[i].mu03;
		//! central normalized moments
		mat_.at<float>(i, 17) = (float) vector_[i].nu20;
		mat_.at<float>(i, 18) = (float) vector_[i].nu11;
		mat_.at<float>(i, 19) = (float) vector_[i].nu02;
		mat_.at<float>(i, 20) = (float) vector_[i].nu30;
		mat_.at<float>(i, 21) = (float) vector_[i].nu21;
		mat_.at<float>(i, 22) = (float) vector_[i].nu12;
		mat_.at<float>(i, 23) = (float) vector_[i].nu03;
	}
}

void CvBayesClassifier::prepareResponseVector(cv::Mat& resp_mat_)
{
	for(unsigned int i = 0; i < training_responses.size(); i++ ){
		// class
		resp_mat_.at<float>(0,i) = (float) training_responses[i];
	}
}



void CvBayesClassifier::onTraining()
{
	CLOG(LTRACE) << "CvBayesClassifier::onTraining\n";
	try {
		// Train in here...
		//cv::Mat img(Size(320,240),CV_8UC3)
/*
		prepareSampleMatrix(training_dataset, train_mat);
		cv::Mat resp_mat;
		prepareResponseVector(resp_mat);
*/

		cv::Mat train_mat = cv::Mat::zeros( training_dataset.size(), 24, CV_32FC1);
		cv::Mat resp_mat = cv::Mat::zeros( 1, training_dataset.size(), CV_32FC1);
/*		prepareSampleMatrix(training_dataset, train_mat);
		prepareResponseVector(resp_mat);*/

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

		// Clear dataset.
//		onClearDataset();

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
		CLOG(LNOTICE) << "Training successful";

	} catch (...) {
		CLOG(LERROR) << "CvBayesClassifier::onTraining failed\n";
	}

}


void CvBayesClassifier::onFilenameChanged(const std::string & old_filename, const std::string & new_filename)
{
	filename = new_filename;
	CLOG(LTRACE) << "onFilenameChanged: " << std::string(filename) << std::endl;
}


void CvBayesClassifier::onDisplayDataset()
{
	CLOG(LTRACE) << "CvBayesClassifier::onDisplayDataset\n";
	cv::Mat train_mat = cv::Mat::zeros( training_dataset.size(), 24, CV_32FC1);
	cv::Mat resp_mat = cv::Mat::zeros( 1, training_dataset.size(), CV_32FC1);

	prepareSampleMatrix(training_dataset, train_mat);
	prepareResponseVector(resp_mat);

	CLOG(LNOTICE) << "Training matrix:\n"<<train_mat;
	CLOG(LNOTICE) << "Training response:\n"<<resp_mat;
}


void CvBayesClassifier::onSaveBayes()
{
	CLOG(LTRACE) << "CvBayesClassifier::onSaveBayes\n";
/*	std::string tmp = std::string("./") + std::string(filename);
	CLOG(LNOTICE) << " file: "<<std::string(tmp).c_str();
	bayes.save(std::string(tmp).c_str());*/
	bayes.save("test.yml");
	CLOG(LNOTICE) << "Bayes saved to xml file: "<<filename;
}


void CvBayesClassifier::onLoadBayes()
{
	CLOG(LTRACE) << "CvBayesClassifier::onLoadBayes\n";
//	bayes.load(std::string(filename).c_str());
	bayes.load("test.yml");
	CLOG(LNOTICE) << "Bayes loaded from the xml file: "<<filename;
}



} //: namespace CvBayesClassifier
} //: namespace Processors
