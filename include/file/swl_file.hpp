#pragma once
#ifndef SWL_FILE_HPP
#define SWL_FILE_HPP

#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <stdexcept>
#include <array>

#include "swl_util.hpp"

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
        virtual void disk_write() = 0;
        virtual void disk_write(const char* filepath) = 0;
        virtual void disk_write(const std::string& filepath) = 0;
        virtual void buffer_write(const std::string& hex) = 0;
        // virtual void write_bytes(const char* raw, size_t size) = 0;

        /* non-virtual functions */
        const std::string& filename() const {
            return mFilename;
        }

        void rename(const std::string& filename) {
            mFilename = filename;
        }

        const size_t filesize() const {
            return mFilesize;
        }

        const size_t tell() const {
            return mSeekpos;
        }

        void seek(size_t pos) {
            mSeekpos = pos > mFilesize ? mFilesize - 1 : pos;
        }

        bool exists() const {
            return mFilepath.empty() ? false : fs::exists(mFilepath);
        }

        

    protected:
        std::string mFilename;
        std::string mFilepath;
        size_t mFilesize;
        size_t mSeekpos;
    };

    // a file with a fixed size
    template <size_t size>
    class fixed_file : public basic_file {
    public:
        fixed_file() = default;
        fixed_file(const char* filename)
            : basic_file(filename)
            , mData( { } )
        {
            mFilesize = size;
        }

        fixed_file(const std::string& filename)
            : basic_file(filename)
            , mData( { } )
        {
            mFilesize = size;
        }

        virtual ~fixed_file() = default;

        /* basic_file functions */
        virtual bool valid() const override {
            return true;
        }

        virtual void disk_write() override {
            if (mFilepath.empty()) {
                mFilepath = "./" + mFilename;
            }
            std::ofstream file(mFilepath, std::ios::binary);
            file.write(mData.data(), mFilesize);
            file.close();
        }

        virtual void disk_write(const char* filepath) override {
            mFilepath = filepath;
            std::ofstream file(mFilepath, std::ios::binary);
            file.write(mData.data(), mFilesize);
            file.close();
        }

        virtual void disk_write(const std::string& filepath) override {
            mFilepath = filepath;
            std::ofstream file(mFilepath, std::ios::binary);
            file.write(mData.data(), mFilesize);
            file.close();
        }

        virtual void buffer_write(const std::string& hex) override {
            if (0 != hex.size() % 2) {
                throw std::runtime_error("swl::fixed_file::buffer_write: hex string must be even in length");
            }
            
            std::vector<char> bytes = swl::hex_to_bytes(hex);

            for (size_t i = 0; mSeekpos < mFilesize && i < bytes.size(); i++, mSeekpos++) {
                mData[mSeekpos] = bytes[i];
            }
        }

        /* fixed_file functions */
        char& operator[](size_t index) const {
            if (index >= mFilesize) {
                throw std::out_of_range("swl::fixed_file::operator[]: index out of range");
            }

            return mData[index];
        }
    private:
        std::array<char, size> mData;
    };

    
}


#endif // SWL_FILE_HPP