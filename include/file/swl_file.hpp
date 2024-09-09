#pragma once
#ifndef SWL_FILE_HPP
#define SWL_FILE_HPP

#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <stdexcept>
#include <array>

namespace fs = std::filesystem;

namespace swl {
    // here's what a file class should represent:
    // - filename
    // - file contents
    // here's what a file class should be able to do:
    // - allow access to filename
    // - allow access to file contents
    // - allow read and write from disk
    // if i have more ideas i can put them here later

    class basic_file {
    protected:
        void init() {
            mFilesize = 0;
            mSeekpos = 0;
        }
    public:
        basic_file() = default;
        basic_file(const char* filename) 
            : mFilename(filename) 
        {
            init();
        }

        basic_file(const std::string& filename)
            : mFilename(filename)
        {
            init();
        }

        virtual ~basic_file() = default;

        /* virtual functions */

        // returns true if the file can be used
        virtual bool valid() const = 0;

        // returns true if the file exists on disk
        virtual bool exists() const = 0;

        /* non-virtual functions */
        const std::string& filename() const {
            return mFilename;
        }

        const size_t filesize() const {
            return mFilesize;
        }

        const size_t tell() const {
            return mSeekpos;
        }

        void seek(size_t pos) {
            // validate operation
            if (mFilesize < pos) {
                mSeekpos = pos;
            }
        }

    protected:
        std::string mFilename;
        size_t mFilesize;
        size_t mSeekpos;
    };

    // // a file with a fixed size
    // template <size_t size>
    // class fixed_file : public basic_file {
    // public:
    //     // unfinished
    // private:
    //     std::array<char, size> mData;
    // };

    
}


#endif // SWL_FILE_HPP