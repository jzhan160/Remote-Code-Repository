/////////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - GUI for Project3                                   //
// ver 3.0                                                                 //
// Source Code: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018  // 
// Updated By: Jiacheng Zhang                                              // 
/////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based GUI .  It's responsibilities are to:
 * - Provide a display of directory contents of a remote ServerPrototype and local staging.
 * - It provides a subdirectory list and a filelist for the selected directory.
 * - You can navigate into subdirectories by double-clicking on subdirectory
 *   or the parent directory, indicated by the name "..".
 * - You can check-in and check-out files and browse files by different query condition
 * This package contains a single class MainWindow with private functions:
 * - Window_Loaded()               : fill window display with local dirs and files and invok tests     
 * - Test1()　　　　　　　         : automated unit test for connection 
 * - Test2()　　　　　　　　　　　 : automated unit test for checkin 
 * - Test3()                       : automated unit test for checkout 
 * - Test4()                       : automated unit test for popping up file contents and showing metadata 
 * - Test5()                       : automated unit test for browsing
 * - Test6()                       : automated unit test for browsing
 * - PrepareForCheckin()           : select the check_in file for unit test
 * - PrepareForCheckout()          : select the check_out file for unit test
 * - Window_Closed()               : delete popup file buffer when the windows closes 
 * - processMessages()             : process incoming messages on child thread
 * - clearDirs()                   : clear dirs in the RepoDirListBox
 * - addDir()                      : add dirs into the RepoDirListBox
 * - addClientProc()               ; add client processing for message with key  
 * - DispatcherLoadConnection()    : load connection success processing into dispatcher dictionary  
 * - DispatcherLoadGetDirs()       : load getDirs processing into dispatcher dictionary   
 * - DispatcherLoadGetFiles()      : load getFiles processing into dispatcher dictionary  
 * - DispatcherLoadCheckout()      : load checkout processing  
 * - DispatcherLoadViewFile()      : load view remote files processing into dispatcher dictionary 
 * - DispatcherLoadMetadata()      : load show metadata processing into dispatcher dictionary
 * - DispatcherLoadShowKeys()      : load show keys processing into dispatcher dictionary
 * - Dispatcher LoadRoots()        : load  show root file of a specific category processing into dispatcher dictionary
 * - DispatcherLoadStatusBar()     : load status bar processing into dispatcher dictionary  
 * - InitLocalStorageView()        : show files and dirs in the local storage 
 * - ShowRepo()                    : show files and dirs in the repository 
 * - GetLocalDirs()                : get dirs in the local staging directory 
 * - GetLocalFiles()               : get files in the local staging directory 
 * - loadDispatcher()              : load all dispatcher processing  
 * - DeleteFiles()                 : Remove file buffer which is used to stored poping up files 
 * - removeFirstDir()              : strip off name of first part of path  
 * - DirList_MouseDoubleClick()    : respond to mouse double-click on local dir name 
 * - RepoDirList_MouseDoubleClick(): respond to mouse double-click on remote dir name 
 * - Parent_Click()                : respond to mouse right-click to add a parent check_in file  
 * - Child_Click()                 : respond to mouse right-click to add children of a parent check_in file  
 * - Reset_Click()                 : respond to mouse right-click to remove selected check_in files  
 * - Checkout_Select_Clic()        : respond to mouse right-click to add a check_out file  
 * - FileMouseDoubleClick()        : respond to pop up file content  
 * - ShowFileContent()             : show file content in both the popup window and the GUI tab 
 * - ShowMetadata（）              ：show file metadata in the view metadata tab 
 * - ShowMetadata()                : get children file names and category names for the metadata 
 * - GetKeysInRepo()               : send getKeys message to the remote server
 * - CatchMetadata()               : get metadata provided by client users
 * - Checkin_Click()               ; respond to check_in click 
 * - Checkout_Click()              : respond to check_out click 
 * - Repo_Refresh_Button_Click() 　: show current files in the repository to show new check_in files  
 * - Local_Refresh_Button_Click()　: show current files in the local storage to show new check_out files 
 * - Connect_Click()               : trigger connection to start comm and load dispatcher
 * - ReceiveValues()               : get keys from Dependency Editor window
 * - Check()                       : Check whether the user provide all metadata for check_in
 * Required Files:
 * ---------------
 * Mainwindow.xaml, MainWindow.xaml.cs
 * Translater.dll
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 30 Mar 2018
 * - first release
 * - Several early prototypes were discussed in class. Those are all superceded
 *   by this package.
 *   ver 2.0 : 8 Apr 2018
 * - add new functions by Jiacheng Zhang
 *   ver 3.0 : 26 Apr 2018
 * - add new functions by Jiacheng Zhang
 */

