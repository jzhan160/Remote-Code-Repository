///////////////////////////////////////////////////////////////////////
// Window1.xaml.cs - Client prototype GUI for Pluggable Repository   //
// Ver 1.0                                                           //
// Jim Fawcett, CSE681-OnLine, Summer 2017                           //
///////////////////////////////////////////////////////////////////////
/*  
 *  Purpose:
 *    Prototype for a secondary popup window for the Pluggable Repository Client.  
 *    This application doesn't connect to the repository - it has no Communication 
 *    facility.  It simply explores kinds of user interface elements needed for that.
 *
 *  Required Files:
 *    MainWindow.xaml, MainWindow.xaml.cs - view into repository and checkin/checkout
 *    Window1.xaml, Window1.xaml.cs       - Code and MetaData view for individual packages
 *
 *
 *  Maintenance History:
 *    ver 1.0 : 15 Jun 2017
 *    - first release
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace GUI
{

    public partial class PopUp : Window
    {
        private static double leftOffset = 500.0;
        private static double topOffset = -20.0;
        public PopUp()
        {
            InitializeComponent();
            InitializeComponent();
            double Left = Application.Current.MainWindow.Left;
            double Top = Application.Current.MainWindow.Top;
            this.Left = Left + leftOffset;
            this.Top = Top + topOffset;
            this.Width = 600.0;
            this.Height = 800.0;
            leftOffset += 20.0;
            topOffset += 20.0;
            if (leftOffset > 700.0)
                leftOffset = 500.0;
            if (topOffset > 180.0)
                topOffset = -20.0;
        }
    private void exitButton_Click(object sender, RoutedEventArgs e)
    {
        this.Close();
    }
    }

    

}
