#include "Canvas.h"
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
		
	}
}