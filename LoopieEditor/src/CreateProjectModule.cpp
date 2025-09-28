#include "CreateProjectModule.h"

namespace Loopie
{
	CreateProjectModule::CreateProjectModule()
	{

	}
	void CreateProjectModule::OnLoad()
	{
		m_interface = new ProjectSetupInterface();
	}
	void Loopie::CreateProjectModule::OnUnload()
	{
		delete m_interface;
	}
	void Loopie::CreateProjectModule::OnInterfaceRender()
	{
		m_interface->Render();
	}
}