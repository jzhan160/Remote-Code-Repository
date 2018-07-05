#pragma once
///////////////////////////////////////////////////////////////////////
// ServerPrototype.h - Console App that processes incoming messages  //
// ver 3.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //  
// Updated by Jiacheng Zhang                                         //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* ---------------------
*  Package contains one class, Server, that contains a Message-Passing Communication
*  facility. It processes each message by invoking an installed callable object
*  defined by the message's command key.
*
*  Message handling runs on a child thread, so the Server main thread is free to do
*  any necessary background processing (none, so far).
*
*  Public functions in the Server class:
* - Server()           : initialize server endpoint and give server a name　
* - start()            : start server's instance of Comm
* - stop()             : stop Comm instance
* - postMessage()      : pass message to Comm for sending
* - getMessage()       : get message from Comm 
* - addMsgProc()       : add ServerProc callable object to server's dispatcher  
* - processMessages()  : start processing messages on child thread 　
* - checkin()          : checkin files using repo
* - checkout()         : checkout files using repo
* - getMetadata()      : get metadata of a specific key in the repo db
* - getBrowsingFiles() : get browsing files by querying descriptions in the repo db  
* - getKeys()          : get all keys in the repo db
* - split()            : split strings by spaces
* - getRoots()         : get all root files of a specific category in the repo db

*  Required Files:
* -----------------
*  ServerPrototype.h, ServerPrototype.cpp
*  Comm.h, Comm.cpp, IComm.h
*  Message.h, Message.cpp
*  FileSystem.h, FileSystem.cpp
*  Utilities.h
*
*  Maintenance History:
* ----------------------
*  ver 1.0 : 3/27/2018
*  - first release
*  ver 2.0 : 4/8/2018
*  - supplemented with NoSqlDb
*  ver 3.0 : 26 Apr 2018
* - add new functions by Jiacheng Zhang
*/
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <thread>
#include "../CppCommWithFileXfer/Message/Message.h"
#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include<iostream>
#include<sstream>
#include "../RepoCore/Repository.h"
#include <windows.h>
#include <tchar.h>

namespace RepositoryServer
{
  using File = std::string;
  using Files = std::vector<File>;
  using Dir = std::string;
  using Dirs = std::vector<Dir>;
  using Xml = std::string;
  using SearchPath = std::string;
  using Key = std::string;
  using Keys = std::vector<string>;
  using Msg = MsgPassingCommunication::Message;
  using ServerProc = std::function<Msg(Msg)>;
  using MsgDispatcher = std::unordered_map<Key,ServerProc>;
  
  const SearchPath storageRoot = "../Repository";  // root for all server file storage
  const MsgPassingCommunication::EndPoint serverEndPoint("localhost", 8080);  // listening endpoint

  class Server
  {
  public:
    Server(MsgPassingCommunication::EndPoint ep, const std::string& name, const Xml& src = "");
    void start();
    void stop();
    void addMsgProc(Key key, ServerProc proc);
    void processMessages();
    void postMessage(MsgPassingCommunication::Message msg);
    MsgPassingCommunication::Message getMessage();
	void checkin(Msg& msg);
	void  checkout(Msg& msg,Msg& reply);
	Msg getMetadata(Msg& msg,std::string fileName);
	Msg getBrowsingFiles(Msg& msg, Msg& reply);
	Msg getKeys(Msg& reply);
	Msg getRoot(Msg& msg, Msg& reply);
	std::vector<std::string> split(std::string childrenStr);
    static Dirs getDirs(const SearchPath& path = storageRoot);
    static Files getFiles(const SearchPath& path = storageRoot);

  private:
    MsgPassingCommunication::Comm comm_;
    MsgDispatcher dispatcher_;
    std::thread msgProcThrd_;
	Repository<NoSqlDb::PayLoad> repo;
  };
 
  //----< initialize server endpoint and give server a name >----------
  inline Server::Server(MsgPassingCommunication::EndPoint ep, const std::string& name, const Xml& src)
    : comm_(ep, name),repo(src) {}

  //----< start server's instance of Comm >----------------------------
  inline void Server::start()
  {
    comm_.start();
  }
  
  //----< stop Comm instance >-----------------------------------------
  inline void Server::stop()
  {
    if(msgProcThrd_.joinable())
      msgProcThrd_.join();
    comm_.stop();
  }

  //----< pass message to Comm for sending >---------------------------
  inline void Server::postMessage(MsgPassingCommunication::Message msg)
  {
    comm_.postMessage(msg);
  }
  
  //----< get message from Comm >--------------------------------------
  inline MsgPassingCommunication::Message Server::getMessage()
  {
    Msg msg = comm_.getMessage();
    return msg;
  }
  
  //----< add ServerProc callable object to server's dispatcher >------
  inline void Server::addMsgProc(Key key, ServerProc proc)
  {
    dispatcher_[key] = proc;
  }
  
