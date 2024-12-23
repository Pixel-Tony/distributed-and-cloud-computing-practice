#include "dcc-common/fileDialog.hpp"


#ifndef _WIN32
#error "File dialogs are not currently implemented for platforms other than Win32"
#endif /* _WIN32 */

#include <Windows.h>
#include <shobjidl.h>
#include <codecvt>

namespace fileDialog {
	sf::String choose(const sf::WindowHandle parentHandle) {
		sf::String success;

		if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))) {

			// Create the FileOpenDialog object.
			if (IFileOpenDialog *pFileOpen;
				SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog,
										   reinterpret_cast<void **>(&pFileOpen)))
				) {

				auto f_types = std::vector<COMDLG_FILTERSPEC>{{L"Image files", L"*.png"}};
				if (SUCCEEDED(pFileOpen->SetFileTypes(f_types.size(), f_types.data()))) {

					// Show the Open dialog box as f_types child of current window.
					if (SUCCEEDED(pFileOpen->Show(parentHandle))) {

						// Get the file name from the dialog box.
						if (IShellItem *pItem; SUCCEEDED(pFileOpen->GetResult(&pItem))) {

							if (PWSTR pszFilePath; SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
								success = pszFilePath;

								CoTaskMemFree(pszFilePath);
							}
							pItem->Release();
						}
					}
				}
				pFileOpen->Release();
			}
			CoUninitialize();
		}
		return success;
	}
}
