#include <windows.h>
// for StrRChr
#include <shlwapi.h>
#include <string>
#include "geheim.h"

typedef std::basic_string<TCHAR> tstring;

static const TCHAR child[] = TEXT("dbaur.exe");

static void chdirToExeDir()
{
	TCHAR prog_dir[MAX_PATH];

	if(GetModuleFileName(NULL, prog_dir, MAX_PATH)) {
		TCHAR *lastbs = StrRChr(prog_dir, NULL, TEXT('\\'));
		// keep backslash to make SetCurrentDirectory happy
		if (lastbs) *(lastbs + 1) = TEXT('\0');
		SetCurrentDirectory(prog_dir);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	const size_t secret_size = dbaurshim::secret.size();
	const size_t key_size = dbaurshim::key.size();
	size_t i;
	STARTUPINFO startmeup;
	PROCESS_INFORMATION pi;

	// build zero-delimited TSTR
	tstring prog_args = child;
	prog_args.reserve(prog_args.size() + secret_size + 2);
	prog_args.append(1, TEXT(' '));
	// no cbegin() on C++98
	for (i = 0; i < secret_size; i++)
		prog_args += static_cast<TCHAR>(dbaurshim::secret[i] ^ dbaurshim::key[i % key_size]);
	prog_args.append(1, TEXT('\0'));

	chdirToExeDir();

	ZeroMemory(&startmeup, sizeof(startmeup));
	startmeup.cb = sizeof(startmeup);
	ZeroMemory(&pi, sizeof(pi));

	// decay to pointer to (mutable) TCHAR for CreateProcessW
	if (CreateProcess(child, &prog_args[0],
			NULL, NULL, FALSE, 0, NULL, NULL,
			&startmeup, &pi)) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return 0;
	} else {
		return 1;
	}
}