/*!
 * \file
 * \brief 
 * \author Jan Figat
 */

#ifndef VIEWSEQUENCE_HPP_
#define VIEWSEQUENCE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"



namespace Processors {
namespace ViewSequence {

/*!
 * \class ViewSequence
 * \brief ViewSequence processor class.
 *
 * ViewSequence processor.
 */
class ViewSequence: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	ViewSequence(const std::string & name = "ViewSequence");

	/*!
	 * Destructor
	 */
	virtual ~ViewSequence();

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


	

};

} //: namespace ViewSequence
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("ViewSequence", Processors::ViewSequence::ViewSequence)

#endif /* VIEWSEQUENCE_HPP_ */
