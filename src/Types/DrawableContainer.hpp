/*!
 * \file DrawableContainer.hpp
 * \brief File containing DrawableContainer type
 *
 * \author mstefanc
 * \date 20.08.2010
 */

#ifndef DRAWABLECONTAINER_HPP_
#define DRAWABLECONTAINER_HPP_

#include "Drawable.hpp"

#include <opencv2/opencv.hpp>
#include <vector>

#include <boost/foreach.hpp>

namespace Types {

class DrawableContainer : public Drawable {
public:
	~DrawableContainer() {}

	virtual void draw(cv::Mat & image, cv::Scalar color, int offsetX = 0, int offsetY = 0) {
		BOOST_FOREACH(Drawable * item, items) {
			item->draw(image, color, offsetX, offsetY);
		}
	}

	void add(Drawable * it) {
		items.push_back(it);
	}

	Drawable * get(size_t id) {
		return items[id];
	}

	size_t size() {
		return items.size();
	}

	void clear() {
		items.clear();
	}

	virtual Drawable * clone() {
		DrawableContainer * ret = new DrawableContainer;
		BOOST_FOREACH(Drawable * item, items) {
			ret->add(item->clone());
		}

		return ret;
	}

private:
	std::vector<Drawable*> items;
};

} //: namespace Types

#endif /* DRAWABLECONTAINER_HPP_ */
