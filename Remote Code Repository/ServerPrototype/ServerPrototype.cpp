/////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages  //
// ver 2.0                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018           //
/////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* ---------------------
*  This file contains functors which will be invoked by the instance of the Server class: 
* - echo        :show message content　
* - getFiles    :functor to reply to the getFile command　
* - getDirs     :functor to reply to getDir command 
* - getMetadata :functor to reply to getMetadata command
* - getKeys     :functor to reply to getKeys command
* - getRoot     :functor to reply to getRoot command
* - checkin     :functor to reply to the checkin command 
* - checkout    :functor to reply to the checkout command 
* - connectEcho :functor to reply to the connect command 
* - viewFile    :functor to reply to the viewFile command 
* - browseFile  :functor to reply to the browsing command
Required Files:
* ---------------
* ServerPrototype.h
*
* Maintenance History:
* --------------------
*   ver 1.0 : 30 Mar 2018
* - first release
*   ver 2.0 : 25 Apr 2018
* - add new functors by Jiacheng Zhang
**/
#include "ServerPrototype.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include "../RepoCore/Repository/Metadata/MetaData.h"
#include <chrono>
#include <string>  
#include <fstream>  
#include <sstream>  
#include <iostream>  
#include <stdlib.h>  
namespace MsgPassComm = MsgPassingCommunication;
using namespace std;
using namespace NoSqlDb;
using namespace RepositoryServer;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

//-----------< get the file list in a specific dir>-----------------------
Files Server::getFiles(const RepositoryServer::SearchPath& path)
{
  return Directory::getFiles(path);
}

//-----------< get the dir list in a specific path>-----------------------
Dirs Server::getDirs(const RepositoryServer::SearchPath& path)
{
  return Directory::getDirectories(path);
}

//-----------< show message content>-----------------------
template<typename T>
void show(const T& t, const std::string& msg)
{
  std::cout << "\n  " << msg.c_str();
  for (auto item : t)
  {
    std::cout << "\n    " << item.c_str();
  }
}

//-----------< show message content>-----------------------
std::function<Msg(Msg)> echo = [](Msg msg) {
  Msg reply = msg;
  reply.to(msg.from());
  reply.from(msg.to());
  return reply;
};

//-----------< functor to reply to the getFile command>-----------------------
std::function<Msg(Msg)> getFiles = [](Msg msg) {
  std::cout<<"\n*******************getFile message********************";
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getFiles");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
    Files files = Server::getFiles(searchPath);
    size_t count = 0;
    for (auto item : files)
    {
      std::string countStr = Utilities::Converter<size_t>::toString(++count);
      reply.attribute("file" + countStr, item);
    }
  }
  else
  {
    std::cout << "\n  getFiles message did not define a path attribute";
  }
  return reply;
};

//-----------<  functor to reply to getDir command>-----------------------
std::function<Msg(Msg)> getDirs = [](Msg msg) {
  std::cout << "\n*******************getDirs message********************";
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getDirs");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
	if (path != storageRoot)  
      searchPath = searchPath + "\\" + path;
    Files dirs = Server::getDirs(path);
    size_t count = 0;
    for (auto item : dirs)
    {
      if (item != ".." && item != ".")
      {
        std::string countStr = Utilities::Converter<size_t>::toString(++count);
        reply.attribute("dir" + countStr, item);
      }
    }
  }
  else
  {
    std::cout << "\n  getDirs message did not define a path attribute";
  }
  return reply;
};

//-----------< functor to reply to the checkin command>-----------------------
std::function<Msg(Msg)> checkin = [](Msg msg) {
	std::cout << "\n*******************checkin message********************";
	std::cout << "\n*The client uploads the file to the repository server by a message-passing communication system***";
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("checkin");
	return reply;
};

//-----------<functor to reply to the checkout command>-----------------------
std::function<Msg(Msg)> checkout = [](Msg msg) {
	std::cout << "\n*******************checkout message********************";
	std::cout << "\n*The client downloads several files from the repository server by a message-passing communication system***";

	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("checkout");
	return reply;
};

//-----------< functor to reply to the connect command>-----------------------
std::function<Msg(Msg)> connectEcho = [](Msg msg) {
	std::cout << "\n***start a message-passing communication system based on Sockets***";
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("connected");
	std::cout << "\n***passing HTTP style messages using asynchronous one - way messaging***";
	return reply;
};

//-----------< functor to reply to the viewFile command>-----------------------
std::function<Msg(Msg)> viewFile = [](Msg msg) {
	std::cout << "\n*******************viewFile message********************";
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("viewFile");
	reply.attribute("fileName", msg.attributes()["fileName"]);
	reply.attribute("destination", "../../../ViewFileBuffer");
	reply.attribute("source", msg.attributes()["viewFile"]);
	reply.file(msg.attributes()["fileName"]);
	reply.attribute("toFile", msg.attributes()["fileName"]);
	return reply;
};

//-----------< functor to reply to the getMetadata command>-----------------------
std::function<Msg(Msg)> getMetadata = [](Msg msg) {
	std::cout << "\n*******************getMetadata message********************";
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getMetadata");
	return reply;
};

//-----------< functor to reply to the getRoot command>-----------------------
std::function<Msg(Msg)> getRoot = [](Msg msg) {
	std::cout << "\n*******************getRoot message********************";
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getRoot");
	return reply;
};

//-----------< functor to reply to the browsing command>-----------------------
std::function<Msg(Msg)> browseFile = [](Msg msg) {
	std::cout << "\n*******************Browsing message********************";
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("BrowsingFiles");
	return reply;
};

//-----------< functor to reply to the getKeys command>-----------------------
std::function<Msg(Msg)> getKeys = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getKeys");
	return reply;
};

int main()
{
  std::cout << "\n Remote Repository Server";
  std::cout << "\n ==========================";
  std::cout << "\n";
  Server server(serverEndPoint, "ServerPrototype","../Test.xml");
  server.start();
  server.addMsgProc("echo", echo);
  server.addMsgProc("getFiles", getFiles);
  server.addMsgProc("getDirs", getDirs);
  server.addMsgProc("serverQuit", echo);
  server.addMsgProc("checkin",checkin);
  server.addMsgProc("checkout", checkout);
  server.addMsgProc("connect", connectEcho);
  server.addMsgProc("viewFile", viewFile);
  server.addMsgProc("browse", browseFile);
  server.addMsgProc("getKeys", getKeys);
  server.addMsgProc("getMetadata", getMetadata);
  server.addMsgProc("getRoot", getRoot);
  server.processMessages();
  std::cin.get();
  return 0;
}

