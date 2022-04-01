#include "RPNCalc.h"
//-------------------------------------------------------------------------------------------
//    Class:		CRPNCalc
//
//    File:             RPNCalc.cpp
//
//    Description:	This file contains the function definitions for CRPNCalc
//
//    Programmer:		
//   
//    Date:             
// 
//    Version:          1.0
//  
//    Environment:	Intel Xeon PC 
//                  	Software:   MS Windows 7 for execution; 
//                  	Compiles under Microsoft Visual C++.Net 2010
// 
//	  class CRPNCalc:
//
//	  Properties:
//				double m_registers[10];
//				stack<string> m_stack;
//				vector<list<string>> m_programs(NUMPROGRAMS);
//				istringstream m_inStrStream;
//	
//
//	  Non-inline Methods:
//	
//				CRPNCalc(bool on)
//				run()
//				print(ostream& ostr)
//				parse()
//				void add() -- 
//				void bin_prep(double& d1, double& d2) -- 
//				void clear() -- 
//				void clearAll() -- 
//				void divide() -- 
//				void exp() -- 
//				void getReg(int reg) -- 
//				void loadProgram() -- 
//				void mod() -- 
//				void multiply() -- 
//				void neg() -- 
//				void parse() -- 
//				void recordProgram() -- 
//				void rotateUp() -- 
//				void rotateDown() -- 
//				void runProgram() -- 
//				void saveToFile() -- 
//				void setReg(int reg) -- 
//				void subtract() -- 
//				void unary_prep(double& d) -- 
	
//	  related functions:
//				ostream &operator <<(ostream &ostr, const CRPNCalc &calc)
//    				istream &operator >>(istream &istr, CRPNCalc &calc)
//
//    History Log:
//                           
// ----------------------------------------------------------------------------	
namespace PB_CALC
{
// ----------------------------------------------------------------------------
//	constructor
// ----------------------------------------------------------------------------
	CRPNCalc::CRPNCalc(bool on): m_on(on), m_error(false), m_helpOn(true),
		m_programRunning(false)
	{
		for(int i = 0; i < NUMREGS; i++)
			m_registers[i] = 0.0;
		if(m_on)
			run();
	}

// ----------------------------------------------------------------------------
//	starts the calculator running
// ----------------------------------------------------------------------------
	void CRPNCalc::run()
	{		
		while (m_on == true)
		{
			system("clear");
			print(cout);
			input(cin);
			
		}
	} 

// ----------------------------------------------------------------------------
//	prints out calculator screen
// ----------------------------------------------------------------------------
	void CRPNCalc::print(ostream& ostr)
	{
		double d = 0.0;
		ostr << "[RPN Programmable Calculator] by <Your Group Name>" << endl;
		if(m_helpOn)
			cout << helpMenu;
		else
			cout << endl << endl << endl;
		cout << line;
		if(!m_stack.empty())
		{
			d = m_stack.front();
			ostr << d;
		}
		ostr << endl << endl;
		if(m_error)
		{
			ostr << "<<error>>" << endl;
			m_error = false;
		}
	} 

// ----------------------------------------------------------------------------
//	parses the next command from m_instrStream
// ----------------------------------------------------------------------------

