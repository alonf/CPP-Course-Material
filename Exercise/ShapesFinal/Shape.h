#pragma once
#include <string>
#include <memory>
#include <iostream>

namespace Shapes
{
	class Shape
	{
	private:
		std::string m_name;
		static int m_totalShapes;

	public:
		Shape(const std::string &name);
		virtual ~Shape();
		virtual void Draw(std::ostream &ostr) const;
	};

	typedef std::shared_ptr<Shape> shape_ptr;
}