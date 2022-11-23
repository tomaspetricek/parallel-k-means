//
// Created by Tomáš Petříček on 04.10.2022.
//

#ifndef MCC_WRITER_HPP
#define MCC_WRITER_HPP

#include <fstream>
#include <filesystem>

namespace mcc::csv {
    class writer {
        std::ofstream file_;
        const char delim_;
        std::ios_base::openmode mode_{std::ios_base::out};

    public:
        explicit writer(const std::filesystem::path& path, char delim = ',')
                :delim_(delim)
        {
            file_ = std::ofstream{path.string(), mode_};

            if (!file_.is_open())
                throw std::runtime_error("Cannot open "+path.string());
        }

        void operator()(const std::string* data, std::size_t n)
        {
            char sep;
            for (int i{0}; i<n; i++) {
                sep = (i<n-1) ? delim_ : '\n';
                file_ << data[i] << sep;
            }
        }
    };
}

#endif //MCC_WRITER_HPP
