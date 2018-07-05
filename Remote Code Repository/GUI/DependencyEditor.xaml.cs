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
    /// <summary>
    /// MetadataEditor.xaml 的交互逻辑
    /// </summary>
    public partial class DependencyEditor : Window
    {
        public delegate void PassValuesHandler(object sender, PassValuesEventArgs e);

        public event PassValuesHandler PassValuesEvent;

        public DependencyEditor()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {

            List<string> children = new List<string>();
            foreach (string item in KeyList.SelectedItems)
                children.Add(item);
            PassValuesEventArgs args = new PassValuesEventArgs(children);
            PassValuesEvent(this, args);

            this.Close();
        }
    }
    public class PassValuesEventArgs  : EventArgs
    {
        
        public List<string> children { get; set; } = new List<string>();

        public PassValuesEventArgs(List<string> children)
        {
            this.children = children;
        }
    }
}
