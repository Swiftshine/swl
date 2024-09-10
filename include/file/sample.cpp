#include <iostream>
#include "file/swl_file.hpp"

int main() {    
    swl::fixed_file<0x10> file("test.bin");
    file.buffer_write("0xDEADBEEF");
    file.disk_write();
    return 0;
}