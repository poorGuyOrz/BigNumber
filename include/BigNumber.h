#pragma once

#include <iostream>

class BigNumber {
 public:
  BigNumber(std::string s);
  // BigNumber(long long l);
  ~BigNumber() {
    if (_m_data) delete _m_data;
  }

  static bool ConvBcdToBigNumHelper(int sourceLength, int targetLength, char *sourceData, char *targetData);
  static bool ConvBigNumToBcdHelper(int sourceLength, int targetLength, char *sourceData, char *targetData);

 private:
  char *_m_data;
};