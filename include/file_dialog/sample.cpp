#include <iostream>
#include "file_dialog/swl_file_dialog.hpp"

int main() {
    std::cout << "File Dialog: " << swl::file_dialog::open_file_dialog("File Dialog", new swl::file_dialog::filter_list(), swl::file_dialog::dialog_flags::none) << std::endl;
    return 0;
}