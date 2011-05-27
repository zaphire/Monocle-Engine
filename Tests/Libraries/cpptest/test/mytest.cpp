// ---
//
// $Id: mytest.cpp,v 1.5 2008/07/11 16:49:43 hartwork Exp $
//
// CppTest - A C++ Unit Testing Framework
// Copyright (c) 2003 Niklas Lundell
//
// ---
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// ---
//
// Test program demonstrating all assert types and output handlers.
//
// ---

#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "../src/cpptest.h"

using namespace std;

// Tests unconditional fail asserts
//
class FailTestSuite : public Test::Suite
{
public:
	FailTestSuite()
	{
		TEST_ADD(FailTestSuite::success)
		TEST_ADD(FailTestSuite::always_fail)
	
	}
	
private:
	void success() {}
	
	void always_fail()
	{
		// This will always fail
		//
		TEST_FAIL("unconditional fail");
	}
};

// Tests compare asserts
//
class CompareTestSuite : public Test::Suite
{
public:
	CompareTestSuite()
	{
		TEST_ADD(CompareTestSuite::success)
		TEST_ADD(CompareTestSuite::compare)
		TEST_ADD(CompareTestSuite::delta_compare)
	}
	
private:
	void success() {}
	
	void compare()
	{
		// Will succeed since the expression evaluates to true
		//
		TEST_ASSERT(1 + 1 == 2)
		
		// Will fail since the expression evaluates to false
		//
		TEST_ASSERT(0 == 1);
	}
	
	void delta_compare()
	{
		// Will succeed since the expression evaluates to true
		//
		TEST_ASSERT_DELTA(0.5, 0.7, 0.3);
		
		// Will fail since the expression evaluates to false
		//
		TEST_ASSERT_DELTA(0.5, 0.7, 0.1);
	}
};

// Tests throw asserts
//
class ThrowTestSuite : public Test::Suite
{
public:
	ThrowTestSuite()
	{
		TEST_ADD(ThrowTestSuite::success)
		TEST_ADD(ThrowTestSuite::test_throw)
	}
	
private:
	void success() {}
	
	void test_throw()
	{
		// Will fail since the none of the functions throws anything
		//
		TEST_THROWS_MSG(func(), int, "func() does not throw, expected int exception")
		TEST_THROWS_MSG(func_no_throw(), int, "func_no_throw() does not throw, expected int exception")
		TEST_THROWS_ANYTHING_MSG(func(), "func() does not throw, expected any exception")
		TEST_THROWS_ANYTHING_MSG(func_no_throw(), "func_no_throw() does not throw, expected any exception")
		
		// Will succeed since none of the functions throws anything
		//
		TEST_THROWS_NOTHING(func())
		TEST_THROWS_NOTHING(func_no_throw())
		
		// Will succeed since func_throw_int() throws an int
		//
		TEST_THROWS(func_throw_int(), int)
		TEST_THROWS_ANYTHING(func_throw_int())
		
		// Will fail since func_throw_int() throws an int (not a float)
		//
		TEST_THROWS_MSG(func_throw_int(), float, "func_throw_int() throws an int, expected a float exception")
		TEST_THROWS_NOTHING_MSG(func_throw_int(), "func_throw_int() throws an int, expected no exception at all")
	}
	
	void func() {}
	void func_no_throw() throw() {}
	void func_throw_int() throw(int) { throw 13; }
};

enum OutputType
{
	Compiler,
	Html,
	TextTerse,
	TextVerbose
};

static void
usage()
{
	cout << "usage: mytest [MODE]\n"
		 << "where MODE may be one of:\n"
		 << "  --compiler\n"
		 << "  --html\n"
		 << "  --text-terse (default)\n"
		 << "  --text-verbose\n";
	exit(0);
}

static auto_ptr<Test::Output>
cmdline(int argc, char* argv[])
{
	if (argc > 2)
		usage(); // will not return
	
	Test::Output* output = 0;
	
	if (argc == 1)
		output = new Test::TextOutput(Test::TextOutput::Verbose);
	else
	{
		const char* arg = argv[1];
		if (strcmp(arg, "--compiler") == 0)
			output = new Test::CompilerOutput;
		else if (strcmp(arg, "--html") == 0)
			output =  new Test::HtmlOutput;
		else if (strcmp(arg, "--text-terse") == 0)
			output = new Test::TextOutput(Test::TextOutput::Terse);
		else if (strcmp(arg, "--text-verbose") == 0)
			output = new Test::TextOutput(Test::TextOutput::Verbose);
		else
		{
			cout << "invalid commandline argument: " << arg << endl;
			usage(); // will not return
		}
	}
	
	return auto_ptr<Test::Output>(output);
}

// Main test program
//
int
main(int argc, char* argv[])
{
	try
	{
		// Demonstrates the ability to use multiple test suites
		//
		Test::Suite ts;
		ts.add(auto_ptr<Test::Suite>(new FailTestSuite));
		ts.add(auto_ptr<Test::Suite>(new CompareTestSuite));
		ts.add(auto_ptr<Test::Suite>(new ThrowTestSuite));

		// Run the tests
		//
		auto_ptr<Test::Output> output(cmdline(argc, argv));
		ts.run(*output, true);

		Test::HtmlOutput* const html = dynamic_cast<Test::HtmlOutput*>(output.get());
		if (html)
			html->generate(cout, true, "MyTest");
	}
	catch (...)
	{
		cout << "unexpected exception encountered\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

