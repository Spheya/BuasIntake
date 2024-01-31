#pragma once

#include "Surface.hpp"
#include "Template.hpp"

namespace tmpl8 {
	class Sprite {
	public:
		// Structors
		Sprite(Surface* a_Surface, unsigned int a_NumFrames, unsigned int a_NumRows);
		~Sprite();
		// Methods
		void Draw(Surface* target, vec2 position, vec2 size);
		void SetFrame(unsigned int a_Index) { m_CurrentFrame = a_Index; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		const Pixel* GetBuffer() const { return m_Surface->GetBuffer(); }
		unsigned int Frames() const { return m_NumFrames; }
		const Surface* GetSurface() const { return m_Surface; }
	private:
		// Methods
		void InitializeStartData();
		// Attributes
		int m_Width, m_Height, m_Pitch;
		unsigned int m_NumFrames;
		unsigned int m_NumRows;
		unsigned int m_CurrentFrame;
		unsigned int** m_Start;
		Surface* m_Surface;
	};
}