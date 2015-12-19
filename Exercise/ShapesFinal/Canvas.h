#pragma once

#include <list>
#include "Shape.h"


namespace Shapes
{
	class Canvas
	{
	private:
		std::list<shape_ptr> m_shapes;
	public:
		Canvas();
		~Canvas();
		void AddShape(shape_ptr shape);
		void RemoveShape(const shape_ptr &shape);
		void Draw() const;
	};
}