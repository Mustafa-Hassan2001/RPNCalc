//----------------------------------------------------------------------------
// CalcDriver.cpp
//
// functions:  main(void)
//----------------------------------------------------------------------------
#include <iostream>
#include <cstdlib>
#include "RPNCalc.h"

using namespace std;
//----------------------------------------------------------------------------
//
//	Function:	main()
//
//	Title:		Driver for RPN Calculator
//
//	Description:	This file contains function main()
//				which creates and starts a calculator
//
//	Programmer:	Paul Bladek
//   
//	Date:		5/20/14
// 
//	Version:	1.0
//  
//	Environment:    Intel Xeon PC 
//                  	Software:   MS Windows 7 for execution; 
//                  	Compiles under Microsoft Visual C++.Net 2010
// 
//	Calls:		CRPNCalc constructor
// 
//	Returns:	EXIT_SUCCESS  = successful 
//
//	History Log:
//			4/205/14  PB  completed version 1.0
//----------------------------------------------------------------------------
int main(void)
{
	using PB_CALC::CRPNCalc;
	CRPNCalc myCalc;
	cout << endl << "Press \"enter\" to continue";
	cin.get();

	return EXIT_SUCCESS;
}
