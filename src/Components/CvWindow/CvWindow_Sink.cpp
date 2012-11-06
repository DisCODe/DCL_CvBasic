/*!
 * \file CvWindow_Sink.cpp
 * \brief
 * \author mstefanc
 * \date 2010-05-15
 */

#include <memory>
#include <string>
#include <iostream>
#include <stdexcept>

#include "CvWindow_Sink.hpp"
#include "Logger.hpp"
#include "Types/Drawable.hpp"

#include <boost/bind.hpp>

namespace Sinks {
namespace CvWindow {

CvWindow_Sink::CvWindow_Sink(const std::string & name) : Base::Component(name),
		title("title", boost::bind(&CvWindow_Sink::onTitleCahnged, this, _1, _2), name),
		count("count", 1)
{
	CLOG(LTRACE)<<"Hello CvWindow_Sink\n";

	registerProperty(title);

	count.setToolTip("Total number of displayed windows");
	registerProperty(count);

	firststep = true;
}

CvWindow_Sink::~CvWindow_Sink() {
	CLOG(LTRACE)<<"Good bye CvWindow_Sink\n";
}


void CvWindow_Sink::prepareInterface() {
	CLOG(LTRACE) << "CvWindow_Sink::configure\n";

	Base::EventHandler2 * hand;
	for (int i = 0; i < count; ++i) {
		char id = '0'+i;
		hand = new Base::EventHandler2;
		hand->setup(boost::bind(&CvWindow_Sink::onNewImageN, this, i));
		handlers.push_back(hand);
		registerHandler(std::string("onNewImage")+id, hand);

		Base::DataStreamIn<cv::Mat, Base::DataStreamBuffer::Newest, Base::Synchronization::Mutex> * stream = new Base::DataStreamIn<cv::Mat, Base::DataStreamBuffer::Newest, Base::Synchronization::Mutex>;
		in_img.push_back(stream);
		registerStream(std::string("in_img")+id, in_img[i]);
		addDependency(std::string("onNewImage")+id, stream);

		in_draw.push_back(new Base::DataStreamInPtr<Types::Drawable, Base::DataStreamBuffer::Newest, Base::Synchronization::Mutex>);
		registerStream(std::string("in_draw")+id, in_draw[i]);
	}

	// register aliases for first handler and streams
	registerHandler("onNewImage", handlers[0]);
	registerStream("in_img", in_img[0]);
	registerStream("in_draw", in_draw[0]);

	img.resize(count);
	to_draw.resize(count);

}

bool CvWindow_Sink::onInit() {
	CLOG(LTRACE) << "CvWindow_Sink::initialize\n";

	for (int i = 0; i < count; ++i) {
		char id = '0' + i;
		cv::namedWindow( std::string(title) + id);
	}
	return true;
}

bool CvWindow_Sink::onFinish() {
	CLOG(LTRACE) << "CvWindow_Sink::finish\n";

#if OpenCV_MAJOR<2 || OpenCV_MINOR<2
	for (int i = 0; i < count; ++i) {
		char id = '0' + i;
		cv::destroyWindow( std::string(title) + id);
	}
#endif

	return true;
}

bool CvWindow_Sink::onStep()
{
	CLOG(LTRACE)<<"CvWindow_Sink::step\n";

	try {
		for (int i = 0; i < count; ++i) {
			char id = '0' + i;

			if (img[i].empty()) {
				CLOG(LWARNING) << name() << ": image " << i << " empty";
			} else {
				// Refresh image.
				imshow( std::string(title) + id, img[i] );
				waitKey( 2 );
			}
		}

	}
	catch(...) {
		CLOG(LERROR) << "CvWindow::onStep failed\n";
	}

	return true;
}

bool CvWindow_Sink::onStop()
{
	CLOG(LTRACE) << name() << "::onStop";
	return true;
}

bool CvWindow_Sink::onStart()
{
	CLOG(LTRACE) << name() << "::onStart";
	return true;
}

void CvWindow_Sink::onNewImageN(int n) {
	CLOG(LTRACE) << name() << "::onNewImage(" << n << ")";

	try {
		if(!in_img[n]->empty()){
			img[n] = in_img[n]->read().clone();
		}

		if (!in_draw[n]->empty()) {
			to_draw[n] = in_draw[n]->read();
		}

		if (to_draw[n]) {
			to_draw[n]->draw(img[n], CV_RGB(255,0,255));
			to_draw[n] = boost::shared_ptr<Types::Drawable>();
		}

		// Display image.
		onStep();
	}
	catch(std::exception &ex) {
		CLOG(LERROR) << "CvWindow::onNewImage failed: " << ex.what() << "\n";
	}
}

void CvWindow_Sink::onTitleCahnged(const std::string & old_title, const std::string & new_title) {
	std::cout << "onTitleChanged: " << new_title << std::endl;

#if OpenCV_MAJOR<2 || OpenCV_MINOR<2
	std::cout << "Changing window title not supported\n";
#else
	for (int i = 0; i < count; ++i) {
		char id = '0' + i;
		try {
			cv::destroyWindow( std::string(old_title) + id );
		}
		catch(...) {}
	}
#endif
}


}//: namespace CvWindow
}//: namespace Sinks
