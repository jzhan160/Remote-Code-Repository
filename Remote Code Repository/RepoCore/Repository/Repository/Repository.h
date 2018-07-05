#pragma once
//////////////////////////////////////////////////////////////////////////////
// Repository.h - provides means to check-in, version, browse, and check-out//
// Ver 2.0                                                                  //
// Jiacheng Zhang, CSE687 - Object Oriented Design, Spring 2018             //
//////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains a single class Repository with public functions:
* - initDb()          : build a db from an existing xml
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
*  5 Apr 2018
* - ver 2.0
*
*/
#include "../Db/DateTime.h"
#include"../Db/DbCore.h"
#include"../CheckinComponent/Checkin.h"
#include"../CheckoutComponent/Checkout.h"
#include "../Db/XmlParser.h"
#include "../Db/XmlDocument.h"
#include "../Db/xmlElementParts.h"
#include "../Db/XmlElement.h"
#include "../Db/Tokenizer.h"
#include "../Db/Persist.h"
#include "../Metadata/MetaData.h"
#include "../Db/PayLoad.h"
#include "../Browse/Browse.h"
#include "../Utilities/Graph.h"
#include <string>

using Key = NoSqlDb::Key;
using Keys = NoSqlDb::Keys;
using Xml = std::string;
//////////////////////////////////////////////////////////////
//Repository Class
//- provides means to check-in, version, browse, and check-out

template<typename T>
class Repository {
public:
	Repository(Xml src = "");
	void checkin(const MetaData<T>& md);
	Keys checkout(const Key& key);
	void closeVersion(const Key& key);
	void dependencyGraph(const Key& parent, const NoSqlDb::Children& children);
	void showGraph();
	NoSqlDb::DbCore<T>& getDb() const { return db_; }
	Keys browse(const std::string& content, const std::string& by);
	void initDb(Xml src = "");
	void initGraph();
	Keys getRoot(const std::string& category);
private:
	std::string storage = "../Storage/";
	static NoSqlDb::DbCore<T> db_;
	static GraphProcessing::Graph<Key> graph_;
	Checkin<T> checkin_;
	Checkout<T> checkout_;
	Browse<T> browse_;
	Version<T> version_;
};

template<typename T>
NoSqlDb::DbCore<T> Repository<T>::db_;

template<typename T>
GraphProcessing::Graph<Key> Repository<T>::graph_;

//---------<constructor>-----------
template<typename T>
Repository<T>::Repository(Xml src) {
	initDb(src);
	initGraph();
 }

//---------<build a db from existing xml file>-----------
template<typename T>
void Repository<T>::initDb(Xml src) {
	if (src == "")
		return;
	else {
		XmlProcessing::XmlParser parser(src);
		parser.verbose();
		XmlProcessing::XmlDocument* pDoc = parser.buildDocument();
		using Sptr = std::shared_ptr<AbstractXmlElement>;
		std::string Xml = pDoc->toString();
		Repository<PayLoad> repo;
		Persist<PayLoad> persist(repo.getDb());
		persist.fromXml(Xml);
		//std::cout << "\n  Show the database built from " << src;
		NoSqlDb::showDb(repo.getDb());
		std::cout << "\n";
	}
}

template<typename T>
void Repository<T>::initGraph() {
	for (Key key : db_.keys()) {
		Children children = db_[key].children();
		dependencyGraph(key, children);
	}
}

//---------<checkin files with metadata>----------------------------
template<typename T>
void Repository<T>::checkin(const MetaData<T>& md) {
	int ver = checkin_.doCheckin(md, db_);
	if (ver == 0)
		return;
	std::string head = md.head();
	std::string name = md.name();
	Key parent = head + "::" + name + "." + std::to_string(ver);
	NoSqlDb::Children children = md.dependencies();
	dependencyGraph(parent, children);
}

//---------< checkout file of a specific version>----------------------------
template<typename T>
Keys Repository<T>::checkout(const Key& key) {
	checkout_.doCheckout(key,db_, graph_);
	return checkout_.getCheckoutKeys();
}

//---------<close check-in status of a specific file>----------------------------
template<typename T>
void Repository<T>::closeVersion(const Key& key) {
	checkin_.closeVersion(key, db_);
}

//---------<browse files and dependencies>----------------------------
template<typename T>
Keys Repository<T>::browse(const std::string& content,const std::string& by) {
		return browse_.doBrowse(db_, content, by);
}

template<typename T>
Keys Repository<T>::getRoot(const std::string& category) {
	Keys keys = browse_.doBrowse(db_, category, "Category");
	Keys roots;
	for (auto key : keys)
	{ 
		std::string fileName = version_.getFileName(key);
		Keys parents = browse_.doBrowse(db_, fileName, "Dependency");
		if (parents.size() == 0)
		   roots.push_back(fileName);
	}
	return roots;
}

//---------<build a file graph using all key in repository>----------------------------
template<typename T>
void  Repository<T>::dependencyGraph(const Key& parent, const NoSqlDb::Children& children) {
	bool parentExist = true;
	if (!graph_.containsNode(parent)) {
		bool parentExist = false;
		graph_.addNode(GraphProcessing::Sptr<Key>(new GraphProcessing::Node<Key>(parent)));
	}
	for (auto child : children) {
		if (!graph_.containsNode(child))
			graph_.addNode(GraphProcessing::Sptr<Key>(new GraphProcessing::Node<Key>(child)));
	    graph_.addEdge(parent, child);
	}
}

//---------<show the file graph>----------------------------
template<typename T>
void  Repository<T>::showGraph() {
	GraphProcessing::showGraph<Key>(graph_);
}
 
