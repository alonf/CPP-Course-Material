// Logic.cpp : Defines the entry point for the console application.
//

#include "Gate.h"
#include <iostream>

using namespace std;

void main()
{
	On on;
	Off off;
	auto or ([](const IGate &a, const IGate &b) -> bool { return a.Output() || b.Output();});
	auto and ([](const IGate &a, const IGate &b) -> bool { return a.Output() && b.Output();});
	auto xor ([](const IGate &a, const IGate &b) -> bool { return a.Output() ^ b.Output();});
	auto not ([](const IGate &a) -> bool { return !a.Output(); });
	
	one_input_gate_t n1(not, off);
	two_inputs_gate_t or1(or , n1, off);
	cout << "or1: " << or1.Output() << endl;

	Switch a, b, cIn;
	two_inputs_gate_t x1(xor, a, b);
	two_inputs_gate_t x2(xor, x1, cIn);
	two_inputs_gate_t a1(and, x1, cIn);
	two_inputs_gate_t a2(and, a, b);
	two_inputs_gate_t o1(or, a1, a2);
	
	for (int i = 0; i < 8; ++i)
	{
		cout << '[' << a.Output() << ' ' << b.Output() << ' ' << cIn.Output() << "] ==> [" << x2.Output() << ' ' << o1.Output() << ']' << endl;
		
		cIn.Toggle();
		if (i % 2 == 1)
			b.Toggle();
		if (i == 3)
			a.Toggle();
	}
}

