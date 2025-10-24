#include "io_utils.hpp"

#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <string>
#include <filesystem>

#include "../globals.hpp"
#include "../../external/tinyfiledialogs/tinyfiledialogs.h"

namespace fs = std::filesystem;

// this code is unbelievable bad.. NEEDS to be recoded
namespace hot_spotter::utils::io {
        std::string selectFolder() {
                const char *folder = tinyfd_selectFolderDialog("Select Folder", "");

                if (folder) {
                        return std::string(folder);
                }
                return "";
        }

        bool exportClasses() {
                fs::path homePath;
#ifdef _WIN32
                homePath = fs::path(std::getenv("USERPROFILE"));
#else
                homePath = fs::path(std::getenv("HOME"));
#endif

                if (homePath.empty()) {
                        return false;
                }

                auto now = std::chrono::system_clock::now();
                auto time_t_now = std::chrono::system_clock::to_time_t(now);
                std::tm tm_now;

#ifdef _WIN32
                gmtime_s(&tm_now, &time_t_now);
#else
                gmtime_r(&time_t_now, &tm_now);
#endif

                std::ostringstream timestr;
                timestr << std::put_time(&tm_now, "%Y-%m-%d-%H-%M-%S");
                // Create directory structure
                fs::path dirPath = selectFolder();

                if (std::error_code ec; !fs::create_directories(dirPath, ec) && !fs::exists(dirPath)) {
                        return false;
                }

                for (auto entry: classes) {
                        // Create full file path
                        std::string className = entry.first;
                        std::replace(className.begin(), className.end(), '/', '.');
                        className += ".class";

                        fs::path filePath = dirPath / className;

                        // Write data to file
                        std::ofstream outFile(filePath, std::ios::binary);
                        if (!outFile) {
                                continue;
                        }

                        outFile.write(reinterpret_cast<const char *>(entry.second.second.second),
                                      entry.second.second.first);
                        if (!outFile) {
                                continue;
                        }

                        outFile.close();
                }
                return true;
        }
}
