#include "Sprite.hpp"

namespace tmpl8 {

	Sprite::Sprite(Surface* a_Surface, unsigned int a_NumFrames, unsigned int a_NumRows) :
		m_Width(static_cast<int>(a_Surface->GetWidth() / (a_NumFrames / a_NumRows))),
		m_Height(static_cast<int>(a_Surface->GetHeight() / a_NumRows)),
		m_Pitch(a_Surface->GetWidth()),
		m_NumFrames(a_NumFrames),
		m_NumRows(a_NumRows),
		m_CurrentFrame(0),
		m_Start(new unsigned int* [a_NumFrames]),
		m_Surface(a_Surface) {
		InitializeStartData();
	}

	Sprite::~Sprite() {
		delete m_Surface;
		for (unsigned int i = 0; i < m_NumFrames; i++) delete m_Start[i];
		delete[] m_Start;
	}

	void Sprite::Draw(Surface* target, vec2 position, vec2 size) {
		int posX = int(position.x);
		int posY = int(position.y);
		int width = int(size.x);
		int height = int(size.y);

		vec2 uvScale = vec2(float(m_NumRows) / float(m_NumFrames), 1.0f / float(m_NumRows));
		vec2 uvOffset = uvScale * vec2(m_CurrentFrame % (m_NumFrames / m_NumRows), (m_NumRows * m_CurrentFrame) / m_NumFrames);

		for (int x = 0; x < width; x++) {
			for (int y = 0; y <height; y++) {
				vec2 uv(float(x) / float(width), float(y) / float(height));
				target->WritePixel(x + posX, y + posY, m_Surface->UvLookup(uv * uvScale + uvOffset));
			}
		}
	}

	void Sprite::InitializeStartData() {
		for (unsigned int f = 0; f < m_NumFrames; ++f) {
			m_Start[f] = new unsigned int[m_Height];
			for (int y = 0; y < m_Height; ++y) {
				m_Start[f][y] = m_Width;
				const Pixel* addr = GetBuffer() + f * m_Width + y * m_Pitch;
				for (int x = 0; x < m_Width; ++x) {
					if (addr[x]) {
						m_Start[f][y] = x;
						break;
					}
				}
			}
		}
	}

}