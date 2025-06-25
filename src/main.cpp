#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "Bundler.h"
#include "LuauBundler.h"
#include "Utils.h"

namespace fs = std::filesystem;

int main(int argc, char const *argv[])
{
        if (argc < 4)
        {
                std::cerr << "Usage: " << argv[0] << " <entry_file.lua> <output.lua> <threads>" << std::endl;
                return 1;
        }

        try
        {
                int threadCount = std::atoi(argv[3]);
                if (threadCount < 1)
                {
                        std::cerr << "Invalid thread count: " << argv[3] << ". Must be >= 1." << std::endl;
                        return 1;
                }

                auto start = std::chrono::high_resolution_clock::now();

                fs::path entryFile(argv[1]);
                Bundler bundler(entryFile, threadCount);
                bundler.bundle();

                const auto &modules = bundler.getModules();
                std::string entryModule = bundler.getEntryModuleName();

                LuauBundler luaBundler(modules, entryModule);
                std::string bundledContent = luaBundler.generate();

                fs::path outputFile(argv[2]);
                std::ofstream out(outputFile);
                if (!out.is_open())
                {
                        std::cerr << "Failed to open output file: " << outputFile << std::endl;
                        return 1;
                }

                out << bundledContent;
                out.close();

                std::cout << "Bundled output written to: " << outputFile << std::endl;

                auto now = std::chrono::high_resolution_clock::now();
                std::cout << "Bundling completed in "
                          << std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count()
                          << " ms." << std::endl;
        }
        catch (const std::exception &e)
        {
                std::cerr << e.what() << std::endl;
                return 1;
        }

        return 0;
}
