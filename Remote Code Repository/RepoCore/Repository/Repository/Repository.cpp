//////////////////////////////////////////////////////////////////////////////
// Repository.cpp - test basic operations of repository                     //
// Ver 1.0                                                                  //
// Jiacheng Zhang, CSE687 - Object Oriented Design, Spring 2018             //
//////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains a single class Repository with public functions:
* - checkin()         : checkin files with metadata
* - checkout()        : checkout file of a specific version
* - browse()          : browse files and dependencies
* - closeVersion()    : close check-in status of a specific file
* - dependencyGraph() : build a file graph using all key in repository
* - showGraph()       : show the file graph
* - getDb()           : return the nosql database in the repository
*  Required Files:
*  ---------------
*  DateTime.h      DbCore.h     Checkin.h
*  Checkout.h      MetaData.h   PayLoad.h
*  Browse.h        Graph.h
*  Maintenance History:
* --------------------
*  2 Mar 2018
* - ver 1.0
*
*/
#include "Repository.h"
#include "../Db/DbCore.h"
#include"../Db/PayLoad.h"
#include "../Db/DateTime.h"
#include<vector>
using namespace NoSqlDb;

#ifdef  TEST_REPO

int main()
{   
	Repository<PayLoad> repo;
	MetaData<PayLoad> md1;
	md1.name() = "DbCore.h";
	md1.author() = "Jiacheng";
	md1.path() = "../Staging/";
	md1.head() = "NoSqlDb";
	md1.description() = "DbCore.h";
	DateTime dt;
	md1.dateTime()= dt;
	md1.dependencies().push_back("NoSqlDb::Definitions.h");
	PayLoad pl;
	vector<std::string> categories;
	categories.push_back("DB");
	pl.value() = "../Storage/";
	pl.categories() = categories;
	md1.payLoad() = pl;
	repo.checkin(md1);

	showDb(repo.getDb());
	std::cout << "\n";
	repo.checkout("NoSqlDb::DbCore.h");

	MetaData<PayLoad> md2;
	md2.name() = "DbCore.h";
	md2.author() = "Jiacheng";
	md2.path() = "../Staging/";
	md2.head() = "NoSqlDb";
	md2.description() = "DbCore.h";
	DateTime dt1;
	md2.dateTime() = dt1;
	md2.dependencies().push_back("NoSqlDb::Definitions.h");
	PayLoad pl1;
	vector<std::string> categories1;
	categories1.push_back("DB");
	pl1.value() = "../Storage/";
	pl1.categories() = categories1;
	md2.payLoad() = pl1;
	repo.checkin(md2);

	std::cout << "\n";

	showDb(repo.getDb());
    return 0;
}

#endif 
