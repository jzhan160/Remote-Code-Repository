#pragma once
///////////////////////////////////////////////////////////////////////
// TestExecutive.h - provides single-user test harness                        //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes:
* - TestExecutor    Executes single test in the context of a try-catch block
* - TestExecutive   Uses TestExecutor to run a sequence of tests
*
* Required Files:
* ---------------
*   Test.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 12 Jan 2018
* - first release
* - refactored from earlier Utilities.h
*
* Notes:
* ------
* - Designed to provide all functionality in header file.
* - Implementation file only needed for test and demo.
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/

#include <vector>
#include <functional>
#include <iostream>
#include "../Utilities/StringUtilities.h"
#include "../Utilities/FileSystem.h"
#include "../Db/DbCore.h"
#include "../Repository/Repository.h"
#include "../Db/PayLoad.h"
#include "../Metadata/MetaData.h"

/////////////////////////////////////////////////////////////////////
// TestExecutor class
// - supports execution of callable objects for testing in the
//   context of a try-catch block.

template<typename T>
class TestExecutor
{
public:
	bool execute(T t, const std::string& name, std::ostream& out = std::cout);
private:
	void check(bool result, std::ostream& out);
};
//----< execute tests in the context of a try-catch block >----------

template <typename T>
bool TestExecutor<T>::execute(T t, const std::string& name, std::ostream& out)
{
	bool result = false;
	try
	{
		result = t(); // t is a test function which returns true or false
		check(result, out);
		out << " -- \"" << name << "\"\n";
	}
	catch (std::exception& ex)
	{
		check(false, out);
		out << " -- \"" << name << "\" ";
		out << ex.what() << "\n";
	}
	return result;
}
//----< display test results >---------------------------------------

template<typename T>
void TestExecutor<T>::check(bool result, std::ostream& out) // output true or false
{
	if (result)
		out << "\n  passed";
	else
		out << "\n  failed";
}

///////////////////////////////////////////////////////////////////////
// TestExecutive class
// - executes a sequence of tests with the help of TestExecutor

class TestExecutive
{
public:
	using Test = std::function<bool()>; // the test function
	using TestStr = struct {
		Test test;
		std::string testName;
	};
	using Tests = std::vector<TestStr>;  //a bunch of testStr(test function and test name)

	bool doTests();
	void registerTest(Test t, const std::string& testName);
	void registerTest(TestStr ts);
private:
	Tests tests_;
};

inline void TestExecutive::registerTest(Test t, const std::string& testName)
{
	TestStr ts{ t, testName };
	tests_.push_back(ts);
}

inline void TestExecutive::registerTest(TestStr ts)
{
	tests_.push_back(ts);
}

inline bool TestExecutive::doTests()
{
	TestExecutor<Test> tester;
	bool result = true;
	for (auto item : tests_)
	{
		bool tResult = tester.execute(item.test, item.testName);
		if (tResult == false)
			result = false;
	}
	return result;
}


