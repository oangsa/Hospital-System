#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;
namespace fs = filesystem;

void runCommand(const string& cmd) {
    cout << cmd << endl;
    int ret = system(cmd.c_str());
    if (ret != 0) {
        cerr << "Command failed: " << cmd << endl;
        exit(1);
    }
}

int main() {
    string features = "Features";
    string libs = "Libs";
    string mainSrc = "main.cpp";
    string buildDir = "builds";
    vector<string> objFiles;

    fs::create_directories(buildDir + "/Features");
    fs::create_directories(buildDir + "/Libs");

    // Compile All Files in Features
    for (const auto& entry : fs::directory_iterator(features)) {
        if (entry.path().extension() == ".cpp") {
            string base = entry.path().stem().string();
            string src = entry.path().string();
            string obj = buildDir + "/Features/" + base + ".o";
            string dep = buildDir + "/Features/" + base + ".d";
            string cmd = "g++ -std=c++17 -c \"" + src + "\" -o \"" + obj +
                              "\" -MMD -MF \"" + dep + "\" -IFeatures -ILibs";
            runCommand(cmd);
            objFiles.push_back("\"" + obj + "\"");
        }
    }

    // Compile All File in Libs
    for (const auto& entry : fs::directory_iterator(libs)) {
        if (entry.path().extension() == ".cpp") {
            string base = entry.path().stem().string();
            string src = entry.path().string();
            string obj = buildDir + "/Libs/" + base + ".o";
            string dep = buildDir + "/Libs/" + base + ".d";
            string cmd = "g++ -std=c++17 -c \"" + src + "\" -o \"" + obj +
                              "\" -MMD -MF \"" + dep + "\" -IFeatures -ILibs";
            runCommand(cmd);
            objFiles.push_back("\"" + obj + "\"");
        }
    }

    // Compile main.cpp
    string mainObj = buildDir + "/main.o";
    string mainDep = buildDir + "/main.d";
    string cmd = "g++ -std=c++17 -c \"" + mainSrc + "\" -o \"" + mainObj +
                      "\" -MMD -MF \"" + mainDep + "\" -IFeatures -ILibs";
    runCommand(cmd);
    objFiles.push_back("\"" + mainObj + "\"");

    // Linker
    string output = "main";
    string linkCmd = "g++ ";
    for (const auto& obj : objFiles) linkCmd += obj + " ";
    linkCmd += "-o " + output;
    runCommand(linkCmd);

    cout << "Done" << endl;
    return 0;
}
