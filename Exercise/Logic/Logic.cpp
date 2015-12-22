// Logic.cpp : Defines the entry point for the console application.
//

#include "Gate.h"
#include <iostream>

using namespace std;

int main()
{
	On on;
	Off off;
	auto or_func  {[](const IGate &a, const IGate &b) -> bool { return a.Output() || b.Output();}};
	auto and_func {[](const IGate &a, const IGate &b) -> bool { return a.Output() && b.Output();}};
	auto xor_func {[](const IGate &a, const IGate &b) -> bool { return a.Output() ^ b.Output();}};
	auto not_func {[](const IGate &a) -> bool { return !a.Output(); }};
	
	one_input_gate_t n1(not_func, off);
	two_inputs_gate_t or1(or_func , n1, off);
	cout << "or1: " << or1.Output() << endl;

	Switch a, b, cIn;
	two_inputs_gate_t x1(xor_func, a, b);
	two_inputs_gate_t x2(xor_func, x1, cIn);
	two_inputs_gate_t a1(and_func, x1, cIn);
	two_inputs_gate_t a2(and_func, a, b);
	two_inputs_gate_t o1(or_func, a1, a2);
	
	for (int i = 0; i < 8; ++i)
	{
		cout << '[' << a.Output() << ' ' << b.Output() << ' ' << cIn.Output() << "] ==> [" << x2.Output() << ' ' << o1.Output() << ']' << endl;
		
		cIn.Toggle();
		if (i % 2 == 1)
			b.Toggle();
		if (i == 3)
			a.Toggle();
	}
	return 0;
}