  //----< start processing messages on child thread >------------------
  inline void Server::processMessages()
  {
    auto proc = [&]()
    {
      while (true)
      {
        Msg msg = getMessage();
		Keys keys;
		int fileNumber = 0;
        std::cout << "\n  received message: " << msg.command() << " from " << msg.from().toString();
		msg.show();
        Msg reply = dispatcher_[msg.command()](msg);
		if (msg.command() == "checkin") 
			 checkin(msg);
		if (msg.command() == "checkout")
		{
			 checkout(msg,reply);
			 continue;
		}
		if (msg.command() == "viewFile" || msg.command() == "getMetadata") 
			getMetadata(reply, msg.value("fileName"));
		if (msg.command() == "browse")
			getBrowsingFiles(msg, reply);
		if (msg.command() == "getKeys")
			getKeys(reply);
		if (msg.command() == "getRoot")
			getRoot(msg,reply);
		if (msg.to().port != msg.from().port)  // avoid infinite message loop
        {   
                postMessage(reply);
		        std::cout << "\n  reply message:";
                reply.show();
        }
		else
          std::cout << "\n  server attempting to post to self";
      }
      std::cout << "\n  server message processing thread is shutting down";
    };
    std::thread t(proc);
    std::cout << "\n  waiting for connections from remote clients";
    msgProcThrd_ = std::move(t);
  }
 
  //----< check in files using repository >------------------
  inline void Server::checkin(Msg& msg) {
	  MetaData<NoSqlDb::PayLoad> md;
	  md.name() = msg.value("name");
	  md.head() = msg.value("namespace");
	  md.description() = msg.value("description");
	  DateTime dt;
	  md.dateTime() = dt;
	  NoSqlDb::PayLoad pl;
	  pl.value() = msg.value("destination");
	  pl.categories() = split(msg.value("category"));
	  pl.setStatus(pl.open);
	  md.payLoad() = pl;
	  md.dependencies() = split(msg.value("children"));
	  repo.checkin(md);
  }
  
  //----< check out files using repository >------------------
  inline void Server::checkout(Msg& msg,Msg& reply) {
	  std::string fileName = msg.value("requestfile");
	  auto pos = fileName.find ("_");
	  Key key = fileName.substr(0, pos) + "::" + fileName.substr(pos + 1); //parent
	  Keys keys = repo.checkout(key); //parent+children
	  for (Key key : keys)
	  {
		  Version<NoSqlDb::PayLoad> ver;
		  reply.attribute("file", ver.getFileName(key));
		  reply.attribute("toFile", ver.removeVersion(ver.removeHead(ver.getFileName(key))));
		  reply.attribute("destination", msg.value("destination"));
		  reply.attribute("source", storageRoot + "/" + ver.getHead(key));
		  postMessage(reply);
		  std::cout << "\n  reply message:";
		  reply.show();
	  }

  }
  
  //----< get metadata of a specific key in the repo db >------------------
  inline Msg Server::getMetadata(Msg& msg, std::string fileName) {
	  int pos = fileName.find("_");
	  Key key = fileName.substr(0, pos) + "::" + fileName.substr(pos+1);
	  NoSqlDb::DbElement<NoSqlDb::PayLoad> elem = repo.getDb()[key];
	  msg.attribute("name", elem.name());
	  msg.attribute("dateTime", elem.dateTime().operator std::string());
	  msg.attribute("description", elem.descrip());
	  int count = 1;
	  for (auto child : elem.children()) {
	   msg.attribute("child"+ to_string(count++), child);
	  }
	  msg.attribute("payLoadPath", elem.payLoad().value());
	  count = 1;
	  for (auto category : elem.payLoad().categories()) {
	    msg.attribute("category" + to_string(count++),category);
	  }
	  msg.attribute("status", to_string(elem.payLoad().getStatus()));
	  return msg;
  }

  //----< get browsing files by querying descriptions in the repo db >------------------
  inline Msg Server::getBrowsingFiles(Msg& msg, Msg& reply) {
	  std::string condition = msg.value("queryCondition");
	  std::string content = msg.value("queryContent");
	  Keys keys = repo.browse(content, condition);
	  std::string files;
	  Version<NoSqlDb::PayLoad> version;
	  for (auto key : keys)
	  {
		  files += version.getFileName(key) + " ";
	  }
	  reply.attribute("BrowsingFiles",files);
	  return reply;
  }

  //----< get all keys in the repo db >------------------
  inline Msg Server::getKeys(Msg& reply) {
	  Keys keys = repo.getDb().keys();
	  std::string keyStr = "";
	  for (auto key : keys)
	  {
		  keyStr+= key + " ";
	  }
	  reply.attribute("keys",keyStr);
	  return reply;
  }

  //----<split strings by spaces>------------------
  inline std::vector<std::string> Server::split(std::string childrenStr) {
	  vector<string> children;
	  string result;
	  stringstream input(childrenStr);
	  while (input >> result)
		  children.push_back(result);
	  return children;
  }

  //----<get all root files of a specific category>------------------
  inline Msg Server::getRoot(Msg& msg, Msg& reply) {
	  std::string category = msg.value("category");
	  Keys files = repo.getRoot(category);
	  std::string roots = "";
	  for (auto file : files)
	  {
		  roots += file + " ";
	  }
	  reply.attribute("roots", roots);
	  return reply;
  }
}