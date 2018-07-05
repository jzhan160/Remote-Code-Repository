#pragma once
///////////////////////////////////////////////////////////////////////////
// CheckinComponent.cpp - test Check-in operations                       //
// Ver 1.0                                                               //
// Jiacheng Zhang, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains a single class Checkin with public functions:
* - getCheckinName()         : Get a valid file name from the check-in metadata
* - doCheckin()              : Execute the check-in command by adding information
* -                            to database and copying file to the repo storage dir
* - closeVersion             : Close the check-in status of a file with a specific version
* - checkDependenciesStatus  : Check the status of the file's dependencies before closing the file
* - checkDependenciesExist   : Check whether the file's dependencies are in the repo before check-in
* - checkOwnership           : Check the ownership of the file before check-in
*  Required Files:
*  ---------------
*  FileSystem.h MetaData.h  DbCore.h
*  Query.h      Version.h   Checkin.h
*  Maintenance History:
* --------------------
*  2 Mar 2018
* - ver 1.0
*
*/
#include <string>
#include <vector>
#include "Checkin.h"
#include "../Db/DbCore.h"
#include"../Db/PayLoad.h"
#include "../Db/DateTime.h"
#include "../Utilities/FileSystem.h"
#include"../Metadata/MetaData.h"
#include "../Db/DbCore.h"
#include"../Db/Query.h"
#include"../VersionComponent/Version.h"
using namespace std;
using namespace NoSqlDb;

#ifdef TEST_CHECKIN
int main()
{  
	DbCore<PayLoad> db;
	MetaData<PayLoad> md1;
	md1.name() = "DbCore.h";
	md1.author() = "Jiacheng";
	md1.path() = "../Staging/";
	md1.head() = "NoSqlDb";
	md1.description() = "DbCore.h";
	DateTime dt;
	md1.dateTime() = dt;
	md1.dependencies().push_back("NoSqlDb::Definitions.h");
	PayLoad pl;
	vector<std::string> categories;
	categories.push_back("DB");
	pl.value() = "../Storage/";
	pl.categories() = categories;
	md1.payLoad() = pl;
	Checkin<PayLoad> checkin;
	checkin.doCheckin(md1, db);
	showDb(db);
	
	return 0;
}

#endif