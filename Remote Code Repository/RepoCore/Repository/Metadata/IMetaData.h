#pragma once
/////////////////////////////////////////////////////////////////////////
// IMetaData.h - declare language for serializing a MetaData instance  //
// ver 1.0                                                             //
// Jiacheng Zhang, CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////////
/*
IMetaData is an interface for the MetaData class.
It contains a pure virtualfunction show() which will show properties,
and a destructor IMetaData()to release memory space.
*/
template<typename T>
struct IMetaData
{
	virtual void show() = 0;
	virtual ~IMetaData() {};
};