#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

#pragma once

class File {
public:
  static std::vector<char> FileTocharArr(std::string path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
      std::cerr << "faled to one file.\n";
      return {};
    }
    std::vector<char> target((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());

    //    delete[] target;
    return target;
  }
};
