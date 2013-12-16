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
#include <ctime>

#include "CvWindow_Sink.hpp"
#include "Logger.hpp"
#include "Types/Drawable.hpp"
#include "Types/DrawableContainer.hpp"

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

namespace Sinks {
namespace CvWindow {

CvWindow_Sink::CvWindow_Sink(const std::string & name) :
	Base::Component(name), title("title", boost::bind(
			&CvWindow_Sink::onTitleChanged, this, _1, _2), name), dir(
			"save.directory", boost::bind(&CvWindow_Sink::onDirChanged, this, _1, _2), "./"),
			filename("save.filename", boost::bind(&CvWindow_Sink::onFilenameChanged, this, _1, _2), name),
			count("count", 1)
{
	CLOG(LTRACE) << "Hello CvWindow_Sink\n";

	registerProperty(title);

	count.setToolTip("Total number of displayed windows");
	registerProperty(count);
	registerProperty( filename);
	registerProperty( dir);

	firststep = true;
}

CvWindow_Sink::~CvWindow_Sink() {
	CLOG(LTRACE) << "Good bye CvWindow_Sink\n";
}

void CvWindow_Sink::prepareInterface() {
	CLOG(LTRACE) << "CvWindow_Sink::configure\n";

	h_onRefresh.setup(this, &CvWindow_Sink::onRefresh);
	registerHandler("onRefresh", &h_onRefresh);

	addDependency("onRefresh", NULL);

	Base::EventHandler2 * hand;
	for (int i = 0; i < count; ++i) {
		char id = '0' + i;
		hand = new Base::EventHandler2;
		hand->setup(boost::bind(&CvWindow_Sink::onNewImageN, this, i));
		handlers.push_back(hand);
		registerHandler(std::string("onNewImage") + id, hand);

		Base::DataStreamIn<cv::Mat, Base::DataStreamBuffer::Newest,
				Base::Synchronization::Mutex> * stream =
				new Base::DataStreamIn<cv::Mat, Base::DataStreamBuffer::Newest,
						Base::Synchronization::Mutex>;
		in_img.push_back(stream);
		registerStream(std::string("in_img") + id,
				(Base::DataStreamInterface*) (in_img[i]));
		addDependency(std::string("onNewImage") + id, stream);

		in_draw.push_back(new Base::DataStreamInPtr<Types::Drawable>);
		registerStream(std::string("in_draw") + id, in_draw[i]);

		out_point.push_back(new Base::DataStreamOut<cv::Point>);
		registerStream(std::string("out_point") + id, out_point[i]);

		// save handlers
		hand = new Base::EventHandler2;
		hand->setup(boost::bind(&CvWindow_Sink::onSaveImageN, this, i));
		handlers.push_back(hand);
		registerHandler(std::string("onSaveImage") + id, hand);
	}

	h_onSaveAllImages.setup(this, &CvWindow_Sink::onSaveAllImages);
	registerHandler("onSaveAllImages", &h_onSaveAllImages);

	// register aliases for first handler and streams
	registerHandler("onNewImage", handlers[0]);
	registerStream("in_img", in_img[0]);
	registerStream("in_draw", in_draw[0]);

	img.resize(count);
	to_draw.resize(count);

	// Split window titles.
	std::string t = title;
	boost::split(titles, t, boost::is_any_of(","));
	if ((titles.size() == 1) && (count > 1))
		titles.clear();
	for (int i = titles.size(); i < count; ++i) {
		char id = '0' + i;
		titles.push_back(std::string(title) + id);
	}
}

bool CvWindow_Sink::onInit() {
	CLOG(LTRACE) << "CvWindow_Sink::initialize\n";

	for (int i = 0; i < count; ++i) {
		cv::namedWindow(titles[i]);
	}
	return true;
}

bool CvWindow_Sink::onFinish() {
	CLOG(LTRACE) << "CvWindow_Sink::finish\n";

#if OpenCV_MAJOR<2 || OpenCV_MINOR<2
	for (int i = 0; i < count; ++i) {
		char id = '0' + i;
		cv::destroyWindow(titles[i]);
	}
#endif

	return true;
}

bool CvWindow_Sink::onStep() {
	return true;
}

bool CvWindow_Sink::onStop() {
	CLOG(LTRACE) << name() << "::onStop";
	return true;
}

bool CvWindow_Sink::onStart() {
	CLOG(LTRACE) << name() << "::onStart";
	return true;
}

void CvWindow_Sink::onNewImageN(int n) {
	CLOG(LTRACE) << name() << "::onNewImage(" << n << ")";

	try {
		if (!in_img[n]->empty()) {
			img[n] = in_img[n]->read().clone();
		}

		Types::DrawableContainer ctr;
		while (!in_draw[n]->empty()) {
			ctr.add(in_draw[n]->read()->clone());
			to_draw[n] = boost::shared_ptr<Types::Drawable>(ctr.clone());
		}

		if (to_draw[n]) {
			to_draw[n]->draw(img[n], CV_RGB(255,0,255));
			// TODO: dodać wygaszanie starszych drawable, np. przez 10 odświeżeń
			//to_draw[n] = boost::shared_ptr<Types::Drawable>();
		}

		// Display image.
		//onStep();
	} catch (std::exception &ex) {
		CLOG(LERROR) << "CvWindow::onNewImage failed: " << ex.what() << "\n";
	}
}

void CvWindow_Sink::onRefresh() {
	CLOG(LTRACE) << "CvWindow_Sink::step\n";

	try {
		for (int i = 0; i < count; ++i) {
			char id = '0' + i;

			if (img[i].empty()) {
				CLOG(LWARNING) << name() << ": image " << i << " empty";
			} else {
				// Refresh image.
				imshow(titles[i], img[i]);
				waitKey(2);
			}
		}

	} catch (...) {
		CLOG(LERROR) << "CvWindow::onStep failed\n";
	}
}

void CvWindow_Sink::onTitleChanged(const std::string & old_title,
		const std::string & new_title) {
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

void CvWindow_Sink::onSaveImageN(int n) {
	CLOG(LTRACE) << name() << "::onSaveImageN(" << n << ")";

	try {
		// Save image.
		std::string tmp_name = std::string(dir) + std::string("/") + std::string(filename) + std::string(".png");
		imwrite(tmp_name, img[n]);
		CLOG(LINFO) << "Window " << name() << " saved to file " << tmp_name <<std::endl;

	} catch (std::exception &ex) {
		CLOG(LERROR) << "CvWindow::onSaveImageN failed: " << ex.what() << "\n";
	}
}

void CvWindow_Sink::onSaveAllImages() {
	CLOG(LTRACE) << name() << "::onSaveAllImages";

	std::time_t rawtime;
	std::tm* timeinfo;
	char buffer [80];

	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	std::strftime(buffer,80,"%Y-%m-%d-%H-%M-%S",timeinfo);

	try {
		for (int i = 0; i < count; ++i) {
			char id = '0' + i;

			if (img[i].empty()) {
				LOG(LWARNING) << name() << ": image " << i << " empty";
			} else {
				// Save image.
				std::string tmp_name = std::string(dir) + std::string("/") + std::string(filename) + id + "_" + buffer + std::string(".png");
				imwrite(tmp_name, img[i]);
				CLOG(LINFO) << "Window " << name() << " saved to file " << tmp_name <<std::endl;
			}
		}
	} catch (std::exception &ex) {
		CLOG(LERROR) << "CvWindow::onSaveAllImages failed: " << ex.what() << "\n";
	}
}

void CvWindow_Sink::onFilenameChanged(const std::string & old_filename,
		const std::string & new_filename) {
	filename = new_filename;
	CLOG(LTRACE) << "onFilenameChanged: " << std::string(filename) << std::endl;
}

void CvWindow_Sink::onDirChanged(const std::string & old_dir,
		const std::string & new_dir) {
	dir = new_dir;
	CLOG(LTRACE) << "onDirChanged: " << std::string(dir) << std::endl;
}

}//: namespace CvWindow
}//: namespace Sinks
