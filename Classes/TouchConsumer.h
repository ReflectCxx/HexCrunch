#pragma once

#include <set>
#include <deque>
#include "Slider.h"

namespace hex
{
	class TouchConsumer
	{
		Slider m_slider;

		bool moveSliderUp();
		bool moveSliderDown();
		bool moveSlider(const Swipe);
		
	public:

		TouchConsumer(const Slider&);

		void init(HexTile*);
		void onInputRecieved(const Swipe, const bool = false);
	};
}