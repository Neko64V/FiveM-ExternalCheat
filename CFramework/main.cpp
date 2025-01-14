#include "Cheat/FrameCore.h"
#include "Framework/Overlay/Overlay.h"

Overlay*	overlay = new Overlay;
CFramework* cheat = new CFramework;

// DEBUG時にはコンソールウィンドウを表示する
#if _DEBUG
int main()
#else 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	// DPIスケールによるあれを防ぐ
	ImGui_ImplWin32_EnableDpiAwareness();

	// Apexのウィンドウをベースにして初期化を行う
	if (!m.AttachProcess("grcWindow", WINDOW_CLASS))
		return 1;

	m.GetBaseAddress("None.", "citizen-playernames-five.dll");

	// Overlay
	if (!overlay->InitOverlay("grcWindow", WINDOW_CLASS))
		return 2;

	// Sorry, i added this
	if (!cheat->Init())
		return 3;

	overlay->OverlayLoop();
	overlay->DestroyOverlay();
	m.DetachProcess();
	g.Run = false;
	delete cheat, overlay;

	return 0;
}

void Overlay::OverlayUserFunction()
{
	cheat->MiscAll();

	cheat->RenderInfo();

	if (g.ESP)
		cheat->RenderESP();

	if (g.ShowMenu)
		cheat->RenderMenu();
}