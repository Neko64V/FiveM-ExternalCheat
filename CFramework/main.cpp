#include "Cheat/FrameCore.h"
#include "Framework/Overlay/Overlay.h"

Overlay*	C_Overlay = new Overlay;
CFramework* C_FiveM = new CFramework;

// DEBUG時にはコンソールウィンドウを表示する
#if _DEBUG
int main()
#else 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	// Fix DPI Scale
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

	// Apexのウィンドウをベースにして初期化を行う
	if (!m.AttachProcess("grcWindow", MODE::WINDOW))
		return 1;

	// Overlay
	if (!C_Overlay->InitOverlay("grcWindow", MODE::WINDOW))
		return 2;

	// Sorry, i added this
	if (!C_FiveM->Init())
		return 3;

	C_Overlay->OverlayLoop();
	C_Overlay->DestroyOverlay();
	m.DetachProcess();
	g.Run = false;
	delete C_FiveM, C_Overlay;

	return 0;
}

void Overlay::OverlayLoop()
{
	while (g.Run)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		C_FiveM->MiscAll();
		OverlayManager("grcWindow");

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		C_FiveM->RenderInfo();

		if (g.ESP)
			C_FiveM->RenderESP();

		if (g.ShowMenu)
			C_FiveM->RenderMenu();

		ImGui::Render();
		const float clear_color_with_alpha[4] = { 0.f, 0.f, 0.f, 0.f };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pSwapChain->Present(1, 0);
	}
}