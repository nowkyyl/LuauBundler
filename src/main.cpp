<<<<<<< HEAD
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "Bundler.h"
#include "LuauBundler.h"
#include "Utils.h"
=======
#include <iostream>
#include <fstream>
#include <chrono>
#include <filesystem>
#include "Bundler.h"
#include "LuauBundler.h"
>>>>>>> 94010ad73578bb307b1f78f75b7a9dbc3beec43c

namespace fs = std::filesystem;

int main(int argc, char const *argv[])
{
<<<<<<< HEAD
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <entry_file.lua> <output.lua> <threads>" << std::endl;
=======
    if (argc < 3)
    {
        std::cerr << "Usage: <entry_file.lua> <output.lua>" << std::endl;
>>>>>>> 94010ad73578bb307b1f78f75b7a9dbc3beec43c
        return 1;
    }

    try
    {
<<<<<<< HEAD
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
=======
        auto start = std::chrono::high_resolution_clock::now();
        fs::path entryFile(argv[1]);

        Bundler Bundler(entryFile);
        Bundler.bundle();

        const auto &modules = Bundler.getModules();
        std::string entryModule = entryFile.filename().string();

        LuauBundler LuauBundler(modules, entryModule);
        std::string bundledContent = LuauBundler.generate();
>>>>>>> 94010ad73578bb307b1f78f75b7a9dbc3beec43c

        fs::path outputFile(argv[2]);
        std::ofstream out(outputFile);
        if (!out.is_open())
        {
            std::cerr << "Failed to open output file: " << outputFile << std::endl;
            return 1;
        }

        out << bundledContent;
        out.close();
<<<<<<< HEAD

        std::cout << "Bundled output written to: " << outputFile << std::endl;

        auto now = std::chrono::high_resolution_clock::now();
        std::cout << "Bundling completed in "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count()
=======
        std::cout << "Bundled output written to: " << outputFile << std::endl;

        auto now = std::chrono::high_resolution_clock::now();
        std::cout << "Bundling completed in " << (std::chrono::duration_cast<std::chrono::milliseconds>(now - start)).count()
>>>>>>> 94010ad73578bb307b1f78f75b7a9dbc3beec43c
                  << " ms." << std::endl;
    }
    catch (const std::exception &e)
    {
<<<<<<< HEAD
        std::cerr << e.what() << std::endl;
=======
        std::cerr << e.what() << '\n';
>>>>>>> 94010ad73578bb307b1f78f75b7a9dbc3beec43c
        return 1;
    }

    return 0;
}
