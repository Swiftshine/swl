#pragma once
#ifndef SWL_UTIL_HPP
#define SWL_UTIL_HPP

#include <vector>
#include <string>
#include <stdexcept>

namespace swl {

    std::vector<char> hex_to_bytes(std::string hex) {
        if (0 != (hex.size() % 2)) {
            throw std::invalid_argument("swl::hex_to_bytes: hex string must be even in length");
        }

        // strip leading 0x
        if ('0' == hex[0] && 'x' == hex[1]) {
            hex = hex.substr(2);
        }

        std::vector<char> bytes(hex.length() >> 2);

        char byteStr[2];

        for (size_t i = 0; i < hex.length(); i += 2) {
            byteStr[0] = hex[i];
            byteStr[1] = hex[i + 1];

            bytes[i >> 2] = static_cast<char>(std::stoi(byteStr, nullptr, 16));
        }
        
        return bytes;
    }
}

#endif // SWL_UTIL_HPP