#pragma once
///////////////////////////////////////////////////////////////////////////
// Browse.h - supports traversal of file dependency graphs               //
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
#include "../Db/DbCore.h"
#include "../Db/Query.h"
#include "../VersionComponent/Version.h"
#include "process.h"
#include <vector>
#include <string>
using Key = NoSqlDb::Key;
using Dependencies = std::vector<std::string>;


///////////////////////////////////////////////////////////////////////
// Browse class
// - supports traversal of file dependency graphs
template <typename T>
class Browse {
public:
	Keys doBrowse(NoSqlDb::DbCore<T>& db,const std::string& content, const std::string& by);
private:
	Version<T> version;
};

//----< Browse a given version of file and its dependencies to show their metadata. >---------------------------
template <typename T>
Keys Browse<T>::doBrowse(NoSqlDb::DbCore<T>& db, const std::string& content, const std::string& by){
	NoSqlDb::Query<T> q1(db);
	Keys saveKeys = q1.keys();
	q1.from(saveKeys);
	NoSqlDb::Conditions<T> conds1;
	if (by == "FileName")
		conds1.name(content);
	else if (by == "Category") {
		auto hasCategory = [&content](NoSqlDb::DbElement<T>& elem) {
			return (elem.payLoad()).hasCategory(content);
		};
		std::cout << "\n  show:";
		return q1.select(hasCategory).keys();
	}
	else if (by == "Dependency") {
		Keys keys;
		keys.push_back(version.getKeyName(content));
		conds1.children(keys);
	}
	else if (by == "Version") {
		Keys keys;
		for (auto key : db.keys()) {
			if (version.getVersion(key) == atoi(content.c_str())) {
				std::cout << "\n " << key;
				keys.push_back(key);
			}
		}
		return keys;
	}
	return q1.select(conds1).keys();
}

