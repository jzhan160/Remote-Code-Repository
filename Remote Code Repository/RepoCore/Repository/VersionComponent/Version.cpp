////////////////////////////////////////////////////////////////////////////////
// Version.CPP -  Test operaions of the Version class                         //
// Ver 1.0                                                                    //
// Jiacheng Zhang, CSE687 - Object Oriented Design, Spring 2018               //
////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains a single class Version with public functions:
* - getLatestVersion() : get the latest version of a given file name
* - removeVersion()    : strips version number from filename
* - getVersion()       : get the version number in a file name string
* - addVersion()       : adds version number to filename
* - getFileName()      : convert a key to a valid file name
* - removeHead()       : remove the namespace in a file name
*  Required Files:
*  ---------------
*  Query.h      DbCore.h
*  Maintenance History:
* --------------------
*  2 Mar 2018
* - ver 1.0
*
*/

#include <string>
#include <vector>
#include "Version.h"
#include "../Db/PayLoad.h"

#ifdef TEST_VERSION
using namespace std;
using namespace NoSqlDb;

int main()
{
	Version<PayLoad> version;
	Key key = "namespace::file1.cpp.1";
	int ver = version.getVersion(key);
	string fileName = version.getFileName(key);
	string fileNameWithoutHead = version.removeHead(fileName);
	string fileNameWithoutVer = version.removeHead(fileName);
	string fileNameWithVer = version.addVersion(fileName, ver+1);
	cout << "The version of key " << key << "is: "<< ver <<"\n";
	cout << "The fileName is " << fileName<< "\n";
	cout << "The fileName Without Head is " << fileNameWithoutHead<<"\n";
	cout << "The fileName Without Version is " << fileNameWithoutVer<<"\n";
	cout << "The fileName with a new version is " << fileNameWithVer << "\n";

	return 0;
}
#endif