using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Threading;
using MsgPassingCommunication;
using System.IO;

namespace GUI
{
    public partial class MainWindow : Window
    {
        private Stack<string> checkinPathStack_ = new Stack<string>();
        private Stack<string> checkoutPathStack_ = new Stack<string>();
        private Dictionary<string, Action<CsMessage>> dispatcher_ = new Dictionary<string, Action<CsMessage>>();
        private Dictionary<string, string> metadata_ = new Dictionary<string, string>();
        private bool isTest = true;
        private int msgCount = 1;
        private bool isConnected = false;
        private bool isRemove = false;
        private bool hasParent = false;
        private Thread rcvThrd = null;
        private string checkoutFile;
        private string checkinFile;
        private string viewFilePath;
        private Translater translater;
        private CsEndPoint endPoint_;
        private CsEndPoint serverEndPoint;
        public MainWindow()
        {
            InitializeComponent();

        }
        //----< fill window display with local dirs and files >------
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            InitLocalStorageView();
            Test1();
            while (!isConnected) ;
            Test2();
            Test3();
            Test4();
            Test5();
            Test6();
            isTest = false;
        }

        //----<automated unit test for connection>------
        private bool Test1()
        {
            Console.WriteLine("**********Connect to the remote repository server*********");
            try
            {
                ConnectButton.RaiseEvent(new RoutedEventArgs(MenuItem.ClickEvent));
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
                return false;
            }
            return true;
        }

        //----<automated unit test for checkin>------
        private bool Test2()
        {
            Console.WriteLine("*******Check in files by editing metadata and uploading files to repo***********");
            try
            {
                PrepareForCheckin();
                Checkin_Button.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
                return false;
            }
            return true;
        }

        //----<automated unit test for Checkout>------
        private bool Test3()
        {
            Console.WriteLine("********Check out files by downloading files from repo********************");
            try
            {
                PrepareForCheckout();
                Checkout_Button.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
                return false;
            }
            return true;
        }

