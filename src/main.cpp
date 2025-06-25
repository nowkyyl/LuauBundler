#include <iostream>
#include <fstream>
#include <chrono>
#include <filesystem>
#include "Bundler.h"
#include "LuauBundler.h"

namespace fs = std::filesystem;

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: <entry_file.lua> <output.lua>" << std::endl;
        return 1;
    }

    try
    {
        auto start = std::chrono::high_resolution_clock::now();
        fs::path entryFile(argv[1]);

        Bundler Bundler(entryFile);
        Bundler.bundle();

        const auto &modules = Bundler.getModules();
        std::string entryModule = entryFile.filename().string();

        LuauBundler LuauBundler(modules, entryModule);
        std::string bundledContent = LuauBundler.generate();

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
        std::cout << "Bundling completed in " << (std::chrono::duration_cast<std::chrono::milliseconds>(now - start)).count()
                  << " ms." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
