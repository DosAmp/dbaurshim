#include <windows.h>
#include <tchar.h>
// for StrRChr
#include <shlwapi.h>
#include <string>
#include "secret.h"

typedef std::basic_string<TCHAR> tstring;

static const TCHAR CHILD[] = TEXT("dbaur.exe");

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
	size_t i;
	STARTUPINFO startmeup;
	PROCESS_INFORMATION pi;

	// build zero-delimited TSTR
	tstring prog_args;
	prog_args.reserve(_tcslen(CHILD) + 2 * secret_size + 2);
	prog_args.append(CHILD).append(1, TEXT(' '));
	// no cbegin() on C++98
	for (i = 0; i < secret_size; i++) {
		prog_args += static_cast<TCHAR>('0' + dbaurshim::secret.at(i) / 16);
		prog_args += static_cast<TCHAR>('0' + dbaurshim::secret.at(i) % 16);
	}
	prog_args.append(1, TEXT('\0'));

	chdirToExeDir();

	ZeroMemory(&startmeup, sizeof(startmeup));
	startmeup.cb = sizeof(startmeup);
	ZeroMemory(&pi, sizeof(pi));

	// decay to pointer to (mutable) TCHAR for CreateProcessW
	if (CreateProcess(CHILD, &prog_args[0],
			NULL, NULL, FALSE, 0, NULL, NULL,
			&startmeup, &pi)) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return 0;
	} else {
		return 1;
	}
}