        //----<automated unit test for popping up file contents and showing metadata >------
        private bool Test4()
        {
            Console.WriteLine("********View files by doubly clicking files in the checkout tab*************");
            try
            {
                Console.WriteLine("  Double click the first item in the FileList of Checkout tab. ");
                MouseButtonEventArgs e1 = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left)
                { RoutedEvent = Control.MouseDoubleClickEvent };
                RepoFileList.RaiseEvent(e1);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
                return false;
            }
            Console.WriteLine("  See the popup window showing file contents and metadata");
            return true;
        }

        //------<automated unit test for browsing>------------------------------
        private bool Test5()
        {
            try
            {
                if(endPoint_.port == 8082)
                {
                   Console.WriteLine("**********Display collections of filenames based on queries defining categories********************");
                    tbQueryContent.Text = "NoSqlDb";
                    ConditionComboBox.Text = "Category";
                    showFiles.SelectedIndex = 0;

                }
                else
                {
                    Console.WriteLine("**********Display collections of filenames based on queries defining names********************");
                    tbQueryContent.Text = "Comm";
                    ConditionComboBox.Text = "FileName";
                    showFiles.SelectedIndex = 0;
                }
                Console.WriteLine("  See the file name and metadata in the browse tab");
                Console.WriteLine("  You can also query by version and dependecies selecting items in the ComboBox");
                BrowseButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
                return false;
            }
            return true;
        }

        //------<automated unit test for browsing>------------------------------
        private bool Test6()
        {
            Console.WriteLine("*****Display package categories, based on metadata stored in the NoSql database******");
            Console.WriteLine("  See categories at the bottom of the browse tab");
            CategoriesList.SelectedIndex = 0;
            Console.WriteLine("*****Display all of the files in any category that have no parents******");
            Console.WriteLine("  Click the category name to see the root file for this category.");

            return true;

        }

        //----<select the check_in file for unit test >------
        private void PrepareForCheckin()
        {
            String parent = FileList.Items[0] as String;
            TextBlock tb = new TextBlock();
            tb.Text = parent;
            tb.FontWeight = FontWeights.Bold;
            CheckinList.Items.Add(tb);
            checkinFile = parent;
            hasParent = true;
            Set_Parent.IsEnabled = false;

            NameText.Text = "Comm.h";
            NamespaceText.Text  ="MPC";
            DescriptionText.Text = "new version for Comm.h";
            CategText.Text  = "MPC Project4";
            StatusComboBox.Text = "Open";
            CheckinList.Items.Add("MPC::IComm.h.1");
        }

        //----<select the check_out file for unit test >------
        private void PrepareForCheckout()
        {
            MouseButtonEventArgs e1 = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left)
            { RoutedEvent = Control.MouseDoubleClickEvent };
            RepoDirList.RaiseEvent(e1);
            String file = "MPC_Comm.h.2";
            CheckoutList.Items.Add(file);
            checkoutFile = file;
        }

        //------< delete popup file buffer when the windows closes>------
        private void Window_Closed(object sender, EventArgs e)
        {
            DeleteFiles("../../../ViewFileBuffer");
        }

        //----< process incoming messages on child thread >----------------
        private void processMessages()
        {
            try
            {
                ThreadStart thrdProc = () =>
                {
                    while (true)
                    {
                        CsMessage msg = translater.getMessage();
                        try
                        {
                            string msgId = msg.value("command");
                            Console.WriteLine("\n  received message:");
                            msg.show();
                            if (dispatcher_.ContainsKey(msgId))
                            {
                                dispatcher_[msgId].Invoke(msg);
                            }
                            dispatcher_["status"].Invoke(msg);
                        }
                        catch (Exception ex)
                        {
                            String ex1 = ex.Message;
                            continue;
                        }
                    }
                };
                rcvThrd = new Thread(thrdProc);
                rcvThrd.IsBackground = true;
                rcvThrd.Start();
            }
            catch (Exception exc)
            {
                Console.WriteLine(exc);
            }
        }

        //----< function dispatched by child thread to main thread >-------
        private void insertParent()
        {
            RepoDirList.Items.Insert(0, "..");
        }

        //----< add client processing for message with key >---------------
        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }

        //----< load connection success processing into dispatcher dictionary >---------------
        private void DispatcherLoadConnection()
        {
            Action<CsMessage> connect = (CsMessage rcvMsg) =>
            {
                Console.WriteLine("  Connection succeeds!");
                isConnected = true;
            };
            addClientProc("connected", connect);
        }

        //----< load getDirs processing into dispatcher dictionary >-------
        private void DispatcherLoadGetDirs()
        {
            Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
            {
                Action clrDirs = () => { RepoDirList.Items.Clear(); ; };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        Action<string> doDir = (string dir) =>
                        {
                            RepoDirList.Items.Add(dir);
                            CategoriesList.Items.Add(dir);
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                }
                Action insertUp = () => { insertParent(); };
                Dispatcher.Invoke(insertUp, new Object[] { });
            };
            addClientProc("getDirs", getDirs);
        }

        //----< load getFiles processing into dispatcher dictionary >------
        private void DispatcherLoadGetFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () => { RepoFileList.Items.Clear(); };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            RepoFileList.Items.Add(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("getFiles", getFiles);
        }

        //----< load checkout processing >---------------------------
        private void DispatcherLoadCheckout()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () => { RepoFileList.Items.Clear(); };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            RepoFileList.Items.Add(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("getFiles", getFiles);
        }

        //----< load view remote files processing into dispatcher dictionary >------
        private void DispatcherLoadViewFile()
        {
            Action<CsMessage> view = (CsMessage rcvMsg) =>
            {
                viewFilePath = "../../../ViewFileBuffer/" + rcvMsg.value("fileName");
                if (msgCount++ == 1)
                {
                    String metadata = string.Join("\n", ShowMetadata(rcvMsg).ToArray()); //write all metadata to a string
                                                                                         //popup 
                    Action<String, String> viewContent = (String fileName, String mData) => { ShowFileContent(rcvMsg.value("fileName"), mData); };
                    Dispatcher.Invoke(viewContent, new Object[] { rcvMsg.value("fileName"), metadata });
                }
            };
            addClientProc("viewFile", view);
        }

        //----< load show metadata processing into dispatcher dictionary >------
        private void DispatcherLoadMetadata()
        {
            Action<CsMessage> getMetadata = (CsMessage rcvMsg) =>
            {
                List<string> metadata = ShowMetadata(rcvMsg);
                Action<List<string>> showMetadata = (List<string> data) =>
                {
                    viewMetadata.Items.Clear();
                    foreach (string md in data)
                        viewMetadata.Items.Add(md);
                };
                Dispatcher.Invoke(showMetadata, new Object[] { metadata });
            };
            addClientProc("getMetadata", getMetadata);
        }
     
        //----< load status bar processing into dispatcher dictionary >------
        private void DispatcherLoadStatusBar()
        {
            Action<CsMessage> getStatus = (CsMessage rcvMsg) =>
            {
                Action<string> doStatus = (string status) => { statusBarText.Text = status; };
                string statusContent = "received msg from repo: " + rcvMsg.value("command");
                Dispatcher.Invoke(doStatus, new Object[] { statusContent });
            };
            addClientProc("status", getStatus);
        }

        //----< load  browsing file processing into dispatcher dictionary >------
        private void DispatcherLoadBrowsingFiles()
        {
            Action<CsMessage> getBrowsingFiles = (CsMessage rcvMsg) =>
            {
                try
                {
                    String filesStr = rcvMsg.attributes["BrowsingFiles"];
                    String[] fileList = filesStr.Split(null);
                    Action<String[]> getFileList = (String[] files) =>
                    {
                        isRemove = true;
                        showFiles.Items.Clear();
                        viewMetadata.Items.Clear();
                        if (fileList.Length != 1)
                        {
                            foreach (String file in files)
                                showFiles.Items.Add(file);
                        }
                        else
                            showFiles.Items.Add("No required files");
                        isRemove = false;
                    };
                    Dispatcher.Invoke(getFileList, new Object[] { fileList });

                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex);
                }
            };
            addClientProc("BrowsingFiles", getBrowsingFiles);
        }

        //----< load  show keys processing into dispatcher dictionary >------
        private void DispatcherLoadShowKeys()
        {
            Action<CsMessage> getBrowsingFiles = (CsMessage rcvMsg) =>
            {
                String keyStr = rcvMsg.attributes["keys"];
                String[] keysArray = keyStr.Split(null);
                Action<String[]> showKeys = (String[] keys) =>
                {
                    DependencyEditor editor = new DependencyEditor();
                    foreach (String key in keys)
                        editor.KeyList.Items.Add(key);
                    editor.PassValuesEvent += new DependencyEditor.PassValuesHandler(ReceiveValues);
                    editor.Show();
                };
                Dispatcher.Invoke(showKeys, new Object[] { keysArray });
            };
            addClientProc("getKeys", getBrowsingFiles);
        }

        //----< load  show root file of a specific category processing into dispatcher dictionary >------
        private void DispatcherLoadRoots()
        {
            Action<CsMessage> getRootFiles = (CsMessage rcvMsg) =>
            {
                String rootFiles = rcvMsg.attributes["roots"];
                String[] roots = rootFiles.Split(null);
                Action<String[]> showRoots = (String[] files) =>
                {
                    if (roots.Length == 0)
                        RootList.Items.Add("No root in this category");
                    else
                    {
                        RootList.Items.Clear();
                        foreach (String root in files)
                            RootList.Items.Add(root);
                    }
                };
                Dispatcher.Invoke(showRoots, new Object[] { roots });
            };
            addClientProc("getRoot", getRootFiles);

        }

        //----<show files and dirs in the local storage>------------------
        private void InitLocalStorageView()
        {
            List<string> dirs = GetLocalDirs("../../../Staging/");
            List<string> files = GetLocalFiles("../../../Staging/");
            DirList.Items.Add("..");
            foreach (string dir in dirs)
            {
                DirList.Items.Add(dir);
            }
            foreach (string file in files)
            {
                FileList.Items.Add(file);
            }
            LocalPathTextBlock.Text = "Staging";
            checkinPathStack_.Push("../../../Staging/");
        }

        //----<show files and dirs in the repository>----------------------
        private void ShowRepo()
        {
            PathTextBlock.Text = PathTextBlock.Text == "" ? "Repository" : PathTextBlock.Text;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", checkoutPathStack_.Peek());
            translater.postMessage(msg);
            msg.remove("command");
            msg.add("command", "getFiles");
            translater.postMessage(msg);
        }

        //----<get dirs in the local staging directory>----------------------
        private List<string> GetLocalDirs(string localPath)
        {
            string[] dirs = Directory.GetDirectories(localPath);
            List<string> dirsName = new List<string>();
            foreach (string dir in dirs)
            {
                dirsName.Add(dir.Substring(dir.LastIndexOf("/") + 1));
            }
            return dirsName;
        }

        //----<get files in the local staging directory>--------------------
        private List<string> GetLocalFiles(string localPath)
        {
            string[] files = Directory.GetFiles(localPath);
            List<string> filesName = new List<string>();
            foreach (string file in files)
            {
                filesName.Add(System.IO.Path.GetFileName(file));
            }
            return filesName;
        }

        //----< load all dispatcher processing >---------------------------
        private void loadDispatcher()
        {
            DispatcherLoadConnection();
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
            DispatcherLoadViewFile();
            DispatcherLoadStatusBar();
            DispatcherLoadBrowsingFiles();
            DispatcherLoadShowKeys();
            DispatcherLoadMetadata();
            DispatcherLoadRoots();
        }

        //----<Remove file buffer which is used to stored poping up files>------
        public void DeleteFiles(string dir)
        {
            try
            {
                if (Directory.Exists(dir))
                {
                    foreach (string file in Directory.GetFileSystemEntries(dir))
                    {
                        if (File.Exists(file))
                            File.Delete(file);
                        else
                            DeleteFiles(file);
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }
        }

        //----< strip off name of first part of path >---------------------
        private string removeFirstDir(string path)
        {
            string modifiedPath = path;
            int pos = path.IndexOf("/");
            modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
            return modifiedPath;
        }

        //----< respond to mouse double-click on local dir name >----------------
        private void DirList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string selectedDir = (string)DirList.SelectedItem;
            string path;
            if (selectedDir == "..")
            {
                if (checkinPathStack_.Count > 1)
                {  // don't pop off "Storage"
                    checkinPathStack_.Pop();
                    path = checkinPathStack_.Peek();
                }
                else
                    return;
            }
            else
            {
                path = checkinPathStack_.Peek() + "/" + selectedDir;
                checkinPathStack_.Push(path);
            }
            FileList.Items.Clear();
            DirList.Items.Clear();
            DirList.Items.Add("..");
            List<string> dirs = GetLocalDirs(path);
            foreach (string dir in dirs)
            {
                DirList.Items.Add(dir);
            }
            List<string> files = GetLocalFiles(path);
            foreach (string file in files)
            {
                FileList.Items.Add(file);
            }
            Console.WriteLine(checkinPathStack_.Peek());
            // display path in Dir TextBlcok
            LocalPathTextBlock.Text = checkinPathStack_.Peek().Length < 23 ? "Staging" : checkinPathStack_.Peek().Substring(9);
        }

        //----< respond to mouse double-click on remote dir name >----------------
        private void RepoDirList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string selectedDir = "";
            if (isTest)
               selectedDir = "MPC";
            else
              selectedDir = (string)RepoDirList.SelectedItem;
            string path;
            if (selectedDir == "..")
            {
                if (checkoutPathStack_.Count > 1)
                {  // don't pop off "Storage"
                    checkoutPathStack_.Pop();
                    path = checkoutPathStack_.Peek();
                }
                else
                    return;
            }
            else
            {
                path = checkoutPathStack_.Peek() + "/" + selectedDir;
                checkoutPathStack_.Push(path);
            }
            RepoFileList.Items.Clear();
            RepoDirList.Items.Clear();
            RepoDirList.Items.Add("..");
            // display path in Dir TextBlcok
            PathTextBlock.Text = removeFirstDir(checkoutPathStack_.Peek());
            ShowRepo();
        }

        //----< respond to mouse right-click to add a parent check_in file >----------------
        private void Parent_Click(object sender, RoutedEventArgs e)
        {
            if (hasParent == false)
            {
                CheckinList.Items.Clear();
                String parent = FileList.SelectedItems[0] as string;
                TextBlock tb = new TextBlock();
                tb.Text = parent;
                tb.FontWeight = FontWeights.Bold;
                CheckinList.Items.Add(tb);
                checkinFile = parent;
                hasParent = true;
                Set_Parent.IsEnabled = false;

            }
        }

        //----< send getKeys message to the remote server >----------------
        private void GetKeysInRepo()
        {
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getKeys");
            translater.postMessage(msg);
            msg.remove("command");
        }

        //----< respond to mouse right-click to remove selected check_in files >----------------
        private void Reset_Click(object sender, RoutedEventArgs e)
        {
            String file = FileList.SelectedItems[0] as string;
            if (file == checkinFile)
            {
                hasParent = false;
                Set_Parent.IsEnabled = true;
            }
            CheckinList.Items.Remove(CheckinList.SelectedItem);
        }

        //----< respond to mouse right-click to add a check_out file >-------
        private void Checkout_Select_Click(object sender, RoutedEventArgs e)
        {
            String file = RepoFileList.SelectedItems[0] as string;
            CheckoutList.Items.Add(file);
            checkoutFile = file;
        }

        //----< respond to pop up file content >-------
        private void FileMouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string fileName;
            if (isTest)
                fileName = "MPC_IComm.h.1";
            else fileName = RepoFileList.SelectedValue as string;
            try
            {
                string path = System.IO.Path.Combine(checkoutPathStack_.Peek(), fileName);

                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "viewFile");
                msg.add("viewFile", checkoutPathStack_.Peek());
                msg.add("fileName", fileName);
                translater.postMessage(msg);
                msg.show();
                // build message to get files and post it
                msg.remove("command");
                msgCount = 1;
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
            }
        }

        //----< show file content in both the popup window and the GUI tab>-------
        private void ShowFileContent(String fileName, String metadata)
        {
            string contents = File.ReadAllText(viewFilePath);
            Paragraph popUpParagraph = new Paragraph();
            popUpParagraph.Inlines.Add(new Run(contents));
            PopUp popUp = new PopUp();
            popUp.codeView.Blocks.Clear();
            popUp.codeView.Blocks.Add(popUpParagraph);
            //show content in the view file tab of GUI
            Paragraph viewFileparagraph = new Paragraph();
            viewFileparagraph.Inlines.Add(new Run(contents));
            popUp.codeLabel.Text = fileName;
            fileView.Blocks.Clear();
            fileView.Blocks.Add(viewFileparagraph);
            //show metadata in the popup window
            Paragraph viewMetadataparagraph = new Paragraph();
            viewMetadataparagraph.Inlines.Add(new Run(metadata));
            popUp.metadataView.Blocks.Clear();
            popUp.metadataView.Blocks.Add(viewMetadataparagraph);
            popUp.Show();
        }

        //----< get children file names and category names for the metadata>-------
        private List<string> ShowMetadata(CsMessage rcvMsg)
        {
            List<string> mdList = new List<string>();
            mdList.Add("FileName:" + rcvMsg.attributes["name"]);
            mdList.Add("Description:" + rcvMsg.attributes["description"]);
            mdList.Add("DateTime:" + rcvMsg.attributes["dateTime"]);
            mdList.Add("Path:" + rcvMsg.attributes["payLoadPath"]);
            mdList.Add("Status:");
            if (rcvMsg.attributes["status"] == "0")
                mdList.Add("<open>");
            else if (rcvMsg.attributes["status"] == "1")
                mdList.Add("<closed>");
            else
                mdList.Add("<closing>");
            mdList.Add("categories:");
            List<String> categories = GetMembers(rcvMsg, "category");
            foreach (String category in categories)
                mdList.Add(category);
            mdList.Add("dependencies:");
            List<String> children = GetMembers(rcvMsg, "child");
            foreach (String child in children)
                mdList.Add(child);
            return mdList;
        }

        //----< show file metadata in the view metadata tab>-------
        private List<String> GetMembers(CsMessage rcvMsg, String keyword)
        {
            List<String> children = new List<string>();
            var enumer = rcvMsg.attributes.GetEnumerator();
            while (enumer.MoveNext())
            {
                string key = enumer.Current.Key;
                if (key.Contains(keyword))
                {
                    children.Add(enumer.Current.Value);
                }
            }
            return children;
        }

        //----< respond to check_in click >-------
        private void Checkin_Click(object sender, RoutedEventArgs e)
        {
            if (isConnected == false)
            {
                MessageBox.Show("Please connect to the server first!");
                return;
            }
            if (checkinFile == "")
            {
                MessageBox.Show("Please select a parent file.");
                return;
            }
            if (Check())
            {
                CatchMetadata();
                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "checkin");
                msg.add("source", checkinPathStack_.Peek());
                msg.add("destination", "../Repository/");
                msg.add("file", checkinFile);
                msg.add("toFile", checkinFile);
                msg.add("name", metadata_["name"]);
                msg.add("namespace", metadata_["namespace"]);
                msg.add("description", metadata_["description"]);
                msg.add("category", metadata_["category"]);
                msg.add("status", metadata_["status"]);
                msg.add("children", metadata_["children"]);
                translater.postMessage(msg);
                msg.show();
                msg.remove("command");
                checkinFile = "";
                CheckinList.Items.Clear();
                Set_Parent.IsEnabled = true;
                hasParent = false;
            }
        }

        //-----<get metadata provided by client users>------------
        private void CatchMetadata()
        {
            String name = NameText.Text;
            String nameSpace = NamespaceText.Text;
            String description = DescriptionText.Text;
            String category = CategText.Text;
            String status = StatusComboBox.Text;

            String children = "";
            for (int i = 1; i < CheckinList.Items.Count; i++)
            {
                String child = CheckinList.Items[i].ToString();
                children += child + " ";
            }
            metadata_["name"] = name;
            metadata_["description"] = description;
            metadata_["namespace"] = nameSpace;
            metadata_["category"] = category;
            metadata_["status"] = status;
            metadata_["children"] = children;
        }
      
        //----< respond to check_out click >-------
        private void Checkout_Click(object sender, RoutedEventArgs e)
        {
            if (isConnected == false)
            {
                MessageBox.Show("Please connect to the server first!");
                return;
            }
            if (checkoutFile == "")
            {
                MessageBox.Show("Please select a parent file.");
                return;
            }

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("destination", "../../../Staging");
            msg.add("source", checkoutPathStack_.Peek());
            msg.add("command", "checkout");
            msg.add("requestfile", checkoutFile);
            translater.postMessage(msg);
            msg.show();

            // build message to get files and post it
            msg.remove("command");
            CheckoutList.Items.Clear();
            checkoutFile = "";
        }

        //----< show current files in the repository to show new check_in files >-------
        private void Repo_Refresh_Button_Click(object sender, RoutedEventArgs e)
        {
            RepoDirList.Items.Clear();
            RepoFileList.Items.Clear();
            ShowRepo();
        }

        //----< show current files in the local storage to show new check_out files >-------
        private void Local_Refresh_Button_Click(object sender, RoutedEventArgs e)
        {
            DirList.Items.Clear();
            FileList.Items.Clear();
            InitLocalStorageView();
        }

        //----<trigger connection to start comm and load dispatcher>------------------------
        private void Connect_Click(object sender, RoutedEventArgs e)
        {
            // start Comm
            serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;

            endPoint_ = new CsEndPoint();
            endPoint_.machineAddress = "localhost";
            endPoint_.port = int.Parse(Environment.GetCommandLineArgs()[1]);

            translater = new Translater();
            translater.listen(endPoint_);
            loadDispatcher();
            processMessages();

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "connect");
            translater.postMessage(msg);
            msg.show();
            msg.remove("command");
            checkoutPathStack_.Push("../Repository");
            ShowRepo();
            Console.WriteLine("\n  Connected to the repo server.");
        }

        //--------------------<Browse button click event>------------------------------
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (isConnected == false)
            {
                MessageBox.Show("Please connect to the server first!");
                return;
            }
            if (ConditionComboBox == null)
            {
                MessageBox.Show("Please select a query condition!");
                return;
            }
            String queryCondition = ConditionComboBox.Text;
            String content = tbQueryContent.Text;
            if (content == "")
            {
                MessageBox.Show("Please input browsing arguments.");
                return;
            }
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "browse");
            msg.add("queryCondition", queryCondition);
            msg.add("queryContent", content);
            translater.postMessage(msg);
            msg.show();
            msg.remove("command");

        }

        //--------------------<Add_Dependency button click event>------------------------------
        private void Add_Dependency_Click(object sender, RoutedEventArgs e)
        {
            if (!isConnected)
            {
                MessageBox.Show("Please connect first!");
                return;
            }
            GetKeysInRepo();

        }

        //--------------------<get keys from Dependency Editor window>---------------------------
        private void ReceiveValues(object sender, PassValuesEventArgs e)
        {
            foreach (string child in e.children)
            {
                CheckinList.Items.Add(child);
            }
        }

        //--------------------<Check whether the user provide all metadata for check_in>-------------------
        private bool Check()
        {
            String target = "";
            if (NameText.Text == "")
                target = "name";
            else if (NamespaceText.Text == "")
                target = "namespace";
            else if (DescriptionText.Text == "")
                target = "description";
            else if (CategText.Text == "")
                target = "category";
            else if (StatusComboBox.Text == "")
                target = "status";
            else
            {
                return true;
            }
            MessageBox.Show("Please set " + target);
            return false;
        }

        //--------------------<send message to server to get metadata of the selected file>-------------------
        private void ShowFiles_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (!isRemove)
            {
                String fileName = showFiles.SelectedValue as String;
                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "getMetadata");
                msg.add("fileName", fileName);
                translater.postMessage(msg);
                msg.show();
                msg.remove("command");
            }
            else return;


        }
        
        //--------------------<send message to server to get files without parents of the selected category>-------------------
        private void CategoriesList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

            String category = CategoriesList.SelectedValue as String;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getRoot");
            msg.add("category", category);
            translater.postMessage(msg);
            msg.show();
            msg.remove("command");

        }

     
    }
}
