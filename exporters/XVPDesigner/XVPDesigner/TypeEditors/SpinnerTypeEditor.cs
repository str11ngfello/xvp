using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms.Design;
using XVPDesigner.UserControls;

namespace XVPDesigner.TypeEditors
{
	public class SpinnerTypeEditor : System.Drawing.Design.UITypeEditor
	{
		private IWindowsFormsEditorService edSvc;
		private SpinnerEditor editor = new SpinnerEditor();

		public override System.Drawing.Design.UITypeEditorEditStyle GetEditStyle(System.ComponentModel.ITypeDescriptorContext context)
		{
			//If you want the editor to be based on a form (external form that appears when the user presses the ... button), 
			//return UITypeEditorEditStyle.Modal
			return System.Drawing.Design.UITypeEditorEditStyle.DropDown;
		}

		public override object EditValue(System.ComponentModel.ITypeDescriptorContext context,System.IServiceProvider provider,object value)
		{
			edSvc = (IWindowsFormsEditorService)provider.GetService(typeof(IWindowsFormsEditorService));
			if (edSvc != null)
			{
				editor.Value = Convert.ToDecimal(Program.g_mainForm.ActivePropertyExplorer.SelectedGridItem.PropertyDescriptor.GetValue(Program.g_mainForm.ActivePropertyExplorer.SelectedObject));

				//Find the EditorRange object and set min and max values
				foreach (Attribute a in Program.g_mainForm.ActivePropertyExplorer.SelectedGridItem.PropertyDescriptor.Attributes)
				{
					EditorRange range = a as EditorRange;
					if (range != null)
					{
						editor.MinValue = Convert.ToDecimal(range.MinValue);
						editor.MaxValue = Convert.ToDecimal(range.MaxValue);
						//editor.Slider.Step = 2;// Convert.ToInt32(range.Step);
						break;
					}
				}

				editor.Spinner.ValueChanged += new EventHandler(spinner_ValueChanged);

				//Here, you instruct the propertygrid to show the control in a combo box
				edSvc.DropDownControl(editor);

				editor.Spinner.ValueChanged -= new EventHandler(spinner_ValueChanged);

				return editor.Value;
			}

			return value;
		}

		public void spinner_ValueChanged(object sender,EventArgs args)
		{
			if (Program.g_mainForm.ActivePropertyExplorer != null)
				Program.g_mainForm.ActivePropertyExplorer.SelectedGridItem.PropertyDescriptor.SetValue(Program.g_mainForm.ActivePropertyExplorer.SelectedObject,Convert.ToSingle(editor.Value));
		}
	}
}