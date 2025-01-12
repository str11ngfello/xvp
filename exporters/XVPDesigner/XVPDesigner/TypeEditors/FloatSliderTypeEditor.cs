using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms.Design;
using XVPDesigner.UserControls;

namespace XVPDesigner.TypeEditors
{
	public class FloatSliderTypeEditor : System.Drawing.Design.UITypeEditor
	{
		private IWindowsFormsEditorService edSvc;
		private FloatSliderEditor editor = new FloatSliderEditor();

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
				editor.Value = Convert.ToSingle(Program.g_mainForm.ActivePropertyExplorer.SelectedGridItem.PropertyDescriptor.GetValue(Program.g_mainForm.ActivePropertyExplorer.SelectedObject));

				//Find the EditorRange object and set min and max values
				foreach (Attribute a in Program.g_mainForm.ActivePropertyExplorer.SelectedGridItem.PropertyDescriptor.Attributes)
				{
					EditorRange range = a as EditorRange;
					if (range != null)
					{
						editor.init(Convert.ToSingle(range.MinValue),
									Convert.ToSingle(range.MaxValue),
									Convert.ToSingle(range.Step));
						break;
					}
				}

				editor.MainTrackBar.ValueChanged += new EventHandler(slider_ValueChanged);

				//Here, you instruct the propertygrid to show the control in a combo box
				if (Program.g_mainForm.ActivePropertyExplorer != null)
					editor.Value = (float)Program.g_mainForm.ActivePropertyExplorer.SelectedGridItem.PropertyDescriptor.GetValue(Program.g_mainForm.ActivePropertyExplorer.SelectedObject);
				edSvc.DropDownControl(editor);

				editor.MainTrackBar.ValueChanged -= new EventHandler(slider_ValueChanged);

				return editor.Value;
			}

			return value;
		}

		public void slider_ValueChanged(object sender,EventArgs args)
		{
			if (Program.g_mainForm.ActivePropertyExplorer != null)
				Program.g_mainForm.ActivePropertyExplorer.SelectedGridItem.PropertyDescriptor.SetValue(Program.g_mainForm.ActivePropertyExplorer.SelectedObject,editor.Value);
		}
	}
}
