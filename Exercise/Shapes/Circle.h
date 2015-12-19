#pragma once
#include "Shape.h"

namespace Shapes
{
	class Circle : public Shape
	{
	private: 
		int m_radius;

	public:
		Circle(const std::string &name, int radius);
		~Circle();
		virtual void Draw(std::ostream & ostr) const;
	};
}