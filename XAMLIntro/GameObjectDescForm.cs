using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace XAMLIntro
{
    public class GameObjectDescForm : StackPanel
    {
        //THis panel will contain everything needed to define a basic Entity object.
        // + Position
        // + Orientation
        // + Scale
        // + Name
        // + Dimension (Layer)

        //Labels for all the GameObject properties.
        //private Label mNameLbl = new Label();
        //private Label mPositionLbl = new Label();
        //private Label mOrientationLbl = new Label();
        //private Label mScaleLbl = new Label();
        //private Label mDimensionLbl = new Label();

        //Position is 3 input boxes.
        private TextBox mXPosInput;
        private TextBox mYPosInput;
        private TextBox mZPosInput;

        //Orientation is 3 as well, euler angles.
        private TextBox mXRotInput;
        private TextBox mYRotInput;
        private TextBox mZRotInput;

        //Scale is 3 as well, with checkbox for uniform scale.  (Uniform uses x-input for all of them.)
        private TextBox mXScaleInput;
        private TextBox mYScaleInput;
        private TextBox mZScaleInput;
        private CheckBox mUniformScale;

        private Button m_OkBtn;

        public GameObjectDescForm()
        {
            //The Layout of the GameObjectDescForm is always in the same place.  One of the few constant panels/forms in editor.
            initializeBaseForm();
        }

        private void initializeBaseForm()
        {
            m_OkBtn = new Button();            

            m_OkBtn.RenderSize = new Size(20, 20);
            m_OkBtn.Content = "Hello World";
            m_OkBtn.Margin = new Thickness(10, 10, 50, 50);
            m_OkBtn.Visibility = Visibility.Visible;

            AddLogicalChild(m_OkBtn);
            AddVisualChild(m_OkBtn);            
        }
    }
}
