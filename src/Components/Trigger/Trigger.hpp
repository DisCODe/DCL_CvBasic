/*!
 * \file
 * \brief 
 * \author Micha Laszkowski
 */

#ifndef TRIGGER_HPP_
#define TRIGGER_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"



namespace Processors {
namespace Trigger {

/*!
 * \class Trigger
 * \brief Trigger processor class.
 *
 * Trigger processor.
 */
class Trigger: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	Trigger(const std::string & name = "Trigger");

	/*!
	 * Destructor
	 */
	virtual ~Trigger();

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

	// Output data streams
	Base::DataStreamOut<Base::UnitType> out_trigger;

	/// Inputy event - stop auto triggering.
	Base::DataStreamIn<Base::UnitType, Base::DataStreamBuffer::Newest> in_stop_auto_trigger;

	/// Auto-trigger - sends a trigger in every step.
	Base::Property<bool> prop_auto_trigger;

	/// Flag indicating whether the trigger button was pressed.
	bool triggered_flag;

	/// Activates trigger when button is pressed.
	void onTriggerButtonPressed();

	/// Sends trigger when component is activated if auto-trigger property is set to true.
	void onAutoTrigger();

	/// Stops auto trigger (if data in in_stop_auto_trigger appears).
	void onStopAutoTrigger();

};

} //: namespace Trigger
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("Trigger", Processors::Trigger::Trigger)

#endif /* TRIGGER_HPP_ */
