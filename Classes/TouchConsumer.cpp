
#include "HexGrid.h"
#include "HexTile.hpp"
#include "Game.h"
#include "TouchConsumer.h"

USING_NS_CC;

namespace {
	constexpr auto ENABLE_SAME_COLOR_SWAP = false;
}


namespace hex
{
	void TouchConsumer::init(HexTile* pActor) {
		m_slider.init(pActor);
	}


	bool TouchConsumer::moveSlider(const Swipe pDir)
	{
		switch (pDir) {
		case Swipe::Up: return moveSliderUp();
		case Swipe::Down: return moveSliderDown();
		case Swipe::Left: return m_slider.moveActorLeft();
		case Swipe::Right: return m_slider.moveActorRight();
		default:return false;
		}
	}


	bool TouchConsumer::moveSliderUp()
	{
		bool movedSuccessfully = false;
		m_slider.highlightSelection(Turn::Off);
		if (m_slider.actor().getRingIndex() != 0)
		{
			m_slider.moveActorUp();
			m_slider.updateOuterRingPathQ();
			movedSuccessfully = true;
		}
		m_slider.highlightSelection(Turn::On);
		return movedSuccessfully;
	}


	bool TouchConsumer::moveSliderDown()
	{
		bool movedSuccessfully = false;
		m_slider.highlightSelection(Turn::Off);
		if (m_slider.actor().getRingIndex() < (RING_COUNT - 2))
		{
			m_slider.moveActorDown();
			m_slider.updateOuterRingPathQ();
			movedSuccessfully = true;
		}
		m_slider.highlightSelection(Turn::On);
		return movedSuccessfully;
	}


	void TouchConsumer::onInputRecieved(const Swipe pDir, const bool pIsMockInput)
	{
		if (!Game::instance().acceptInput() || !m_slider.isActive()) 
		{
			CCLOG("Grid not idle! %lu cmds running.", 
				  Game::instance().fxController().getRunningCmdCount());
			return;
		}

		if (pDir == Swipe::SingleTap) {
			m_slider.swapSelection(*this);
			return;
		}

		m_slider.trackSwipe(pDir);
		m_slider.highlightBlockedTiles(Turn::Off);
		
		if constexpr (ENABLE_SAME_COLOR_SWAP) {
			moveSlider(pDir);
		}
		else {
			auto lastActor = &m_slider.actor();
			auto lastFollowerI = m_slider.followerIndex();
			bool movedSuccessfully = false;

			do {
				movedSuccessfully = moveSlider(pDir);
				if (m_slider.actor().getColorId() == m_slider.follower().getColorId()) {
					m_slider.blockedTiles().insert(&m_slider.actor());
					m_slider.blockedTiles().insert(&m_slider.follower());
				}
			} while (movedSuccessfully && m_slider.actor().getColorId() == m_slider.follower().getColorId());

			if (!movedSuccessfully) {
				m_slider.resetToPosition(lastActor, lastFollowerI);
			}
			m_slider.highlightBlockedTiles(Turn::On);
		}

		if (!pIsMockInput) {
			m_slider.alignWithGrid();
		}
	}
}