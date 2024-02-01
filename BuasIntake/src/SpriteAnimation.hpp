#pragma once

#include <initializer_list>
#include <vector>

class SpriteAnimation {
public:
	explicit SpriteAnimation(std::initializer_list<unsigned int> frames);

	void update(float deltatime, float fps = 12.0f);
	void restart();

	unsigned int getCurrentFrame() const;

private:
	std::vector<unsigned int> m_frames;
	float m_time;
};

