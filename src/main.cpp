/// @file main.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.

#include <iostream>
#include <llvm/Support/CommandLine.h>
#include <labo/debug/Log.h>

namespace labo::cl {
using namespace std;
using namespace llvm::cl;

/// List used commandline options here.
const vector<const OptionCategory*> related_categories{};
};

int
main(int argc, char* argv[])
{
    // Command line stuff. It's magic.
    llvm::cl::HideUnrelatedOptions(labo::cl::related_categories);
    llvm::cl::ParseCommandLineOptions(argc, argv);

    std::cout << "Hello world!" << std::endl;
    labo::logs << "Hi" << std::endl;
    labo::failure();
    return 0;
}