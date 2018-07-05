///////////////////////////////////////////////////////////////////////////
// CheckinComponent.cpp - test check-out operations                      //
// Ver 1.0                                                               //
// Jiacheng Zhang, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains a single class Checkin with public functions:
* - doCheckout()    : find a specific file and all of its dependenciy files, and copy them
*                     to a specific location (../Staging/Checkout/)
* - getDependency() : use graph walk to get all of dependencies
*  Required Files:
*  ---------------
*  Graph.h      MetaData.h       DbCore.h
*  Query.h      Version.h
*  Maintenance History:
* --------------------
*  2 Mar 2018
* - ver 1.0
*
*/

#include <string>
#include <vector>
#include "../CheckinComponent/Checkin.h"
#include "../VersionComponent/Version.h"
#include "../Utilities/FileSystem.h"
#include "../Metadata/MetaData.h"
#include "../Utilities/Graph.h"
#include "../Db/DateTime.h"
#include "../Db/DbCore.h"
#include"../Db/PayLoad.h"
#include "../Db/DbCore.h"
#include"../Db/Query.h"
#include "Checkout.h"

using namespace std;
using namespace NoSqlDb;
using namespace GraphProcessing;
#ifdef TEST_CHECKOUT
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
	Graph<Key> graph;
	Checkout<PayLoad> checkout;
	checkout.doCheckout("NoSqlDb::DbCore.h",db,graph);
    return 0;
}

#endif