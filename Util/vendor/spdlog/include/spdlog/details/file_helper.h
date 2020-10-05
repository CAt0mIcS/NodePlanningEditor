//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

// Helper class for file sinks.
// When failing to open a file, retry several times(5) with a delay interval(10 ms).
// Throw spdlog_ex exception on errors.

#include "spdlog/details/log_msg.h"
#include "spdlog/details/os.h"

#include <cerrno>
#include <chrono>
#include <cstdio>
#include <string>
#include <thread>
#include <tuple>

#include <fstream>


namespace spdlog {
namespace details {

class file_helper
{

public:
    const int open_tries = 5;
    const int open_interval = 10;

    explicit file_helper() = default;

    file_helper(const file_helper &) = delete;
    file_helper &operator=(const file_helper &) = delete;

    ~file_helper()
    {
        close();
    }

    void open(const filename_t &fname, bool truncate = false)
    {
        close();
        auto *mode = truncate ? SPDLOG_FILENAME_T("wb") : SPDLOG_FILENAME_T("ab");
        _filename = fname;
        for (int tries = 0; tries < open_tries; ++tries)
        {
            if (!os::fopen_s(&fd_, fname, mode))
            {
                return;
            }

            details::os::sleep_for_millis(open_interval);
        }

        throw spdlog_ex("Failed opening file " + os::filename_to_str(_filename) + " for writing", errno);
    }

    void reopen(bool truncate)
    {
        if (_filename.empty())
        {
            throw spdlog_ex("Failed re opening file - was not opened before");
        }
        open(_filename, truncate);
    }

    void flush()
    {
        std::fflush(fd_);
    }

    void close()
    {
        if (fd_ != nullptr)
        {
            std::fclose(fd_);
            fd_ = nullptr;
        }
    }

    void write(const fmt::memory_buffer &buf)
    {
        size_t msg_size = buf.size();
        auto data = buf.data();
        
        //__MODIFIED__ __HIT__
        std::string str = data;

#ifdef SPDLOG_PRINT_TO_CONSOLE
        printf(str.c_str());
#endif

        size_t id = 0;
        id = str.find("[]");

        size_t idSlashR = 0;
        idSlashR = str.find("\r");

        if (idSlashR != std::string::npos)
        {
            str.replace(str.begin() + idSlashR, str.begin() + idSlashR + 1, "");
        }

        if (id == std::string::npos)
        {
            std::ofstream writer(os::filename_to_str(_filename), std::ios_base::app);
            writer << data;
            writer.close();

            //if (std::fwrite(data, 1, msg_size, fd_) != msg_size)
            //{
            //    throw spdlog_ex("Failed writing to file " + os::filename_to_str(_filename), errno);
            //}
        }
        else
        {
            str.replace(str.begin() + id, str.begin() + id + 3, "");

            std::ofstream writer(os::filename_to_str(_filename), std::ios_base::app);
            writer << str;
            writer.close();

            //if (std::fwrite(str.c_str(), 1, msg_size, fd_) != msg_size)
            //{
            //    throw spdlog_ex("Failed writing to file " + os::filename_to_str(_filename), errno);
            //}
        }
        
    }

    size_t size() const
    {
        if (fd_ == nullptr)
        {
            throw spdlog_ex("Cannot use size() on closed file " + os::filename_to_str(_filename));
        }
        return os::filesize(fd_);
    }

    const filename_t &filename() const
    {
        return _filename;
    }

    static bool file_exists(const filename_t &fname)
    {
        return os::file_exists(fname);
    }

    //
    // return file path and its extension:
    //
    // "mylog.txt" => ("mylog", ".txt")
    // "mylog" => ("mylog", "")
    // "mylog." => ("mylog.", "")
    // "/dir1/dir2/mylog.txt" => ("/dir1/dir2/mylog", ".txt")
    //
    // the starting dot in filenames is ignored (hidden files):
    //
    // ".mylog" => (".mylog". "")
    // "my_folder/.mylog" => ("my_folder/.mylog", "")
    // "my_folder/.mylog.txt" => ("my_folder/.mylog", ".txt")
    static std::tuple<filename_t, filename_t> split_by_extenstion(const spdlog::filename_t &fname)
    {
        auto ext_index = fname.rfind('.');

        // no valid extension found - return whole path and empty string as
        // extension
        if (ext_index == filename_t::npos || ext_index == 0 || ext_index == fname.size() - 1)
        {
            return std::make_tuple(fname, spdlog::filename_t());
        }

        // treat casese like "/etc/rc.d/somelogfile or "/abc/.hiddenfile"
        auto folder_index = fname.rfind(details::os::folder_sep);
        if (folder_index != filename_t::npos && folder_index >= ext_index - 1)
        {
            return std::make_tuple(fname, spdlog::filename_t());
        }

        // finally - return a valid base and extension tuple
        return std::make_tuple(fname.substr(0, ext_index), fname.substr(ext_index));
    }

private:
    std::FILE *fd_{nullptr};
    filename_t _filename;
};
} // namespace details
} // namespace spdlog