///////////////////////////////////////////////////////////////////////////
// Browse.cpp - test browse functions                                    //
// Ver 1.0                                                               //
// Jiacheng Zhang, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains a single class Browse with public functions:
* - doBrowse()   : Browse a given version of file and its dependencies to show their metadata.
* - showContent(): Display contents of the file using child process;
*
*  Required Files:
*  ---------------
*  DbCore.h  Version.h  process.h
*
*  Maintenance History:
* --------------------
*  2 Mar 2018
* - ver 1.0
*
*/
#include "Browse.h"
#include "process.h"
#include "../Db/PayLoad.h"
#include "../Db/DbCore.h"

using namespace NoSqlDb;
#ifdef TEST_BROWSE
int main()
{
	DbCore<PayLoad> db;
	DbElement<PayLoad> ele;
	ele.name() = "DbCore.h";
	ele.descrip() = "DbCore.h";
	db["DbCore.h"] = ele;
	Browse<PayLoad> browse;
	browse.showContent("DbCore.h","../Staging/");
	browse.doBrowse(db, "DbCore.h");
    return 0;
}

#endif