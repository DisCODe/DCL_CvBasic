/*!
 * \file
 * \brief 
 * \author Tomek Kornuta,,,
 */

#ifndef CVBAYESCLASSIFIER_HPP_
#define CVBAYESCLASSIFIER_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"

#include "set"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;

namespace Processors {
namespace CvBayesClassifier {


/*!
 * \class CvBayesClassifier
 * \brief CvBayesClassifier processor class.
 *
 * CvBayesClassifier processor.
 */
class CvBayesClassifier: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CvBayesClassifier(const std::string & name = "CvBayesClassifier");

	/*!
	 * Destructor
	 */
	virtual ~CvBayesClassifier();

	/*!
	 * Prepare components interface (register streams and handlers).
	 * At this point, all properties are already initialized and loaded to 
	 * values set in config file.
	 */
	void prepareInterface();

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();

	/// Returns matrix with samples.
	void prepareSampleMatrix(const vector<Moments>& vector_, cv::Mat& mat_);

	/// Returns vector with responses.
	void prepareResponseVector(cv::Mat& resp_mat_);


	/// Event handler.
	Base::EventHandler<CvBayesClassifier> h_onBayesTraining;
	/// Train the classifier with the possessed dataset.
	void onBayesTraining();


	// Handler activated when user will trigger "add to dataset"
	Base::EventHandler<CvBayesClassifier> h_onAddToDataset;
	// Sets the add_set flag.
	void onAddToDataset();


	// Handler activated when user will trigger "clear whole dataset"
	Base::EventHandler<CvBayesClassifier> h_onClearDataset;
	// Adds received observation to dataset.
	void onClearDataset();


	// Handler activated when user will trigger "display dataset"
	Base::EventHandler<CvBayesClassifier> h_onDisplayDataset;
	// Displays the dataset.
	void onDisplayDataset();

	// Updates filename
	void onFilenameChanged(const std::string & old_filename, const std::string & new_filename);

	// Handler activated when user will trigger "save bayes"
	Base::EventHandler<CvBayesClassifier> h_onBayesSave;
	// Saves the bayes internal state to an xml file.
	void onBayesSave();

	// Handler activated when user will trigger "load bayes"
	Base::EventHandler<CvBayesClassifier> h_onBayesLoad;
	// Loads the model state from an xml file.
	void onBayesLoad();

	// Handler activated when user will trigger "Clear bayes"
	Base::EventHandler<CvBayesClassifier> h_onBayesClear;
	// Loads clears the bayes settings.
	void onBayesClear();

	/// Event handler.
	Base::EventHandler<CvBayesClassifier> h_onNewData;
	/// Event handler function.
	void onNewData();

	/// Input data stream
	Base::DataStreamIn<vector<Moments> > in_moments;

	// Flag: if set, the Bayes uses spatial moments.
	Base::Property<bool> use_spatial;

	// Flag: if set, the bayes uses central moments.
	Base::Property<bool> use_central;

	// Flag: if set, the bayes uses nomalized central moments.
	Base::Property<bool> use_normalized_central;


	// Flag: if set, the bayes will add every sample to dataset.
	Base::Property<bool> continuousCollection;

	// Flag: if set, the bayes recognition.
	Base::Property<bool> continuousRecognition;

	// Class of the incoming training example.
	Base::Property<short> trainingClass;

	// Name of file.
	Base::Property<std::string> filename;

	// Checks whether such a set of moments alread exists in dataset.
	bool isAlreadyPresent(const vector<Moments>& dataset_, const Moments &m_);

private:
	// Classifier.true
	CvNormalBayesClassifier bayes;

	// The vector of vectors of moments used for training
	vector<Moments> training_dataset;

	// The vector of vectors of moments used for training
	vector<short> training_responses;

	// Class names
	vector<std::string> classes;

	// Flag used for memorizing that user demanded to add the incoming moments to dataset.
	bool add;

	// Number of features used as inputs for Bayes classfier.
	short number_of_features;
};

} //: namespace CvBayesClassifier
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvBayesClassifier", Processors::CvBayesClassifier::CvBayesClassifier)

#endif /* CVBAYESCLASSIFIER_HPP_ */
