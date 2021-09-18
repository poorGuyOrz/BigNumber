#pragma once

#include <iostream>

class BigNumber {
 public:
  BigNumber(std::string s);
  // BigNumber(long long l);
  ~BigNumber() {
    if (_m_data) delete _m_data;
  }

  bool ConvBcdToBigNumHelper(int sourceLength, int targetLength, char *sourceData, char *targetData);
  std::string ConvBigNumToBcdHelper() const;

  friend std::ostream &operator<<(std::ostream &os, const BigNumber &number){
    number.ConvBigNumToBcdHelper();
  }

 private:
  char *_m_data;
  int _m_size;
};