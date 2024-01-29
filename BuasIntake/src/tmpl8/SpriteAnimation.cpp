#include "SpriteAnimation.hpp"

#include <cmath>

namespace Tmpl8 {

	SpriteAnimation::SpriteAnimation(std::initializer_list<unsigned int> frames) :
		m_frames(std::move(frames)),
		m_time(0.0f)
	{}

	void SpriteAnimation::update(float deltatime, float fps) {
		m_time += deltatime * fps;
		m_time = std::fmod(m_time, float(m_frames.size()));
	}

	void SpriteAnimation::restart() {
		m_time = 0.0f;
	}

	unsigned int SpriteAnimation::getCurrentFrame() const {
		return m_frames[unsigned int(m_time)];
	}

}
