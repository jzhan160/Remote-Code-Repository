/////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Executive package of the program            //
// ver 1.0                                                         //
// Jiacheng Zhang                                                  //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a RepoProvider class:
* - provides mechanism to share a test repository between test functions
* - without explicitly passing as a function argument.
* And a MetaDataProvider class which provides mechanism to metadata for functional tests:
* - metadata1() : provide metadata of Definitions.h
* - metadata2() : provide metadata of DateTime.h
* - metadata3() : provide metadata of DateTime.cpp
* - metadata4() : provide metadata of DbCore.h
* - metadata5() : provide metadata of DbCore.cpp
* - metadata6() : provide metadata of Query.h

* The package also provides functions for testing:
* - ExecuteTest1(): holds a collection of tests for check-in and maintainance.
* - ExecuteTest2(): holds a collection of tests for check-out and browse .
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* TestExecutive.h
* MetaData.h, PayLoad.h
* Repository.h,FileSystem.h
* StringUtilities.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 4 Feb 2018
* ver 2.0 : 6 Feb 2018
*/


#include <stack>
#include <string>
#include <cctype>
#include <locale>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include "TestExecutive.h"

using namespace std;
using namespace NoSqlDb;
using Key = std::string;

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};


///////////////////////////////////////////////////////////////////////
// RepoProvider class
// - provides mechanism to share a test repository between test functions
//   without explicitly passing as a function argument.

class RepoProvider
{
public:
	Repository<PayLoad>& repo() { return repo_; }

private:
	static Repository<PayLoad> repo_;
};
Repository<PayLoad> RepoProvider::repo_;

///////////////////////////////////////////////////////////////////////
// MetaDataProvider class
// - provides mechanism to metadata for functional tests

class MetaDataProvider {
public:
	MetaData<PayLoad> metadata1();
	MetaData<PayLoad> metadata2();
	MetaData<PayLoad> metadata3();
	MetaData<PayLoad> metadata4();
	MetaData<PayLoad> metadata5();
	MetaData<PayLoad> metadata6();
};

//----< metadata of Definitions.h >------------------------------------------
MetaData<PayLoad> MetaDataProvider::metadata1() {
	MetaData<PayLoad> md1;
	md1.name() = "Definitions.h";
	md1.author() = "Fawcette";
	md1.path() = "../Staging/";
	md1.head() = "NoSqlDb";
	md1.description() = "Definitions.h is a head file containing definitions";
	DateTime dt;
	md1.dateTime() = dt;
	PayLoad pl;
	vector<std::string> categories;
	categories.push_back("DB");
	pl.value() = "../Storage/";
	pl.categories() = categories;
	pl.setStatus(pl.open);
	md1.payLoad() = pl;
	return md1;
}

//----< metadata of DateTime.h >------------------------------------------
MetaData<PayLoad> MetaDataProvider::metadata2() {
	MetaData<PayLoad> md2;
	md2.name() = "DateTime.h";
	md2.author() = "Fawcette";
	md2.path() = "../Staging/";
	md2.head() = "Utility";
	md2.description() = "DateTime.h is a head file to generate time.";
	DateTime dt;
	md2.dateTime() = dt;
	PayLoad pl;
	vector<std::string> categories;
	categories.push_back("DB");
	categories.push_back("Repo");
	pl.value() = "../Storage/";
	pl.categories() = categories;
	pl.setStatus(pl.open);
	md2.payLoad() = pl;
	return md2;
}

//----< metadata of DateTime.cpp >------------------------------------------
MetaData<PayLoad> MetaDataProvider::metadata3() {
	MetaData<PayLoad> md3;
	md3.name() = "DateTime.cpp";
	md3.author() = "Fawcette";
	md3.path() = "../Staging/";
	md3.head() = "Utility";
	md3.dependencies().push_back("Utility::DateTime.h.1");
	md3.description() = "DateTime.cpp is a cpp file to implement DateTime.h";
	DateTime dt;
	md3.dateTime() = dt;
	PayLoad pl;
	vector<std::string> categories;
	categories.push_back("DB");
	categories.push_back("Repo");
	pl.value() = "../Storage/";
	pl.categories() = categories;
	pl.setStatus(pl.open);
	md3.payLoad() = pl;
	return md3;
}

//----< metadata of DbCore.h >------------------------------------------
MetaData<PayLoad> MetaDataProvider::metadata4() {
	MetaData<PayLoad> md4;
	md4.name() = "DbCore.h";
	md4.author() = "Fawcette";
	md4.path() = "../Staging/";
	md4.head() = "NoSqlDb";
	md4.dependencies().push_back("Utility::DateTime.h.1");
	md4.dependencies().push_back("Utility::DateTime.cpp.1");
	md4.dependencies().push_back("NoSqlDb::Definitions.h.1");
	md4.description() = "DbCore.h is a head file declaring DB functions.";
	DateTime dt;
	md4.dateTime() = dt;
	PayLoad pl;
	vector<std::string> categories;
	categories.push_back("DB");
	pl.value() = "../Storage/";
	pl.categories() = categories;
	pl.setStatus(pl.open);
	md4.payLoad() = pl;
	return md4;
}