	bool checkDigitalLetter(char c)
	{
		if (c < '0' || c > '9')
			return false;
		return true;
	}
	bool checkPoint(char c)
	{
		if (c != '.')
			return false;
		return true;
	}
	bool isDigital(string str)
	{
		for (size_t i = 0; i < str.length(); ++i)
		{
			if (checkDigitalLetter(str[i]) == false && checkPoint(str[i]) == false)
				return false;
		}
		return true;
	}
	void CRPNCalc::parse()
	{		
		double number = 0;
		while (m_buffer.length() != 0)
		{
			bool delDecimal = false;
			while (m_buffer[0] == ' ')
				m_buffer.erase(m_buffer.begin());
			number = atof(m_buffer.c_str());
			// if this is number
			if ((number != 0 && m_buffer[0] != '+') || m_buffer[0] == '0')
			{
				if (m_buffer[0] == '-')
					m_buffer.erase(m_buffer.begin());
				while ((m_buffer[0] >= '0' && m_buffer[0] <= '9')
					|| m_buffer[0] == '.')
				{
					if (m_buffer[0] == '.')
						if (delDecimal == false)
							delDecimal = true;
						else
							break;
					m_buffer.erase(m_buffer.begin());
					if (m_buffer.length() == 0)
						break;
				}
				m_stack.push_front(number);
			}
			else
			{
				string token;
				if (m_buffer.length() >= 2)
				{
					// Check CE
					if (toupper(m_buffer[0]) == 'C'
						&& toupper(m_buffer[1]) == 'E')
					{
						m_buffer.erase(m_buffer.begin(), m_buffer.begin() + 2);
						clearAll();
						continue;
					}					
					else if (m_buffer[0] == '-' && m_buffer[1] == '0')
					{
						m_buffer.erase(m_buffer.begin());
						while (m_buffer[0] == '0')
						{
							m_buffer.erase(m_buffer.begin());
							if (m_buffer.length() == 0)
								break;
						}
						m_stack.push_front(number);
						neg();
						continue;
					}
					else if (toupper(m_buffer[0]) == 'S' &&
						m_buffer[1] >= '0' && m_buffer[1] <= '9')
					{
						m_buffer.erase(m_buffer.begin()); 
						char index = m_buffer[0];
						setReg(static_cast<int>(index) - '0');
						m_buffer.erase(m_buffer.begin()); 
						continue;
					}					
					else if (toupper(m_buffer[0]) == 'G' &&
						m_buffer[1] >= '0' && m_buffer[1] <= '9')
					{
						m_buffer.erase(m_buffer.begin()); 
						char index = m_buffer[0];
						getReg(static_cast<int>(index) - '0');
						m_buffer.erase(m_buffer.begin());
						continue;
					}
				}
				if (m_buffer.length() != 0)
				{
					token = m_buffer[0];
					if (0 == token.compare("+"))
						add();
					else if (0 == token.compare("-"))
						subtract();
					else if (0 == token.compare("*"))
						multiply();
					else if (0 == token.compare("/"))
						divide();
					else if (0 == token.compare("^"))
						exp();
					else if (0 == token.compare("%"))
						mod();
					else if (0 == token.compare("c") || 0 == token.compare("C"))
						clearEntry();
					else if (0 == token.compare("d") || 0 == token.compare("D"))
						rotateDown();
					else if (0 == token.compare("f") || 0 == token.compare("F"))
						saveToFile();
					else if (0 == token.compare("h") || 0 == token.compare("H"))
						m_helpOn = !m_helpOn;
					else if (0 == token.compare("l") || 0 == token.compare("L"))
						loadProgram();
					else if (0 == token.compare("m") || 0 == token.compare("M"))
						neg();
					else if (0 == token.compare("p") || 0 == token.compare("P"))
						recordProgram();
					else if (0 == token.compare("r") || 0 == token.compare("R"))
					{
						runProgram();
						return;
					}
					else if (0 == token.compare("u") || 0 == token.compare("U"))
						rotateUp();
					else if (0 == token.compare("x") || 0 == token.compare("X"))
						m_on = false;
					else
						m_error = true;
					m_buffer.erase(m_buffer.begin());
				}
			}
		}
	}

// ----------------------------------------------------------------------------
//	if possible, pops top 2 elements from the stack, adds them
//	  and pushes the result onto the stack
// ----------------------------------------------------------------------------	
	void CRPNCalc::add()
	{		
		double d1 = 0;
		double d2 = 0;
		binary_prep(d1, d2);
		if (m_error != true)
		{
			// pop two operand from stack
			m_stack.pop_front();
			m_stack.pop_front();
			// push the result
			m_stack.push_front(d1 + d2);
		}
		
	} 

// ----------------------------------------------------------------------------
//	sets the args to the popped values from the stack, if possible
//	  set error state otherwise
// ----------------------------------------------------------------------------

