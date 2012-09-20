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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Media.Media3D;

namespace XAMLIntro
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
        }

        private void ListView_SelectionChanged_1(object sender, SelectionChangedEventArgs e)
        {

        }

        private void TextBox_LostFocus_1(object sender, RoutedEventArgs e)
        {
            //Store current content. 
            TextBox box = e.OriginalSource as TextBox;            

            if (box != null)
            {
                //Check content of the box.
                string toCheck = (sender as TextBox).Text;
                double num;
                bool isNum = double.TryParse(toCheck, out num);
                if (isNum)
                {
                    //All is well, accept the new input.
                }
                else
                {
                    //Undo last change to box, which is the entry of this non-valid input.
                    box.Undo();
                }
            }
        }
    }
}