//----< metadata of DbCore.cpp >------------------------------------------
MetaData<PayLoad> MetaDataProvider::metadata5() {
	MetaData<PayLoad> md5;
	md5.name() = "DbCore.cpp";
	md5.author() = "Fawcette";
	md5.path() = "../Staging/";
	md5.head() = "NoSqlDb";
	md5.dependencies().push_back("Utility::DateTime.h.1");
	md5.dependencies().push_back("Utility::DateTime.cpp.1");
	md5.dependencies().push_back("NoSqlDb::DbCore.h.1");
	md5.description() = "DbCore.cpp is a cpp file to test DB functions.";
	DateTime dt;
	md5.dateTime() = dt;
	PayLoad pl;
	vector<std::string> categories;
	categories.push_back("DB");
	pl.value() = "../Storage/";
	pl.categories() = categories;
	pl.setStatus(pl.open);
	md5.payLoad() = pl;
	return md5;
}

//----< metadata of Query.h >------------------------------------------
MetaData<PayLoad> MetaDataProvider::metadata6() {
	MetaData<PayLoad> md5;
	md5.name() = "Query.h";
	md5.author() = "Fawcette";
	md5.path() = "../Staging/";
	md5.head() = "NoSqlDb";
	md5.dependencies().push_back("NoSqlDb::DbCore.h.1");
	md5.description() = "DbCore.cpp is a head file to declare query functions.";
	DateTime dt;
	md5.dateTime() = dt;
	PayLoad pl;
	vector<std::string> categories;
	categories.push_back("DB");
	pl.value() = "../Storage/";
	pl.categories() = categories;
	pl.setStatus(pl.open);
	md5.payLoad() = pl;
	return md5;
}


///////////////////////////////////////////////////////////////////////
// test functions

//----< demo requirement #1 >------------------------------------------
bool testR1()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions.";
	putLine();
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------
bool testR2()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine();
	return true;
}

//----< demo requirement #3 >----------------------------
bool testR3()
{
	Utilities::title("Demonstrating Requirement #3- provides C++ packages which enforce the Single Responsiblity Principle");
	putLine();
	cout << "  Pacakges: \n";
	FileSystem::Directory dir;
	vector<string> packages = dir.getDirectories("../");
	for (auto package : packages) {
		if (package == ".vs" || package == "Debug" || package == "Staging" || package == "Storage")
			continue;
		cout << "  "<<package << "\n";
			
	}
	putLine();
	return true;
}

//----< show graph walk in the repository >----------------------------
bool testR4() {
	Utilities::title("Demonstrating Requirement  - show the whole graph built by files in the repository");
	RepoProvider repop;
	Repository<PayLoad> repo = repop.repo();
	repo.getDb()["Utility::DateTime.h.1"].children().push_back("Utility::DateTime.cpp.1");
	repo.dependencyGraph("Utility::DateTime.h.1", repo.getDb()["Utility::DateTime.h.1"].children());
	repo.getDb()["NoSqlDb::DbCore.h.1"].children().push_back("NoSqlDb::DbCore.cpp.1");
	repo.dependencyGraph("NoSqlDb::DbCore.h.1", repo.getDb()["NoSqlDb::DbCore.h.1"].children());
	repo.getDb()["NoSqlDb::DbCore.cpp.1"].children().push_back("NoSqlDb::Query.h.1");
	repo.dependencyGraph("NoSqlDb::DbCore.cpp.1", repo.getDb()["NoSqlDb::DbCore.cpp.1"].children());

	repo.showGraph();
	putLine();
	return true;
}

//----< demo requirement about new check-in >----------------------------
bool testCheckin1() {
	Utilities::title("Demonstrating Requirement   - start new check-ins file by file and store file in ../Storage/");
	putLine();
	std::cout << "  Check in 6 new files one by one: \n";
	RepoProvider repop;
	Repository<PayLoad> repo = repop.repo();
	MetaDataProvider mdp;
	repo.checkin(mdp.metadata1());
	repo.checkin(mdp.metadata2());
	repo.checkin(mdp.metadata3());
	repo.checkin(mdp.metadata4());
	repo.checkin(mdp.metadata5());
	repo.checkin(mdp.metadata6());
	showDb(repo.getDb());
	return true;
}

//----< demo requirement about open check-in >----------------------------
bool testCheckin2() {
	Utilities::title("Demonstrating Requirement   - start an open check-in file without altering its version number");
	putLine();
	RepoProvider repop;
	Repository<PayLoad> repo = repop.repo();
	MetaDataProvider mdp;
	MetaData<PayLoad> md = mdp.metadata1();
	md.description() = "open check-in definition.h";
	repo.checkin(md);
	showHeader();
	showRecord("NoSqlDb::Definitions.h.1",repo.getDb()["NoSqlDb::Definitions.h.1"]);
	return true;
}

