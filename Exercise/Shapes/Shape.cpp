#include "Shape.h"
#include <ostream>

namespace Shapes
{
	int Shape::m_totalShapes = 0;

	Shape::Shape(const std::string & name) : m_name(name)
	{
		m_totalShapes++;
	}

	Shape::~Shape()
	{
		m_totalShapes--;
	}

	void Shape::Draw(std::ostream &ostr) const
	{
		ostr << "Shape: " << m_name << std::endl << "Totsl Shapes " << m_totalShapes;
	}
}
