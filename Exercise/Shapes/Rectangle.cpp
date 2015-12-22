#include "Rectangle.h"
#include <iostream>
using namespace std;

namespace Shapes
{

	Rectangle::Rectangle(const string &name, int width, int height) : Shape(name), m_width(width), m_height(height)
	{
	}


	Rectangle::~Rectangle()
	{
	}

	void Rectangle::Draw(ostream &ostr) const
	{
		ostr << "Rectangle" << endl;
		ostr << "---------" << endl;
		Shape::Draw(ostr);
		ostr << endl;
		ostr << "Width: " << m_width << endl;
		ostr << "Height: " << m_height << endl << endl;
	}
}
