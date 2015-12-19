#pragma once
#include <string>

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

	typedef Shape *shape_ptr;
}