// ShapesFinal.cpp : Defines the entry point for the console application.
//


#include <memory>

#include "Canvas.h"
#include "Rectangle.h"
#include "Circle.h"

using namespace std;
using namespace Shapes;

void main()
{
	shape_ptr r1 = make_shared<Rectangle>("R1", 20, 40);
	shape_ptr c1 = make_shared<Circle>("C1", 20);

	Canvas canvas;

	canvas.AddShape(r1);
	canvas.AddShape(c1);
	canvas.Draw();
	canvas.AddShape(make_shared<Rectangle>("R2", 200, 400));
	canvas.AddShape(make_shared<Circle>("C2", 200));
	canvas.Draw();
	canvas.RemoveShape(c1);
	canvas.RemoveShape(r1);
	canvas.Draw();

	r1.reset();
	c1.reset();

	canvas.Draw();
}

