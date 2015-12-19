// MetaprogrammingExample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

#ifndef LAST 
#define LAST 97 
#endif
 
template <int p, int i>
class is_prime 
{
public:
		enum 
		{ 
			prime = (p==2) || (p % i) && is_prime<( i > 2 ? p : 0), i-1>::prime
		}; 
}; 
 
template<>
class is_prime<0,0> {
public:
		enum 
		{
			prime = 1
		};
}; 
 
template<>
class is_prime<0,1> 
{
public:
		enum 
		{
			prime = 1
		};
}; 
 
template <int i>
class print_prime_number
{      // primary template for loop to print prime numbers
public:
		print_prime_number<i-1> print_previous_prime_number; 
		
		enum 
		{ 
			prime = is_prime<i, i-1>::prime
		}; 

		void print_only_prime()
		{
				print_previous_prime_number.print_only_prime();
				if (prime)
				{
						cout << i << ", ";
				}
		} 
}; 
 
template<>
class print_prime_number<1>     // full specialization to end the loop
{
public:
	enum 
	{
		prime = 0
	}; 
		
	void print_only_prime()
	{
		cout << "Prime Numbers: ";
	}
}; 
 
template<>
class print_prime_number<LAST>     // full specialization to end the loop
{
public:
	print_prime_number<LAST - 1> print_previous_prime_number; 

	enum 
	{
		prime = is_prime<LAST, LAST - 1>::prime
	}; 
		
	void print_only_prime()
	{
		print_previous_prime_number.print_only_prime();

		if (prime)
		{
			cout << LAST;
		}
		cout << endl;
	} 

	
};

int main()
{
   print_prime_number<LAST> prime_printer; 
   prime_printer.print_only_prime(); 

   return 0;
}

