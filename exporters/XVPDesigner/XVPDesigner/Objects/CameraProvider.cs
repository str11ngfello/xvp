using System;
using System.Collections.Generic;
using System.Text;

namespace XVPDesigner.Objects
{
	public interface CameraProvider
	{
		string setAsCamera(uint viewportIndex);
		void removeAllPostEffects();
		void addPostEffect(BasePostEffect postEffect);
	}
}
