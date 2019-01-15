#pragma once

class cLayer16x16
{
	unsigned m_VerticalSpacing;
	unsigned m_HorizontalSpacing;
	unsigned m_LayerWidth;
	unsigned m_LayerHeight;
	char     m_LayerInfo[16][16];

public:
	void Create(unsigned verticalSpacing, unsigned horizontalSpacing);

	cLayer16x16();
	~cLayer16x16();
};

