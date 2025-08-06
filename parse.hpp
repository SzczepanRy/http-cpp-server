#include <cstring>
#include <string>
#include <iostream>
#pragma once
class Parse {
public:
  static void slice(const char *arr, char target[], int start, int end,
                    size_t targetSize) {
    int len = end - start;
    if (len < 0 || targetSize == 0)
      return;
    if ((size_t)len >= targetSize)
      len = targetSize - 1;

    for (int i = 0; i < len; ++i) {
      target[i] = arr[start + i];
    }
    target[len] = '\0';
  }

  static void getValueBefChar(char arr[], char target[], char sep) {
    int i;
    for (i = 0; arr[i] != '\0'; i++) {
      if (arr[i] == sep) {
        target[i] = '\0';
        return;
      } else {
        target[i] = arr[i];
      }
    }
  }

  static void spilt(char arr[], int size, char target[], size_t targetSize,
                    char split, int index) {
    int last = 0;
    int tempI = 0;

    for (int i = 0; i < size && arr[i] != '\0'; i++) {
      if (arr[i] == split) {
        if (index == tempI) {
          Parse::slice(arr, target, last, i, targetSize);
          return;
        }
        tempI++;
        last = i + 1;
      }
    }

    // If delimiter not found enough times, maybe take last part
    if (index == tempI) {
      Parse::slice(arr, target, last, size, targetSize);
    }
  }
  static void getLast(char arr[], int size, char target[], size_t targetSize,
                      char split) {
    int lastIndex = -1;

    for (int i = size - 1; i >= 0; --i) {
      if (arr[i] == split) {
        lastIndex = i + 1;
        break;
      }
    }

    if (lastIndex != -1) {
      Parse::slice(arr, target, lastIndex, size, targetSize);
    } else {
      Parse::slice(arr, target, 0, size, targetSize);
    }
  }

  static void copyCharArr(const char src[], char dest[], size_t maxSize) {
    if (maxSize == 0)
      return;
    size_t i;
    for (i = 0; i < maxSize - 1 && src[i] != '\0'; ++i) {
      dest[i] = src[i];
    }
    dest[i] = '\0';
  }

  static void clearSpaces(char arr[], const int size) {

    char *array = new char[size];
    int last = 0;
    for (int i = 0; i <= size; i++) {
      if (arr[i] != ' ') {
        array[last] = arr[i];
        last++;
      }
      if (arr[i] == '\0') {
        arr[i] = '\0';
      }
    }

    for (int i = 0; i <= size; i++) {
      arr[i] = array[i];
    }
  }
  static bool compareArrays(char arr1[], char arr2[]) {
    if (len(arr1) != len(arr2)) {
      return false;
    }
    for (int i = 0; arr1[i] != '\0'; i++) {
      if (arr2[i] != arr1[i]) {
        return false;
      }
    }
    return true;
  }
  static int len(char arr1[]) {
    int i;
    for (i = 0; arr1[i] != '\0'; i++) {
    }

    return i;
  }

  static bool checkStr(std::string str, std::string search, size_t size,
                       size_t searchSize) {
    for (int i = 0; i < size - searchSize; i++) {
      bool isGood = true;
      for (int j = 0; j < searchSize; j++) {
        if (str.c_str()[i + j] != search.c_str()[j]) {
          isGood = false;
        }
        //  std::cout<< str.c_str()[i+j] <<" "<<
      }
      if (isGood) {
        return true;
      }
    }
    return false;
  }

  static void findJsonBody(std::string req) {
    std::cout << "find first : " << req.find_first_of("{")
              << "find last off : " << req.find_last_of("}") << "\n";
  }
};
