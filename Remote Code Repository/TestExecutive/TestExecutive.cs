/////////////////////////////////////////////////////////////////////////////
// TestExecutive.cs - Executive for Project3                               //
// ver 1.0                                                                 //
// Author: Jiacheng Zhang                                                  // 
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* ---------------------
*  Package contains one class, TestExecutive, that  is used to invoke multiple clients
*  TestExecutive uses define the port number of differnt client and uses C# Process class
*  to pass these numbers by command line and invoke multiple client.
* Maintenance History:
* ----------------------
*  ver 1.0 : 4/10/2018
*/
using System;
using System.Diagnostics;

namespace TestExecutive
{
    class TestExecutive
    {
        static void Main(string[] args)
        {
            String client = "..\\..\\..\\GUI\\bin\\x86\\Debug\\GUI.exe";
            int port1 = 8082;
            int port2 = 8083;
           Process.Start(client, port1.ToString());
          Process.Start(client,port2.ToString());

        }
    }
}
