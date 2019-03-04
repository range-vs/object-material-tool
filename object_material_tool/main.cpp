#include "MainWindow.h"

using namespace object_material_tool;

[STAThreadAttribute]
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hIntOld, LPSTR cmdLine, int typeShowWindow)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Application::Run(gcnew MainWindow());
	return 0;
}
