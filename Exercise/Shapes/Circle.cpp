#include "Circle.h"
#include <iostream>
using namespace std;

namespace Shapes
{

	Circle::Circle(const string &name, int radius) : Shape(name), m_radius(radius)
	{

	}


	Circle::~Circle()
	{
	}

	void Circle::Draw(ostream &ostr) const
	{
		ostr << "Circle" << endl;
		ostr << "------" << endl;
		Shape::Draw(ostr);
		ostr << endl;
		ostr << "Radius: " << m_radius << endl << endl;
	}
};

