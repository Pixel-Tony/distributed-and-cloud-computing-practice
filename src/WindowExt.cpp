#include "WindowExt.hpp"
#include "Image.hpp"

#ifdef _WIN32
    #define _WIN32_FILEDIALOG_IMPL
    #include <codecvt>
    #include <Windows.h>
    #include <shobjidl.h>

    #include <string.h>
    #include <stringapiset.h>
#endif


namespace WindowExt {

    inline bool sanitize(wchar_t *input, std::string &utf8dest) {
        utf8dest.resize(wcslen(input)*2);
        return stbi_convert_wchar_to_utf8((char *)utf8dest.data(), utf8dest.length(), input);
    }


    bool showFileDialog(sf::Window &window, std::string &filePath) {

#ifdef _WIN32_FILEDIALOG_IMPL
        bool success{ false };
        HRESULT hr;

        hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (SUCCEEDED(hr)) {
            IFileOpenDialog *pFileOpen;

            // Create the FileOpenDialog object.
            hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void **>(&pFileOpen));
            if (SUCCEEDED(hr)) {
                // Show the Open dialog box.
                hr = pFileOpen->Show(window.getSystemHandle());

                // Get the file name from the dialog box.
                if (SUCCEEDED(hr)) {

                    IShellItem *pItem;
                    hr = pFileOpen->GetResult(&pItem);

                    if (SUCCEEDED(hr)) {

                        PWSTR pszFilePath;
                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                        if (SUCCEEDED(hr)) {
                            success = sanitize(pszFilePath, filePath);
                            CoTaskMemFree(pszFilePath);
                        }
                        pItem->Release();
                    }
                }
                pFileOpen->Release();
            }
            CoUninitialize();
        }
        return success;

#endif /* _WIN32_FILEDIALOG_IMPL */


#ifdef X11
        return false;
#endif /* X11 */
    }

}
