#include "stdafx.h"
#include "fileutil.h"
#include "filename.h"

namespace FileUtil
{

bool RemoveReadOnly(LPCTSTR filename)
{
	DWORD dwFileAtts = ::GetFileAttributes(filename);
	if(dwFileAtts & FILE_ATTRIBUTE_READONLY)
	{
		dwFileAtts &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(filename, dwFileAtts);
		return true;
	}
	
	return false;
}

bool FileIsReadOnly(LPCTSTR filename)
{
	DWORD dwFileAttributes = ::GetFileAttributes(filename);
	return (dwFileAttributes & FILE_ATTRIBUTE_READONLY);
	
	// This code will check whether we really can change a file, but it feels dirty
	// to me so is currently disabled.
	//{
	//	if (SetFileAttributes(pathname,dwFileAttributes |FILE_ATTRIBUTE_READONLY ) == 0)
	//	{
	//		// write permission
	//		return true;
	//	}
	//	else
	//	{
	//		// no write permission -> restore File Attributes
	//		SetFileAttributes(pathname,dwFileAttributes);
	//		return false;
	//	}
	//}
}

bool CreateBackupFile(LPCTSTR path, LPCTSTR prefix, LPCTSTR extension)
{
	std::string backupname(path);
	
	if (prefix != NULL)
	{
		backupname = prefix + backupname;
	}

	if (extension != NULL)
	{
		backupname += extension;
	}
	else
	{
		backupname += ".bak";
	}

	return CopyFile(path, backupname.c_str(), FALSE) != 0;
}

/**
 * @param path Path buffer, must be at least MAX_PATH big...
 * @param folder Folder ID
 */
BOOL PNGetSpecialFolderPath (LPTSTR path, int folder)
{
    ITEMIDLIST *pidl;		// Shell Item ID List ptr
    IMalloc    *imalloc;	// Shell IMalloc interface ptr
    BOOL		result;		// Return value

    if (SHGetSpecialFolderLocation (NULL, folder, &pidl) != NOERROR)
        return FALSE;

    result = SHGetPathFromIDList (pidl, path);

    if (SHGetMalloc (&imalloc) == NOERROR) {
		imalloc->Free(pidl);
        imalloc->Release();
    }

    return result;
}

} // namespace File