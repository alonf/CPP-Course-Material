#pragma once
#include "Shape.h"

namespace Shapes
{
	class Rectangle : public Shape
	{
	private:
		int m_width;
		int m_height;
	public:
		Rectangle(const std::string & name, int width, int height);
		~Rectangle();
		virtual void Draw(std::ostream & ostr) const;
	};
}