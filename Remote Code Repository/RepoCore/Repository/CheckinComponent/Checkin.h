#pragma once
///////////////////////////////////////////////////////////////////////////
// CheckinComponent.h - manages adding new files to Repository           //
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
*  Query.h      Version.h
*  Maintenance History:
* --------------------
*  2 Mar 2018
* - ver 1.0
*
*/
#include <string>
#include <vector>
#include "../Utilities/FileSystem.h"
#include"../Metadata/MetaData.h"
#include "../Db/DbCore.h"
#include"../Db/Query.h"
#include"../VersionComponent/Version.h"

using Key = NoSqlDb::Key;
using Keys = NoSqlDb::Keys;
///////////////////////////////////////////////////////////////////
// Checkin Class 
//- manages adding new files to Repository 
template<typename T>
class Checkin
{
public:
	std::string getCheckinName(std::string headInfo, std::string file);
	int doCheckin(const MetaData<T>& md, NoSqlDb::DbCore<T>& db);
	void closeVersion(const Key& key, NoSqlDb::DbCore<T>& db);
	bool checkDependenciesStatus(const Key& key, NoSqlDb::DbCore<T>& db);
	bool checkDependenciesExist(const Keys& keys, NoSqlDb::DbCore<T>& db);
	bool checkOwnership(const std::string& checkinAuthor, const std::string& OriginalAuthor);
private:
	Version<T> version_;
};

//-------------<Execute the check-in command by adding information to database and copying file to the repo storage dir>----------
template<typename T>
int Checkin<T>::doCheckin(const MetaData<T>& md, NoSqlDb::DbCore<T>& db) {
	if (!checkDependenciesExist(md.dependencies(), db)) 
		return 0;
	std::string newKey;
	NoSqlDb::DbElement<T> ele;
	ele.name() = md.head()+"::"+ md.name(); 
	ele.descrip() = md.description();
	ele.children() = md.dependencies();
	ele.dateTime() = md.dateTime();
	ele.payLoad() = md.payLoad();
	int ver = version_.getLatestVersion(ele.name(),db);
	if (ver == 0) {
		newKey = md.head() + "::" + md.name()+".1";
		db[newKey] = ele;
		for (auto category : ele.payLoad().categories())
		  FileSystem::File::copy("../Repository/" + md.name(), ele.payLoad().value() + category+"/"+getCheckinName(md.head(), md.name()) + ".1", false);
		return 1;
	}
	else {
		std::string lastKey = md.head() + "::" + version_.addVersion(md.name(), ver);
			if (db[lastKey].payLoad().getStatus() == db[lastKey].payLoad().open) { 
				if (ele.payLoad().getStatus() == ele.payLoad().closed && checkDependenciesStatus(lastKey,db))
					ele.payLoad().setStatus(ele.payLoad().closed);
				else
				   ele.payLoad().setStatus(ele.payLoad().closing);
				db[lastKey] = ele;
				for (auto category : ele.payLoad().categories())
				FileSystem::File::copy("../Repository/" + md.name(), 
					ele.payLoad().value() + category + "/" + version_.addVersion(getCheckinName(md.head(), md.name()), ver),
					false);
				return ver;
			}
			else {
				newKey = md.head() + "::" + version_.addVersion(md.name(), ver + 1);
				ele.payLoad().setStatus(ele.payLoad().open);
				db[newKey] = ele;
				for (auto category : ele.payLoad().categories()) {
				   FileSystem::File::copy("../Repository/" + md.name(),
					ele.payLoad().value() + category + "/" + version_.addVersion(getCheckinName(md.head(), md.name()), ver + 1),
					false);
				}
				return ver + 1;
			}
	}
	return true;
}

//-----------<Get a valid file name from the check-in metadata>--------------------
template<typename T>
std::string  Checkin<T>::getCheckinName(std::string headInfo, std::string file) {
	std::string checkinName = headInfo + "_" + file;
	return checkinName;
}

//-----------<Check the ownership of the file before check-in>--------------------
template<typename T>
bool Checkin<T>::checkOwnership(const std:: string& checkinAuthor, const std::string& OriginalAuthor) {
	if (checkinAuthor == OriginalAuthor)
		return true;
	else {
		cout << "\n Failed to check in.";
		cout << "\n The ownership of the file is "<< OriginalAuthor;

	     return false;
	}
}

//-----------<Close the check-in status of a file with a specific version>--------------------
template<typename T>
void Checkin<T>::closeVersion(const Key& key, NoSqlDb::DbCore<T>& db) {
	NoSqlDb::PayLoad pl;
	if (checkDependenciesStatus(key, db)) {
		db[key].payLoad().setStatus(pl.closed);
		cout << "\n  Closed " << key;
	}
	else {
		cout << "\n  -----Cannot close "<<key;
		cout << "\n  -----Set the status of " << key << " as closing (closed pending)";
		db[key].payLoad().setStatus(pl.closing);
	}
}

//-----------<Check the status of the file's dependencies before closing the file>--------------------
template<typename T>
bool Checkin<T>::checkDependenciesStatus(const Key& key, NoSqlDb::DbCore<T>& db) {
	bool result = true;
	NoSqlDb::Children children = db[key].children();
	for (auto child : children)
	{
		NoSqlDb::PayLoad pl;
		if (db[child].payLoad().getStatus() == pl.open) {
			result = false;
			cout << "\n  "<<key<<" 's dependency " << child << " is not closed.";
		}
			
	}
	return result;
}

//-----------<Check whether the file's dependencies are in the repo before check-in>--------------------
template<typename T>
bool Checkin<T>::checkDependenciesExist(const Keys& keys, NoSqlDb::DbCore<T>& db) {
	bool result = true;
	for (auto key : keys) {
		if (db.contains(key))
			continue;
		else {
			result = false;
			cout << key << " does not exit.";
		}
	}
	return result;
}