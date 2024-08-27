#pragma once
#ifndef SWL_FILEDIALOG_HPP
#define SWL_FILEDIALOG_HPP

#include <string>
#include <vector>
#include <utility>

namespace swl {
    class file_dialog {
    public:

        enum dialog_flags {
            none            = 0,
            file_must_exist = 1,
        };

        class filter_list {
        public:
            void add_filter(const char* description, const char* extension) {
                mFilters.emplace_back(std::make_pair(description, extension));
            }

            std::string get() const;
        private:
            using filter_description = std::string;
            using filter_extension = std::string;

            std::vector<std::pair<filter_description, filter_extension>> mFilters;
        };
    public:
        static std::string open_file_dialog(const char* prompt_name, const filter_list* filters, const int flags = 0);
    };
}

// Windows Implementation

#if defined(_WIN32)

#include <windows.h>
#include <commdlg.h>

std::string swl::file_dialog::filter_list::get() const {
    std::string result = "";

    for (auto& filter : mFilters) {
        result += filter.first;
        result += '\0';
        result += filter.second;
        result += '\0';
    }

    return result;    
}

std::string swl::file_dialog::open_file_dialog(const char* prompt_name, const filter_list* filters, const int flags) {
    char buf[MAX_PATH] = "";

    // this structure holds file dialog information
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;

    ofn.lpstrFile = reinterpret_cast<LPTSTR>(buf);

    ofn.nMaxFile = MAX_PATH;

    if (filters) {
        ofn.lpstrFilter = reinterpret_cast<LPTSTR>(filters->get().data());
    } else {
        // all by default
        ofn.lpstrFilter = (LPTSTR)"All\0*.*\0";
    }
    ofn.nFilterIndex = 1; // default filter index
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrInitialDir = NULL;

    if (flags & dialog_flags::file_must_exist) {
        ofn.Flags |= OFN_FILEMUSTEXIST;
    }

    if (GetOpenFileName(&ofn)) {
        return std::string(buf);
    }

    return "";
}

#endif // _WIN32

#endif // SWL_FILEDIALOG_HPP