#pragma once
///////////////////////////////////////////////////////////////////////////
// CheckinComponent.h - supports extraction of file dependency graphs    //
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
#include "../Metadata/MetaData.h"
#include "../Db/DbCore.h"
#include "../Db/Query.h"
#include "../VersionComponent/Version.h"
#include "../Utilities/Graph.h"

using Key = NoSqlDb::Key;
using Keys = NoSqlDb::Keys;

///////////////////////////////////////////////////////////////////
// Checkout Class 
//- supports extraction of file dependency graphs
template<typename T>
class Checkout {
public:
  void doCheckout(const Key& key, NoSqlDb::DbCore<T>& db, GraphProcessing::Graph<Key>& graph);
  Keys getCheckoutKeys() { return keys; }
private:
	Version<T> version;
	Keys keys;
	std::string storage = "../Storage/";
	std::string staging = "../Staging/Checkout/";
};

//--------------------<find a specific file and all of its dependenciy files, and copy them to a specific location>---------
template<typename T>
void Checkout<T>::doCheckout(const Key& key, NoSqlDb::DbCore<T>& db, GraphProcessing::Graph<Key>& graph) {
	if (!db.contains(key)) {
		std::cout << "\n  Cannot find the file in the repository. \n";
		return;
	}
	if (!(db[key].payLoad().getStatus() == db[key].payLoad().open)) {
		std::cout << "\n  Walk through the dependency tree: ";
		keys = getDependency(graph,key); // all keys about checkout file and its dependencies
	}
	else
	{
		std::cout << "\n  Cannot check out unclosed file. \n";
	}
}

//--------------------------<use graph walk to get all of dependencies>-------------------------------------------------------
inline std::vector<std::string> getDependency(GraphProcessing::Graph<Key>& graph, const Key& key) {
	std::vector<Key> trace;
	auto proc = [](GraphProcessing::Sptr<Key> pNode) { std::cout << " --> " << pNode->name(); };
	using P = decltype(proc);
	using V = Key;
	GraphProcessing::Sptr<Key> pNode = graph.find(key);
	graph.walker(DFS_r_ltr<V, P>, pNode, proc, trace);
	std::cout << "\n  Retrieved Files: \n";
	for (auto node : trace)
	{
		std::cout << "  "<<node << "\n";
	}
	return trace;
}