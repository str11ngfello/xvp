using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms.Design;
using XVPDesigner.UserControls;

namespace XVPDesigner.TypeEditors
{
	public class DirVecTypeEditor : System.Drawing.Design.UITypeEditor
	{
		private IWindowsFormsEditorService edSvc;
		private DirVecEditor editor = new DirVecEditor();

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
				float[] components = (float[])(Program.g_mainForm.ActivePropertyExplorer.SelectedGridItem.PropertyDescriptor.GetValue(Program.g_mainForm.ActivePropertyExplorer.SelectedObject));
				editor.XSpinner.ValueChanged += new EventHandler(spinner_ValueChanged);
				editor.YSpinner.ValueChanged += new EventHandler(spinner_ValueChanged);
				editor.ZSpinner.ValueChanged += new EventHandler(spinner_ValueChanged);

				//Here, you instruct the propertygrid to show the control in a combo box
				edSvc.DropDownControl(editor);

				editor.XSpinner.ValueChanged -= new EventHandler(spinner_ValueChanged);
				editor.YSpinner.ValueChanged -= new EventHandler(spinner_ValueChanged);
				editor.ZSpinner.ValueChanged -= new EventHandler(spinner_ValueChanged);

				return new float[] {editor.XValue,editor.YValue,editor.ZValue};
			}

			return value;
		}

		public void spinner_ValueChanged(object sender,EventArgs args)
		{
			if (Program.g_mainForm.ActivePropertyExplorer != null)
			{

				Program.g_mainForm.ActivePropertyExplorer.SelectedGridItem.PropertyDescriptor.SetValue(Program.g_mainForm.ActivePropertyExplorer.SelectedObject,new float[] {editor.XValue,editor.YValue,editor.ZValue});
			}
		}
	}
}
