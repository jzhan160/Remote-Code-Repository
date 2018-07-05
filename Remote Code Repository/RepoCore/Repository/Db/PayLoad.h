#ifndef PAYLOAD_H
#define PAYLOAD_H
///////////////////////////////////////////////////////////////////////
// PayLoad.h - application defined payload                           //
// ver 1.1                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a single class, PayLoad:
*  - holds a payload string and vector of categories
*  - provides means to set and access those values
*  - provides methods used by Persist<PayLoad>:
*    - Sptr toXmlElement();
*    - static PayLoad fromXmlElement(Sptr elem);
*  - provides a show function to display PayLoad specific information
*  - PayLoad processing is very simple, so this package contains only
*    a header file, making it easy to use in other packages, e.g.,
*    just include the PayLoad.h header.
*
*  Required Files:
*  ---------------
*    PayLoad.h, PayLoad.cpp - application defined package
*    DbCore.h, DbCore.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 19 Feb 2018
*  - added inheritance from IPayLoad interface
*  Ver 1.0 : 10 Feb 2018
*  - first release
*
*/

#include <string>
#include <vector>
#include <iostream>
#include "XmlDocument.h"
#include "XmlElement.h"
#include "Definitions.h"
#include "DbCore.h"
#include "IPayLoad.h"

///////////////////////////////////////////////////////////////////////
// PayLoad class provides:
// - a std::string value which, in Project #2, will hold a file path
// - a vector of string categories, which for Project #2, will be 
//   Repository categories
// - methods used by Persist<PayLoad>:
//   - Sptr toXmlElement();
//   - static PayLoad fromXmlElement(Sptr elem);

using namespace XmlProcessing;
using namespace std;
namespace NoSqlDb
{

  class PayLoad : public IPayLoad<PayLoad>
  {
  public:
    enum status{
     open,
     closed,
     closing
    };
    PayLoad() = default;
    PayLoad(const std::string& val) : value_(val) {}
    static void identify(std::ostream& out = std::cout);
    PayLoad& operator=(const std::string& val)
    {
      value_ = val;
      return *this;
    }
    operator std::string() { return value_; }

    status getStatus() const { return status_; }
    void setStatus(const status& s) { status_ = s; }

    std::vector<std::string>& categories() { return categories_; }
    std::vector<std::string> categories() const { return categories_; }

	std::string value() const { return value_; }
	std::string& value() { return value_; }
	void value(const std::string& value) { value_ = value; }

    bool hasCategory(const std::string& cat)
    {
      return std::find(categories().begin(), categories().end(), cat) != categories().end();
    }

    Sptr toXmlElement();
    static PayLoad fromXmlElement(Sptr elem);

    static void showElementPayLoad(const NoSqlDb::DbElement<PayLoad>& elem, std::ostream& out = std::cout);
  private:
    std::string value_;
    std::vector<std::string> categories_;
	status status_  = open;
  };

  //----< show file name >---------------------------------------------

  inline void PayLoad::identify(std::ostream& out)
  {
    out << "\n  \"" << __FILE__ << "\"";
  }
  //----< create XmlElement that represents PayLoad instance >---------
  /*
  * - Required by Persist<PayLoad>
  */
  inline Sptr PayLoad::toXmlElement()
  {
    Sptr sPtr = XmlProcessing::makeTaggedElement("payload");
    XmlProcessing::XmlDocument doc(makeDocElement(sPtr));
    Sptr sPtrVal = XmlProcessing::makeTaggedElement("value",value_);
    sPtr->addChild(sPtrVal);
    Sptr sPtrCats = XmlProcessing::makeTaggedElement("categories");
    sPtr->addChild(sPtrCats);
    for (auto cat : categories_)
    {
      Sptr sPtrCat = XmlProcessing::makeTaggedElement("category", cat);
      sPtrCats->addChild(sPtrCat);
    }
    return sPtr;
  }
  //----< create PayLoad instance from XmlElement >--------------------
  /*
  * - Required by Persist<PayLoad>
  */
  inline PayLoad PayLoad::fromXmlElement(Sptr pElem)
  {
    PayLoad pl;
    for (auto pChild : pElem->children())
    {
      std::string tag = pChild->tag();
      std::string val = pChild->children()[0]->value();
	  status status;
      if (tag == "path")
      {
        pl.value(val);
      }
	  if (tag == "status") {
		  if (val == "Open")
			  status = pl.open;
		  else if (val == "Closed") 
			  status = pl.closed;
		  else if (val == "Closing") 
			  status = pl.closing;
		  pl.setStatus(status);
	  }
      if (tag == "categories")
      {
        std::vector<Sptr> pCategories = pChild->children();
        for (auto pCat : pCategories)
        {
          pl.categories().push_back(pCat->children()[0]->value());
        }
      }
    }
    return pl;
  }
  /////////////////////////////////////////////////////////////////////
  // PayLoad display functions


  inline void PayLoad::showElementPayLoad(const NoSqlDb::DbElement<PayLoad>& elem, std::ostream& out)
  {
    out << "\n  PayLoad: ";
	out <<"<path>" << elem.payLoad().value() << "</path>"<<"\n";
	if (elem.payLoad().getStatus() == elem.payLoad().open)
		out << "           " << "<status>"<<"open"<< "</status>" << "\n";
	else
		out  <<"            " << "<status>" << "closed" << "</status>" << "\n";
	vector<string> categories = elem.payLoad().categories();
    for (auto cat : categories)
    {
      out << "            " << "<category>"<< cat << "</category> "<<"\n";
    }
  }
}
#endif
