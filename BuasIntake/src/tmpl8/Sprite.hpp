#pragma once

#include <memory>

#include "Surface.hpp"
#include "Template.hpp"

namespace tmpl8 {
	class Sprite {
	public:
		// Structors
		Sprite(std::shared_ptr<Surface> a_Surface, unsigned int a_NumFrames, unsigned int a_NumRows);
		// Methods
		void Draw(Surface* target, vec2 position, vec2 size);
		void SetFrame(unsigned int a_Index) { m_CurrentFrame = a_Index; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		void SetOrigin(vec2 origin) { m_Origin = origin; }
		vec2 GetOrigin() const { return m_Origin; }
		const Pixel* GetBuffer() const { return m_Surface->GetBuffer(); }
		unsigned int Frames() const { return m_NumFrames; }
		const std::shared_ptr<Surface> GetSurface() const { return m_Surface; }
	private:
		// Attributes
		int m_Width, m_Height, m_Pitch;
		unsigned int m_NumFrames;
		unsigned int m_NumRows;
		unsigned int m_CurrentFrame;
		vec2 m_Origin = vec2(0.5f);
		std::shared_ptr<Surface> m_Surface;
	};
}