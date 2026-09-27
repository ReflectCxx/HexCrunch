#pragma once

#include <set>
#include <deque>
#include <optional>

#include "Slider.h"

namespace hex
{
	class Command;

	class TouchConsumer
	{
		Slider m_slider;

		std::optional<std::reference_wrapper<Command>> m_resumeFxCmd = std::nullopt;

		void resumeFxQ();
		bool moveSliderUp();
		bool moveSliderDown();
		bool moveSlider(const Swipe);
		
	public:

		void init(HexTile*);
		void setPauseCmd(Command&);
		void onInputRecieved(const Swipe, const bool = false);
	};
}