//----< demo requirement about closed check-in >----------------------------
bool testCheckin3() {
	Utilities::title("Demonstrating Requirement  - supports for close-check-in with a new version number");
	putLine();
	RepoProvider repop;
	Repository<PayLoad> repo = repop.repo();
	MetaDataProvider mdp;
	cout << "  Close the version 1.0 of file NoSqlDb::Definitions.h";
	repo.closeVersion("NoSqlDb::Definitions.h.1");
	cout << "\n  Check in the file NoSqlDb::Definitions.h with a new version 2.0";
	cout << "\n  Show the updated repo: ";
	repo.checkin(mdp.metadata1());
	showHeader();
	showRecord("NoSqlDb::Definitions.h.2", repo.getDb()["NoSqlDb::Definitions.h.2"]);
	return true;
}

//----< demo requirement about closing a version >----------------------------
bool testCheckin4() {
	Utilities::title("Demonstrating Requirement  - supports for checking dependency status before closing a file version");
	putLine();
	Key key = "NoSqlDb::DbCore.h.1";
	RepoProvider repop;
	Repository<PayLoad> repo = repop.repo();
	cout << "  Attempt to close the version 1.0 of file NoSqlDb::DbCore.h";
	repo.closeVersion(key);
	cout << "\n  Try to close dependencies:";
	repo.closeVersion("NoSqlDb::Definitions.h.1");
	repo.closeVersion("Utility::DateTime.h.1");
	repo.closeVersion("Utility::DateTime.cpp.1");
	repo.closeVersion("NoSqlDb::DbCore.cpp.1");

	repo.closeVersion(key);

	return true;
}

//----< demo requirement about ownership detection >----------------------------
bool testCheckin5() {
	Utilities::title("Demonstrating Requirement  - supports checking ownerships when the user checks in");
	putLine();
	RepoProvider repop;
	Repository<PayLoad> repo = repop.repo();
	MetaDataProvider mdp;
	MetaData<PayLoad> md = mdp.metadata1();
	md.author() = "Jiacheng";
	repo.checkin(md);
	putLine();
	return true;
}

//----< demo requirement about check-out  >----------------------------
bool testCheckout()
{
	Utilities::title("Demonstrating Requirement - supports check-out which retrieves the file without version to path ../Staging/");
	RepoProvider repop;
	Repository<PayLoad> repo = repop.repo();
	Key key = "NoSqlDb::DbCore.h.1";
	cout << "\n  check out the file: "<< key << " \n";
	repo.checkout(key);
	putLine();
	return true;
}

//----< demo requirement #5 >------------------------------------------
bool testR5()
{
	Utilities::title("Demonstrating Requirement #5 - supports browsing of files by displaying file descriptions and contents");
	putLine();
	RepoProvider repop;
	Repository<PayLoad> repo = repop.repo();
	repo.browse("NoSqlDb::DbCore.h.1");
	putLine();
	return true;
}

//----< function which holds a collection of tests for basic operation tests >----------------------------
bool ExecuteTests1() {
	TestExecutive ex;
	TestExecutive::TestStr ts1{ testR1, "Use C++11" };
	TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };
	TestExecutive::TestStr ts3{ testR3, " provides C++ packages which enforce the Single Responsiblity Principle" };
	TestExecutive::TestStr ts4{ testR4, "show the whole graph built by files in the repository" };
	TestExecutive::TestStr ts5{ testCheckin1, "start new check-ins file by file and store file in ../Storage/" };

	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3);
	ex.registerTest(ts5);
	ex.registerTest(ts4);

	bool result = ex.doTests();
	return result;
}

//----< function which holds a collection of tests for basic operation tests >----------------------------
bool ExecuteTests2() {
	TestExecutive ex;
	TestExecutive::TestStr ts6{ testCheckin2, "start an open check-in file without altering its version number" };
	TestExecutive::TestStr ts7{ testCheckin3, "supports for close-check-in with a new version number" };
	TestExecutive::TestStr ts8{ testCheckin4, "supports for checking dependency status before closing a file version" };
	TestExecutive::TestStr ts9{ testCheckin5, "supports checking ownerships when the user checks in" };
	TestExecutive::TestStr ts10{ testCheckout, "supports check-out which retrieves the file without version to path ../Staging/" };
	TestExecutive::TestStr ts11{ testR5, "supports browsing of files by displaying file descriptions and contents" };

	ex.registerTest(ts6);
	ex.registerTest(ts7);
	ex.registerTest(ts8);
	ex.registerTest(ts9);
	ex.registerTest(ts10);
	ex.registerTest(ts11);

	bool result = ex.doTests();
	return result;
}

int main()
{
	Utilities::Title("Testing Nosql Database");	putLine();
	bool result = ExecuteTests1() & ExecuteTests2();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";
	getchar();
	getchar();
	return 0;
}

