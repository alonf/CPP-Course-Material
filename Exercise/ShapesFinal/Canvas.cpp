#include "Canvas.h"
#include <algorithm>
#include <memory>
#include <functional>

using namespace std;
using namespace placeholders;


namespace Shapes
{
	Canvas::Canvas() 
	{
	}


	Canvas::~Canvas()
	{
	}

	void Canvas::AddShape(shape_ptr shape)
	{
		m_shapes.push_back(shape);
	}

	void Canvas::RemoveShape(const shape_ptr & shape)
	{
		m_shapes.remove(shape);
	}


	void Canvas::Draw() const
	{
		for_each(cbegin(m_shapes), cend(m_shapes), bind(&Shape::Draw, _1, ref(cout)));
	}
}