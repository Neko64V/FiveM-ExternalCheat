#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <string>

/*	[+] メモリやオーバーレイの初期化モードを設定します

	WINDOW_TITLE : ウィンドウのタイトルを使用します
	WINDOW_CLASS : ウィンドウのクラス名を使用します
	PROCESS      : 実行ファイル名を使用します				*/
enum InitializeMode : int
{
	WINDOW_TITLE,
	WINDOW_CLASS,
	PROCESS
};

// これからのコーディングを少し楽にする素晴らしいMemoryクラス
class Memory
{
private:
	uintptr_t GetModuleBase(const std::string moduleName);
	PROCESSENTRY32 GetProcess(const std::string processName);
public:
	DWORD m_dwPID;
	HANDLE m_hProcess;
	uintptr_t m_gProcessBaseAddr;
	uintptr_t m_gNameDllBaseAddr;

	bool AttachProcess(const char* targetName, int InitMode);
	void GetBaseAddress(const char* targetName1, const char* targetName2);
	void DetachProcess();

	template <typename T>
	constexpr const T Read(const uintptr_t& address) const noexcept
	{
		T value{};
		ReadProcessMemory(m_hProcess, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}
	template <typename T>
	constexpr void Write(const uintptr_t& address, const T& value) const noexcept
	{
		WriteProcessMemory(m_hProcess, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
	bool ReadString(uintptr_t address, LPVOID buffer, SIZE_T size) const
	{
		SIZE_T size_read;
		return !!::ReadProcessMemory(m_hProcess, LPCVOID(address), buffer, size, &size_read) && size_read > 0;
	}
};

extern Memory m;