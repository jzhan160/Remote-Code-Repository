#pragma once
///////////////////////////////////////////////////////////////////////////
// MetaData.h - in-memory representation of Repository MetaData file    //
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
*  DateTime.h 
*  Maintenance History:
* --------------------
*  2 Mar 2018
* - ver 1.0
*
*/
#include <string>
#include <vector>
#include "../Db/DateTime.h"
#include "IMetaData.h"
using Dependencies = std::vector<std::string>;

///////////////////////////////////////////////////////////////////
// MetaData Class 
//- in-memory representation of Repository MetaData file
template<typename T>
class MetaData : public IMetaData<T>
{
public:
	std::string& head() { return head_; }
	std::string head() const { return head_; }
	void head(const std::string& head) { head_ = head; }

	std::string& name() { return name_; }
	std::string name() const { return name_; }
	void name(const std::string& name) { name_ = name; }

	std::string& path() { return path_; }
	std::string path() const { return path_; }
	void path(const std::string& path) { path_ = path; }

	//std::string& author() { return author_; }
	//std::string author() const { return author_; }
	//void author(const std::string& author) { author_ = author; }

	std::string& description() { return description_; }
	std::string description() const { return description_; }
	void description(const std::string& description) { description_ = description; }

	DateTime& dateTime() { return dateTime_; }
	DateTime dateTime() const { return dateTime_; }
	void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

	Dependencies& dependencies() { return deps_; }
	Dependencies dependencies() const { return deps_; }
	void dependencies(const Dependencies& deps) { deps_ = deps; }

	T& payLoad() { return payLoad_; }
	T payLoad() const { return payLoad_; }
	void payLoad(const T& payLoad) { payLoad_ = payLoad; }

	void show();
private:
	std::string head_;
	std::string name_;
	std::string author_;
	std::string description_;
	DateTime dateTime_;
	std::string path_;
	Dependencies deps_;
	T payLoad_;
};

template<typename T>
/*----< display >----------------------------------------------*/
void MetaData<T>::show()
{
	cout << "\n  Repository File MetaData:";
	cout << "\n    name:               " << name_;
	cout << "\n    author:             " << author_;
	cout << "\n    DateTime:           " << std::string(dateTime_);
	cout << "\n    Description:        " << description_;
	cout << "\n    File Dependencies:";
	for (string dep : deps_)
	{
		cout << "\n      " << dep;
	}

}