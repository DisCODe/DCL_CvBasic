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
			continuousRecognition("Recognition.continuous", false),
			continuousCollection("Training.continuous samples collection", false),
			trainingClass("Training.class", 1),
			filename("Classifier.filename", boost::bind(&CvBayesClassifier::onFilenameChanged, this, _1, _2), name),
			use_spatial("Classifier.Spatial moments", true),
			use_central("Classifier.Central moments", true),
			use_normalized_central("Classifier.Normalized central moments", true)
{
	// Register properties.
	registerProperty(continuousRecognition);
	registerProperty(continuousCollection);
	registerProperty(trainingClass);
	registerProperty(use_spatial);
	registerProperty(use_central);
	registerProperty(use_normalized_central);
	registerProperty(filename);

	// Initialize variables.
	add = false;
	number_of_features = 24;
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
	registerHandler("Training dataset add sample", &h_onAddToDataset);

	// Clear dataset.
	h_onClearDataset.setup(this, &CvBayesClassifier::onClearDataset);
	registerHandler("Training dataset clear", &h_onClearDataset);

	// Display dataset.
	h_onDisplayDataset.setup(this, &CvBayesClassifier::onDisplayDataset);
	registerHandler("Training dataset display", &h_onDisplayDataset);

	// Bayes: clear.
	h_onBayesClear.setup(this, &CvBayesClassifier::onBayesClear);
	registerHandler("Bayes reset", &h_onBayesClear);

	// Bayes: Training.
	h_onBayesTraining.setup(this, &CvBayesClassifier::onBayesTraining);
	registerHandler("Bayes train classifier with dataset", &h_onBayesTraining);

	// Bayes: Save.
	h_onBayesSave.setup(this, &CvBayesClassifier::onBayesSave);
	registerHandler("Bayes save parameters to file", &h_onBayesSave);

	// Bayes: Load.
	h_onBayesLoad.setup(this, &CvBayesClassifier::onBayesLoad);
	registerHandler("Bayes load parameters from file", &h_onBayesLoad);

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


bool CvBayesClassifier::isAlreadyPresent(const vector<Moments>& dataset_, const Moments &m_)
{
    for (std::vector<Moments>::const_iterator it = dataset_.begin(); it != dataset_.end(); ++it)
    {
    	bool present = true;
		if (it->m00 != m_.m00)
			present = false;
		if (it->m10 != m_.m10)
			present = false;
		if (it->m01 != m_.m01)
			present = false;
		if (it->m11 != m_.m11)
			present = false;
		if (it->m20 != m_.m20)
			present = false;
		if (it->m02 != m_.m02)
			present = false;
		if (it->m30 != m_.m30)
			present = false;
		if (it->m21 != m_.m21)
			present = false;
		if (it->m12 != m_.m12)
			present = false;
		if (it->m03 != m_.m03)
			present = false;

		if (it->mu20 != m_.mu20)
			present = false;
		if (it->mu11 != m_.mu11)
			present = false;
		if (it->mu02 != m_.mu02)
			present = false;
		if (it->mu30 != m_.mu30)
			present = false;
		if (it->mu21 != m_.mu21)
			present = false;
		if (it->mu12 != m_.mu12)
			present = false;
		if (it->mu03 != m_.mu03)
			present = false;

		if (it->nu20 != m_.nu20)
			present = false;
		if (it->nu11 != m_.nu11)
			present = false;
		if (it->nu02 != m_.nu02)
			present = false;
		if (it->nu30 != m_.nu30)
			present = false;
		if (it->nu21 != m_.nu21)
			present = false;
		if (it->nu12 != m_.nu12)
			present = false;
		if (it->nu03 != m_.nu03)
			present = false;

		if (present)
			return true;
    }
    // Identical set of moments not found.
    return false;
}

void CvBayesClassifier::onNewData() {
	CLOG(LTRACE) << "CvBayesClassifier::onNewData\n";
	try {
		// Read input - moments.
		vector<Moments> m = in_moments.read();

		// Check number of features.
		// ?

		if (add || continuousCollection) {
			add = false;
			// Add all samples to training dataset along with the currently set class.
			for (std::vector<Moments>::iterator it = m.begin(); it != m.end(); ++it) {
				if (!isAlreadyPresent(training_dataset,(Moments) *it)) {
					training_dataset.push_back((Moments) *it);
					training_responses.push_back(trainingClass);
					CLOG(LNOTICE) << "Set of moments added to the training dataset";
				}
				else
					CLOG(LERROR) << "Set of moments not added: set already present in the training dataset";

			}
			CLOG(LINFO) << "Size of training dataset: "
					<< training_dataset.size();
		}

		if (continuousRecognition) {
			// Prepare data structures for prediction.
			cv::Mat test_mat = cv::Mat::zeros(m.size(), number_of_features, CV_32FC1);
			//prepareSampleMatrix(m, test_mat);
			for (unsigned int i = 0; i < m.size(); i++) {
				if (use_spatial) {
					test_mat.at<float> (i, 0) = (float) m[i].m00;
					test_mat.at<float> (i, 1) = (float) m[i].m10;
					test_mat.at<float> (i, 2) = (float) m[i].m01;
					test_mat.at<float> (i, 3) = (float) m[i].m20;
					test_mat.at<float> (i, 4) = (float) m[i].m11;
					test_mat.at<float> (i, 5) = (float) m[i].m02;
					test_mat.at<float> (i, 6) = (float) m[i].m30;
					test_mat.at<float> (i, 7) = (float) m[i].m21;
					test_mat.at<float> (i, 8) = (float) m[i].m12;
					test_mat.at<float> (i, 9) = (float) m[i].m03;
				}
				//! central moments
				if (use_central) {
					test_mat.at<float> (i, 10) = (float) m[i].mu20;
					test_mat.at<float> (i, 11) = (float) m[i].mu11;
					test_mat.at<float> (i, 12) = (float) m[i].mu02;
					test_mat.at<float> (i, 13) = (float) m[i].mu30;
					test_mat.at<float> (i, 14) = (float) m[i].mu21;
					test_mat.at<float> (i, 15) = (float) m[i].mu12;
					test_mat.at<float> (i, 16) = (float) m[i].mu03;
				}
				//! central normalized moments
				if (use_normalized_central) {
					test_mat.at<float> (i, 17) = (float) m[i].nu20;
					test_mat.at<float> (i, 18) = (float) m[i].nu11;
					test_mat.at<float> (i, 19) = (float) m[i].nu02;
					test_mat.at<float> (i, 20) = (float) m[i].nu30;
					test_mat.at<float> (i, 21) = (float) m[i].nu21;
					test_mat.at<float> (i, 22) = (float) m[i].nu12;
					test_mat.at<float> (i, 23) = (float) m[i].nu03;
				}
			}

			cv::Mat prediction = cv::Mat::zeros(1, m.size(), CV_32FC1);
			CLOG(LNOTICE) << "Test matrix:\n" << test_mat;
			// Predict!
			bayes.predict(test_mat, &prediction);
			CLOG(LNOTICE) << "Recognized classes: " << prediction;
		}

	} catch (...) {
		CLOG(LERROR) << "CvBayesClassifier::onNewData failed\n";
	}
}

void CvBayesClassifier::prepareSampleMatrix(const vector<Moments>& vector_,cv::Mat& mat_)
{
	vector<Moments>::const_iterator it;
	int i = 0;
    for (it = vector_.begin(); it != vector_.end(); ++it, ++i) {
        //std::cout << *it << ' ';
		if (use_spatial) {
			mat_.at<float> (i, 0) = (float) it->m00;
			mat_.at<float> (i, 1) = (float) it->m10;
			mat_.at<float> (i, 2) = (float) it->m01;
			mat_.at<float> (i, 3) = (float) it->m20;
			mat_.at<float> (i, 4) = (float) it->m11;
			mat_.at<float> (i, 5) = (float) it->m02;
			mat_.at<float> (i, 6) = (float) it->m30;
			mat_.at<float> (i, 7) = (float) it->m21;
			mat_.at<float> (i, 8) = (float) it->m12;
			mat_.at<float> (i, 9) = (float) it->m03;
		}
		//! central moments
		if (use_central) {
			mat_.at<float> (i, 10) = (float) it->mu20;
			mat_.at<float> (i, 11) = (float) it->mu11;
			mat_.at<float> (i, 12) = (float) it->mu02;
			mat_.at<float> (i, 13) = (float) it->mu30;
			mat_.at<float> (i, 14) = (float) it->mu21;
			mat_.at<float> (i, 15) = (float) it->mu12;
			mat_.at<float> (i, 16) = (float) it->mu03;
		}
		//! central normalized moments
		if (use_normalized_central) {
			mat_.at<float> (i, 17) = (float) it->nu20;
			mat_.at<float> (i, 18) = (float) it->nu11;
			mat_.at<float> (i, 19) = (float) it->nu02;
			mat_.at<float> (i, 20) = (float) it->nu30;
			mat_.at<float> (i, 21) = (float) it->nu21;
			mat_.at<float> (i, 22) = (float) it->nu12;
			mat_.at<float> (i, 23) = (float) it->nu03;
		}
	}
}

void CvBayesClassifier::prepareResponseVector(cv::Mat& resp_mat_) {
	for (unsigned int i = 0; i < training_responses.size(); i++) {
		// class
		resp_mat_.at<float> (0, i) = (float) training_responses[i];
	}
}


void CvBayesClassifier::onAddToDataset() {
	CLOG(LTRACE) << "CvBayesClassifier::onAddToDataset\n";
	add = true;
}


void CvBayesClassifier::onClearDataset() {
	CLOG(LTRACE) << "CvBayesClassifier::onClearDataset\n";
	training_dataset.clear();
	training_responses.clear();
	CLOG(LINFO) << "Training dataset cleared";
}

void CvBayesClassifier::onBayesClear() {
	CLOG(LTRACE) << "CvBayesClassifier::onBayesClear\n";
	bayes.clear();
	CLOG(LINFO) << "Bayes classifer cleared";
}


void CvBayesClassifier::onBayesTraining() {
	CLOG(LTRACE) << "CvBayesClassifier::onTraining\n";
	try {
		cv::Mat train_mat = cv::Mat::zeros(training_dataset.size(), number_of_features,
				CV_32FC1);
		cv::Mat resp_mat = cv::Mat::zeros(1, training_dataset.size(), CV_32FC1);
		prepareSampleMatrix(training_dataset, train_mat);
		prepareResponseVector(resp_mat);

		CLOG(LINFO) << "Training matrix:\n" << train_mat;
		CLOG(LINFO) << "Training response:\n" << resp_mat;

		// Train is coming! ;)
		bayes.train(train_mat, resp_mat);

		// Clear dataset.
		//		onClearDataset();
		CLOG(LNOTICE) << "Training successful";

	} catch (...) {
		CLOG(LERROR) << "CvBayesClassifier::onTraining failed\n";
	}

}

void CvBayesClassifier::onFilenameChanged(const std::string & old_filename,
		const std::string & new_filename) {
	filename = new_filename;
	CLOG(LTRACE) << "onFilenameChanged: " << std::string(filename) << std::endl;
}

void CvBayesClassifier::onDisplayDataset() {
	CLOG(LTRACE) << "CvBayesClassifier::onDisplayDataset\n";
	cv::Mat train_mat = cv::Mat::zeros(training_dataset.size(), number_of_features, CV_32FC1);
	cv::Mat resp_mat = cv::Mat::zeros(1, training_dataset.size(), CV_32FC1);

//	CLOG(LNOTICE) << "Training vector:\n" << training_dataset;

	prepareSampleMatrix(training_dataset, train_mat);
	prepareResponseVector(resp_mat);

	CLOG(LNOTICE) << "Training matrix:\n" << train_mat;
	CLOG(LNOTICE) << "Training response:\n" << resp_mat;
}

void CvBayesClassifier::onBayesSave() {
	try {
		CLOG(LTRACE) << "CvBayesClassifier::onSaveBayes\n";
		std::string tmp = std::string("./") + std::string(filename);
		 /* CLOG(LNOTICE) << " file: "<<std::string(tmp).c_str();*/
		bayes.save(std::string(tmp).c_str());
		//bayes.save("test.yml");
		CLOG(LNOTICE) << "Bayes saved to xml file: " << filename;
	} catch (...) {
		CLOG(LERROR) << "CvBayesClassifier::onSaveBayes failed\n";
	}
}

void CvBayesClassifier::onBayesLoad() {
	try {
		CLOG(LTRACE) << "CvBayesClassifier::onLoadBayes\n";
		bayes.load(std::string(filename).c_str());
		//	bayes.load("test.yml");
		CLOG(LNOTICE) << "Bayes loaded from the xml file: " << filename;
	} catch (...) {
		CLOG(LERROR) << "CvBayesClassifier::onLoadBayes failed\n";
	}
}

} //: namespace CvBayesClassifier
} //: namespace Processors
