//
// Created by Tomáš Petříček on 01.10.2022.
//

#ifndef MCC_READER_HPP
#define MCC_READER_HPP

#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

namespace mcc::csv {
    class reader final {
        std::ifstream file_;
        std::filesystem::path path_;
        char delim_;
        std::string line_;

    public:
        explicit reader(std::filesystem::path  path, char delim)
                :path_(std::move(path)), delim_(delim)
        {
            if (!std::filesystem::exists(path_))
                throw std::invalid_argument("File does not exist");

            file_ = std::ifstream{path_.string()};

            if (!file_.is_open())
                throw std::runtime_error("Cannot open "+path_.string());
        }

        template<std::size_t n>
        void operator()(std::string (& data)[n])
        {
            // read lines
            std::getline(file_, line_);

            // clean
            line_.erase(std::remove(line_.begin(), line_.end(), '\r'), line_.end());

            // parse line
            std::stringstream ss(line_);
            ss.exceptions(std::ios::failbit);

            for (std::size_t i{0}; i<n; i++)
                std::getline(ss, data[i], delim_);
        }
    };
}

#endif //MCC_READER_HPP