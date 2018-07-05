#pragma once
////////////////////////////////////////////////////////////////////////////////
// Version.h - manages version numbering for all files held in the Repository //
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
#include <unordered_map>
#include "../Db/DbCore.h"
#include "../Db/Query.h"

using VerNum = int;       
using Key = NoSqlDb::Key;

///////////////////////////////////////////////////////////////////
//Version Class
//- manages version numbering for all files held in the Repository 

template<typename T>
class Version {
public:
	int getLatestVersion(const std::string& fileName, NoSqlDb::DbCore<T>& db);
	std::string removeVersion(std::string fileName);
	std::string addVersion(std::string fileName,int verNum);
	int getVersion(const Key& key);
	std::string getFileName(const Key& key);
	std::string getHead(const  Key& key);
	std::string removeHead(const std::string& fileName);
	std::string getKeyName(const std::string& fileName);
};

/*----< get the latest version of a given file name >------------------*/
template<typename T>
int Version<T>::getLatestVersion(const std::string& fileName, NoSqlDb::DbCore<T>& db)
{
	Query<T> q1(db);
	Conditions<T> conds1;
	conds1.name(fileName);
	q1.select(conds1);
	if (q1.keys().size() == 0)
		return 0;
	VerNum ver = 0;
	for (auto file : q1.keys())
	{
		auto pos = file.find_last_of(".");
		std::string verStr = file.substr(pos + 1);
		ver = atoi(verStr.c_str()) > ver ? atoi(verStr.c_str()) : ver;
	}
	return ver;
}

/*----< strips version number from filename >------------------*/
template<typename T>
std::string Version<T>::removeVersion(std::string fileName)
{
	int version;
	auto pos = fileName.find_last_of('.');
	std::string end = fileName.substr(pos + 1);
	version = atoi(end.c_str());
	if (version)
		return fileName.substr(0, pos);
	return fileName;
}

/*----< adds version number to filename >----------------------*/
template<typename T>
std::string Version<T>::addVersion(std::string fileName, int verNum)
{
	std::string fileNameWithoutVer = removeVersion(fileName);
	return fileNameWithoutVer + "." + to_string(verNum);
}

/*----< get the version number in a file name string >----------------------*/
template<typename T>
int Version<T>::getVersion(const NoSqlDb::Key& key) {
	auto pos = key.find_last_of(".");
	std::string verStr = key.substr(pos + 1);
	int ver = atoi(verStr.c_str());
	return ver;
}

/*----< convert a key to a valid file name>----------------------*/
template<typename T>
std::string  Version<T>::getFileName(const Key& key) {
	auto firstCol = key.find_first_of(":");
	auto secondCol = key.find_last_of(":");
	std::string fileName = key.substr(0, firstCol) + "_" + key.substr(secondCol + 1);
	return fileName;
}

template<typename T>
std::string  Version<T>::getHead(const  Key& key) {
	auto pos = key.find_first_of(":");
	return key.substr(0,pos);
}

/*----< remove the namespace in a file name>----------------------*/
template<typename T>
std::string Version<T>::removeHead(const std::string& fileName) {
	auto pos = fileName.find_last_of("_");
	return fileName.substr(pos + 1);
}

/*----< convert a file name to a key name>----------------------*/
template<typename T>
std::string Version<T>::getKeyName(const std::string& fileName) {
	int pos = fileName.find("_");
	return fileName.substr(0, pos) + "::" + fileName.substr(pos+1);
}