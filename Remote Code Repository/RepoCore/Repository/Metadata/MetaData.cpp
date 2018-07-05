///////////////////////////////////////////////////////////////////////////
// MetaData.cpp - test operations of MetaData class                       //
// Ver 1.0                                                               //
// Jiacheng Zhang, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains a class MetaData with properties:
* - name         : file name string
* - head         : namespace string
* - path         : storage path of the file
* - author       ; owner of the file
* - dateTime     : DateTime instance
* - description  : description string
* - dependencies : vector<string> containing dependency file names
* - payLoad      : contains path and category information
* and with methods:
* - show()         : displays the MetaData instance on the console
*
*  Required Files:
*  ---------------
*  DateTime.h  PayLoad.h  DbCore.h
*  Maintenance History:
* --------------------
*  2 Mar 2018
* - ver 1.0
*
*/
#include <string>
#include <vector>
#include <iostream>
#include "MetaData.h"
#include "../Db/PayLoad.h"
#include "../Db/DateTime.h"
#include "../Db/DbCore.h"

using namespace std;
using namespace NoSqlDb;

#ifdef TEST_MD
int main()
{
	MetaData<PayLoad> md;
	md.author() = "Jiacheng";
	md.name() = "file1.cpp";
	md.description() = "This is the file1.cpp.";
	md.dependencies().push_back("file1.h");
	md.dependencies().push_back("file2.h");
	DateTime dt;
	md.dateTime() = dt;
	PayLoad payload;
	payload.value() = "../Storage/";
	payload.categories().push_back("show");
	payload.categories().push_back("display");
	md.show();
	std::cout << "\n  ";
	std::cout << std::setw(30) << std::left << "Payload Value";
	std::cout << std::setw(20) << std::left << "Categories";
	std::cout << "\n  ";
	std::cout << std::setw(30) << std::left << "----------------";
	std::cout << std::setw(20) << std::left << "--------------------------------------";
	std::cout << "\n  ";
	std::cout << std::setw(30) << std::left << payload.value();
	std::cout << std::setw(40);
	for (auto cat : payload.categories())
	{
		 std::cout<< cat << " " ;
	}
	std::cout << "\n  ";
	return 0;
}

#endif