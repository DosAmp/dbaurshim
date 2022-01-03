#include "geheim.h"

#include <cstring>
#include <string>

#include <windows.h>
#include <tchar.h>
// for StrRChr
#include <shlwapi.h>

typedef std::basic_string<TCHAR> tstring;

#if !defined(DBAURSHIM_HAS_SECRET) || !defined(DBAURSHIM_HAS_KEY)
#error Where's the secret, dawg?
#endif

static const TCHAR child[] = TEXT("dbaur.exe");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	const size_t secret_size = sizeof(dbaurshim::secret);
	const size_t key_size = strlen(dbaurshim::key);
	size_t off;
	STARTUPINFO startmeup;
	PROCESS_INFORMATION pi;
	TCHAR *prog_args;
	TCHAR prog_dir[MAX_PATH];

	tstring decrypted_secret(secret_size, static_cast<TCHAR>(0));
	for (off = 0; off < secret_size; off++)
		decrypted_secret[off] = static_cast<TCHAR>(dbaurshim::secret[off] ^ dbaurshim::key[off % key_size]);

	tstring prog_arg = child;
	prog_arg += static_cast<TCHAR>(' ');
	prog_arg += decrypted_secret;

	if(GetModuleFileName(NULL, prog_dir, MAX_PATH)) {
		TCHAR *lastbs = StrRChr(prog_dir, NULL, (TCHAR)'\\');
		// keep backslash for SetCurrentDirectory
		if (lastbs) *(lastbs + 1) = static_cast<TCHAR>(0);
		SetCurrentDirectory(prog_dir);
	}

	ZeroMemory(&startmeup, sizeof(startmeup));
	startmeup.cb = sizeof(startmeup);
	ZeroMemory(&pi, sizeof(pi));

#ifdef UNICODE
	// god damn, CreateProcessW 
	prog_args = new WCHAR[prog_arg.size() + 1];
	if (!prog_args) return 1;
	wcscpy(prog_args, prog_arg.c_str());
#else
	prog_args = const_cast<CHAR*>(prog_arg.c_str());
#endif
	if (CreateProcess(child, prog_args,
			NULL, NULL, FALSE, 0, NULL, NULL,
			&startmeup, &pi)) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
#ifdef UNICODE
	delete prog_args;
#endif
	return 0;
}