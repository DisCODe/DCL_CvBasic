/*!
 * \file
 * \brief 
 * \author Tomek Kornuta,,,
 */

#ifndef CVBAYESCLASSIFIER_HPP_
#define CVBAYESCLASSIFIER_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
//#include "EventHandler2.hpp"

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

	/// Event handler.
	Base::EventHandler<CvBayesClassifier> h_onTraining;
	/// Train the classifier with the possessed dataset.
	void onTraining();


	// Handler activated when user will trigger "add to dataset"
	Base::EventHandler<CvBayesClassifier> h_onAddToDataset;
	// Sets the add_set flag.
	void onAddToDataset();
	
	// Handler activated when user will trigger "predict data"
	Base::EventHandler<CvBayesClassifier> h_onPredict;
	// Sets the predict flag.
	void onPredict();


	// Handler activated when user will trigger "clear whole dataset"
	Base::EventHandler<CvBayesClassifier> h_onClearDataset;
	// Adds received observation to dataset.
	void onClearDataset();

	
	// Handler activated when user will trigger "save params"
	Base::EventHandler<CvBayesClassifier> h_onSaveParams;
	// Save classifier parameters to file
	void onSaveParams();
	
	// Handler activated when user will trigger "load params"
	Base::EventHandler<CvBayesClassifier> h_onLoadParams;
	// Load classifier parameters from file
	void onLoadParams();

	/// Event handler.
	Base::EventHandler<CvBayesClassifier> h_onNewData;
	/// Event handler function.
	void onNewData();

	/// Input data stream
	Base::DataStreamIn<vector<Moments> > in_moments;

	// Class of the incomming training example.
	Base::Property<short> trainingClass;

private:
	// Classifier.
	CvNormalBayesClassifier bayes;

	// The vector of vectors of moments used for training
	vector<Moments> training_dataset;

	// The vector of vectors of moments used for training
	vector<short> training_responses;

	// Class names
	vector<std::string> classes;

	// Flag used for memorizing that user demanded to add the incomming moments to dataset.
	bool add;

	// Flag used for memorizing that user demanded to predict incomming moments.
	bool predict;
};

} //: namespace CvBayesClassifier
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvBayesClassifier", Processors::CvBayesClassifier::CvBayesClassifier)

#endif /* CVBAYESCLASSIFIER_HPP_ */
