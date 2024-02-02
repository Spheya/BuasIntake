#include "Sprite.hpp"

namespace tmpl8 {

	Sprite::Sprite(std::shared_ptr<Surface> a_Surface, unsigned int a_NumFrames, unsigned int a_NumRows) :
		m_Width(static_cast<int>(a_Surface->GetWidth() / (a_NumFrames / a_NumRows))),
		m_Height(static_cast<int>(a_Surface->GetHeight() / a_NumRows)),
		m_Pitch(a_Surface->GetWidth()),
		m_NumFrames(a_NumFrames),
		m_NumRows(a_NumRows),
		m_CurrentFrame(0),
		m_Surface(a_Surface)
	{}

	void Sprite::Draw(Surface* target, vec2 position, vec2 size) {
		int pos1X = int(position.x) - int(size.x * m_Origin.x);
		int pos1Y = int(position.y) - int(size.y * m_Origin.y);
		int pos2X = int(position.x) + int(size.x * (1.0f - m_Origin.x));
		int pos2Y = int(position.y) + int(size.y * (1.0f - m_Origin.y));

		vec2 uvScale = vec2(float(m_NumRows) / float(m_NumFrames), 1.0f / float(m_NumRows));
		vec2 uvOffset = uvScale * vec2(float(m_CurrentFrame % (m_NumFrames / m_NumRows)), float((m_NumRows * m_CurrentFrame) / m_NumFrames));

		for (int x = std::min(pos1X, pos2X); x < std::max(pos1X, pos2X); x++) {
			for (int y = std::min(pos1Y, pos2Y); y < std::max(pos1Y, pos2Y); y++) {
				vec2 uv(float(x - pos1X) / float(pos2X - pos1X), float(y - pos1Y) / float(pos2Y - pos1Y));
				Pixel color = m_Surface->UvLookup(uv * uvScale + uvOffset);
				if (color & 0xff000000) {
					target->WritePixel(x, y, color);
				}
			}
		}
	}

}