	void CRPNCalc::binary_prep(double& d1, double& d2)
	{
		if (m_stack.size() < 2)
		{
			m_error = true;
		}
		else
		{
			d1 = m_stack.at(0);
			d2 = m_stack.at(1);
		}
	} 

// ----------------------------------------------------------------------------
//	removes the top element from the stack
// ----------------------------------------------------------------------------
	void CRPNCalc::clearEntry()
	{
		if(!m_stack.empty())
		m_stack.pop_front();
	} 

// ----------------------------------------------------------------------------
//	empties the stack
// ----------------------------------------------------------------------------
	void CRPNCalc::clearAll()
	{
		while(!m_stack.empty())	
			m_stack.pop_front();
	} 

// ----------------------------------------------------------------------------
//	if possible, pops top 2 elements from the stack, divides them
//	  and pushes the result onto the stack
// ----------------------------------------------------------------------------
   void CRPNCalc::divide()
   {
	   double d1 = 0.0;
	   double d2 = 0.0;
	   binary_prep(d1, d2);
	   if (m_error != true)
	   {
		   // check devide zero
		   if (d1 == 0)
		   {
			   // pop the zero from the stack
			   m_stack.pop_front();
			   // set zero if devide zero
			   m_error = true;			   
		   }
		   else
		   {
			   // pop two operand from m_stack
			   m_stack.pop_front();
			   m_stack.pop_front();
			   // push the result
			   m_stack.push_front(d2 / d1);
		   }
	   }
   } 

// ----------------------------------------------------------------------------
//	if possible, pops top 2 elements from the stack,
//	  raises one element to the other power
//	  and pushes the result onto the stack
// ----------------------------------------------------------------------------
   void CRPNCalc::exp()
   {
	   double d1 = 0;
	   double d2 = 0;
	   binary_prep(d1, d2);
	   if (m_error != true)
	   {
		   if (d1 == 0 && d2 == 0)
		   {
			   m_error = true;
			   return;
		   }
		   double result = pow(d2, d1);
		   // pop two operand from m_stack
		   m_stack.pop_front();
		   m_stack.pop_front();
		   // push the result
		   m_stack.push_front(result);
	   }
   }  

// ----------------------------------------------------------------------------
//	pushes the given register's value onto the stack
// ----------------------------------------------------------------------------
	void CRPNCalc::getReg(int reg)
	{
		if (reg >= 0 && reg < NUMREGS)
			m_stack.push_front(m_registers[reg]);
		else
			m_error = true;
	}  

// ----------------------------------------------------------------------------
//	retrieves the filename from the user and loads it into m_program
// ----------------------------------------------------------------------------
	void CRPNCalc::loadProgram()
	{
		ifstream inputfile;
		string filename;
		cout << "Enter File Name of Saved Program: ";
		getline(cin, filename);
		if (filename == "x" || filename == "X")
			return;
		inputfile.open(filename);
		if (inputfile.is_open())
		{
			m_program.clear();
			string line;
			// Read line and push to m_program
			while (getline(inputfile, line))
			{
				m_program.push_back(line);
			}
			inputfile.close();
		}
		else
		{
			m_error = true;
		}
		
	}  

// ----------------------------------------------------------------------------
//	if possible, pops top 2 elements from the stack, mods them
//	  and pushes the result onto the stack
// ----------------------------------------------------------------------------
   void CRPNCalc::mod()
   {
	   double d1 = 0;
	   double d2 = 0;
	   binary_prep(d1, d2);
	   if (m_error != true)
	   {
		   double result = fmod(d2, d1);
		   // pop two operand from m_stack
		   m_stack.pop_front();
		   m_stack.pop_front();
		   // push the result
		   m_stack.push_front(result);
	   }
   } 

// ----------------------------------------------------------------------------
//	if possible, pops top 2 elements from the stack, multiplies them
//	  and pushes the result onto the stack
// ----------------------------------------------------------------------------
   void CRPNCalc::multiply()
   {
	   double d1 = 0;
	   double d2 = 0;
	   binary_prep(d1, d2);
	   if (m_error != true)
	   {		  
		   // pop two operand from m_stack
		   m_stack.pop_front();
		   m_stack.pop_front();
		   // push the result
		   m_stack.push_front(d1 * d2);
	   }
   }  

// ----------------------------------------------------------------------------
//	resets the top element of the stack to it's negative
// ----------------------------------------------------------------------------
	void CRPNCalc::neg()
	{
		double d = 0;
		unary_prep(d);
		if (m_error != true)
		{
			// pop the front m_stack
			m_stack.pop_front();
			// push negative back
			m_stack.push_front(d);
		}
	}  

// ----------------------------------------------------------------------------
//	sets the arg to the popped value from the stack, if possible
//	  sets error state otherwise
// ----------------------------------------------------------------------------
	void CRPNCalc::unary_prep(double& d)
	{
		if (m_stack.empty())
		{
			m_error = true;
			return;
		}
		else
		{
			d = m_stack.front();
			d = 0 - d;
		}
	}  

// ----------------------------------------------------------------------------
//	takes command-line input and loads it into m_program 
// ----------------------------------------------------------------------------
	void CRPNCalc::recordProgram()
	{
		string line;
		bool recorded = true;
		int lineNumber = 0;
		m_program.clear();
		while (recorded)
		{
			cout << lineNumber << ">";
			getline(cin, line);
			if (line == "p" || line == "P")
			{
				// exit recording
				recorded = false;
			}
			else
				m_program.push_back(line);
		}
	} 

// ----------------------------------------------------------------------------
//	removes the bottom of the stack and adds it to the top
// ----------------------------------------------------------------------------
	void CRPNCalc::rotateDown()
	{
		if (m_stack.empty())
		{
			m_error = true;
		}
		else
		{
			double tmp = m_stack.front();
			m_stack.pop_front();
			m_stack.push_back(tmp);
		}
	
	} 

// ----------------------------------------------------------------------------
//	removes the top of the stack and adds it to the bottom
// ----------------------------------------------------------------------------
	void CRPNCalc::rotateUp()
	{
		if (m_stack.empty())
		{
			m_error = true;
		}
		else
		{
			double tmp = m_stack.back();
			m_stack.pop_back();
			m_stack.push_front(tmp);
		}
	} 

// ----------------------------------------------------------------------------
//	runs the program in m_program 
// ----------------------------------------------------------------------------
	void CRPNCalc::runProgram()
	{
		list<string>::const_iterator it = m_program.begin();
		while (it != m_program.end())
		{
			m_buffer = *it;
			parse();
			++it;
		}
	} 

// ----------------------------------------------------------------------------
//	asks the user for a filename and saves m_program to that file
// ----------------------------------------------------------------------------
	void CRPNCalc::saveToFile()
	{
		ofstream outputfile;
		string filename;
		cout << "Enter File Name of Saved Program: " << endl;
		getline(cin, filename);		
		outputfile.open(filename);
		if (outputfile.is_open())
		{
			list<string>::const_iterator it = m_program.begin();
			while (it != m_program.end())
			{
				outputfile << *it << endl;					
				++it;
			}		
			outputfile.close();
		}
		else
		{
			m_error = true;
		}

	}  

// ----------------------------------------------------------------------------
//	gets the value from the top of the stack
//	  and places it into the given register
// ----------------------------------------------------------------------------
	void CRPNCalc::setReg(int reg)
	{
		if (reg >= 0 && reg < NUMREGS)
			m_registers[reg] = m_stack.front();
		else
			m_error = true;
	} 

// ----------------------------------------------------------------------------
//	if possible, pops top 2 elements from the stack, subtracts them
//	  and pushes the result onto the stack
// ----------------------------------------------------------------------------
   void CRPNCalc::subtract()
   {
	   double d1 = 0;
	   double d2 = 0;
	   binary_prep(d1, d2);
	   if (m_error != true)
	   {
		   // pop two operand from m_stack
		   m_stack.pop_front();
		   m_stack.pop_front();
		   // push the result
		   m_stack.push_front(d2 - d1);
	   }
   } 

// ----------------------------------------------------------------------------
//	inputs a line from the given stream
// ----------------------------------------------------------------------------
	void CRPNCalc::input(istream &istr)
	{		
		getline(istr, m_buffer);
		m_instrStream.str(m_buffer);
		// call paser
		parse();
	} 

// ----------------------------------------------------------------------------
//	ostream's << defined for CRPNCalc
// ----------------------------------------------------------------------------
   ostream &operator <<(ostream &ostr, CRPNCalc &calc)
	{
		calc.print(ostr);
		return ostr;
	} 


// ----------------------------------------------------------------------------
//	istream's >> defined for CRPNCalc
// ----------------------------------------------------------------------------
	istream &operator >>(istream &istr, CRPNCalc &calc)
	{
		calc.input(istr);
		return istr;
	} 
} // end namespace PB_CALC
