
#include <xvpsdk.h>

extern XHandle rcHandle;

class MainState : public XState
{
private:
	static MainState m_MainState;
	
	XScene m_mainScene;
	XHDRPostEffect hdrPostEffect;

	XScene m_uiScene;

	XCamera c;

	XModelTemplate mt;
	XModel m;
	
	XWhiteSC dsc;
	
	XUFOController fpc;

	XUserInterface m_UI;
public:
	XS8* getStateDesc();
	XStatus init();
	XStatus destroy();
	XStatus activate(void* pUserData);
	XStatus deactivate();
	XStatus update(XF32 dt);
	static MainState* instance() {return &m_MainState;}
};

