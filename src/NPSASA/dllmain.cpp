// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

extern HMODULE _hModule;

HMODULE _hModule = 0;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		_hModule = hModule;
		DisableThreadLibraryCalls(hModule);
	case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


class HKEYContainer
{
	HKEY _hkey;
public:
	HKEYContainer(HKEY hkey)
	{
		_hkey = hkey;
	}

	~HKEYContainer()
	{
		Close();
	}

	operator HKEY()
	{
		return _hkey;
	}

	void Close()
	{
		if (_hkey != NULL)
		{
			RegCloseKey(_hkey);
			_hkey = NULL;
		}
	}
};


DWORD UpdateRegistry(bool reg)
{
	if (_hModule == 0)
	{
		return ERROR_INVALID_HANDLE;
	}

	WCHAR szFilename[1024];
	DWORD ccFilenameLength = GetModuleFileNameW(_hModule, szFilename, _countof(szFilename) - 2);
	if (ccFilenameLength == 0)
	{
		DWORD err = GetLastError();
		if (err == ERROR_SUCCESS)
			err = ERROR_UNIDENTIFIED_ERROR;
		return err;
	}
	if (ccFilenameLength >= (_countof(szFilename) - 3))
	{
		return ERROR_INSUFFICIENT_BUFFER;
	}
	// len == strlen(fn)
	szFilename[ccFilenameLength] = 0;
	szFilename[ccFilenameLength + 1] = 0; // double null terminated

	HKEY _hkey;
	LSTATUS ret = RegCreateKeyExW(HKEY_LOCAL_MACHINE, AUTHSRV_PARAMETERS_KEY_W, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &_hkey, NULL);
	if (ret != ERROR_SUCCESS)
		return ret;

	HKEYContainer hkey = { _hkey };

	DWORD valueType;
	DWORD valueSize = 0;
	LPBYTE pData = NULL;
	ret = RegQueryValueExW(hkey, AUTHSRV_EXTENSIONS_VALUE_W, 0, &valueType, NULL, &valueSize);

	if (ret == ERROR_SUCCESS)
	{
		if (valueType != REG_MULTI_SZ)
		{
			return ERROR_INVALID_PARAMETER;
		}
		int dataSize = valueSize + (ccFilenameLength + 2) * sizeof(WCHAR);
		pData = (LPBYTE)alloca(dataSize); // extra
		ret = RegQueryValueExW(hkey, AUTHSRV_EXTENSIONS_VALUE_W, 0, &valueType, pData, &valueSize);
		if (ret != ERROR_SUCCESS)
		{
			return ret;
		}

		if (valueSize % sizeof(WCHAR) != 0)
		{
			return ERROR_INVALID_DATA;
		}

		if (valueSize == 0)
		{
			return ERROR_BAD_LENGTH;
		}

		LPWSTR szFile = wcsrchr(szFilename, L'\\');
		if (!szFile) szFile = szFilename;
		size_t ccFile = wcslen(szFile);

		size_t mszValueLength = valueSize / sizeof(WCHAR);
		size_t mszOffset = 0;
		LPWSTR pSz = (LPWSTR)pData;

		size_t mszFileOffset = 0;
		bool foundFile = false;
		while (1)
		{
			// 0 1 2 3 4 5 6
			// a 0 b 0 c 0 0
			size_t mszEntryLength = wcslen(pSz + mszOffset);
			if (mszEntryLength == 0)
			{
				if (mszOffset == mszValueLength - 1)
					break;
				return ERROR_INVALID_DATA;
			}

			if (ccFile <= mszEntryLength && _wcsicmp(szFile, pSz + mszOffset + (mszEntryLength - ccFile)) == 0)
			{
				mszFileOffset = mszOffset;
				foundFile = true;
			}

			mszOffset += mszEntryLength + 1;
			if (mszOffset >= mszValueLength)
			{
				return ERROR_INVALID_DATA;
			}
		}

		if (reg)
		{
			if (foundFile)
			{
				return ERROR_DUP_NAME;
			}

			memcpy_s(pData + valueSize - sizeof(WCHAR), dataSize, szFilename, (ccFilenameLength + 2) * sizeof(WCHAR));
			valueSize += (ccFilenameLength + 1) * sizeof(WCHAR);
		}
		else
		{
			if (!foundFile)
			{
				return ERROR_PATH_NOT_FOUND;
			}

			size_t ccEntryToBeRemoved = wcslen(pSz + mszFileOffset);
			memmove(pSz + mszFileOffset, pSz + mszFileOffset + ccEntryToBeRemoved + 1, (mszValueLength - (mszFileOffset + ccEntryToBeRemoved)) * sizeof(WCHAR));
			valueSize -= (DWORD) ( (ccEntryToBeRemoved + 1) * sizeof(WCHAR));
		}
	}
	else if (ret == ERROR_FILE_NOT_FOUND)
	{
		if (!reg)
		{
			return S_OK;
		}
		pData = (LPBYTE)szFilename;
		valueSize = (ccFilenameLength + 2) * sizeof(WCHAR);
	}
	else
	{
		return ret;
	}

	ret = RegSetValueExW(hkey, AUTHSRV_EXTENSIONS_VALUE_W, 0, REG_MULTI_SZ, pData, valueSize);
	return ret;
}

STDAPI DllRegisterServer(void)
{
#pragma comment(linker, "/EXPORT:" __FUNCTION__"=" __FUNCDNAME__ ",PRIVATE")
	//DebugBreak();
	DWORD err = UpdateRegistry(true);
	if (err)
		return HRESULT_FROM_WIN32(err);
	return S_OK;
}

STDAPI DllUnregisterServer(void)
{
#pragma comment(linker, "/EXPORT:" __FUNCTION__"=" __FUNCDNAME__ ",PRIVATE")
	DWORD err = UpdateRegistry(false);
	if (err)
		return HRESULT_FROM_WIN32(err);
	return S_OK;
}
