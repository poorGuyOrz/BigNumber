#pragma once

#include <cstring>
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

  friend std::ostream &operator<<(std::ostream &os, const BigNumber &number) {
    os << number.ConvBigNumToBcdHelper();
    return os;
  }

  friend BigNumber operator+(BigNumber &a, const BigNumber &b) {
    auto len = (a._m_size > b._m_size) ? a._m_size : b._m_size;
    auto data = new char[len + 1];
    std::memset(data, 0, len + 1);
    int lenr = 0;
    AddHelper(a._m_size, b._m_size, a._m_data, b._m_data, data, lenr);
    BigNumber nb("0");
    nb._m_data = data;
    nb._m_size = len;
    return nb;
  }

  friend BigNumber operator-(BigNumber &a, const BigNumber &b) {
    auto len = (a._m_size > b._m_size) ? a._m_size : b._m_size;
    auto data = new char[len + 1];
    std::memset(data, 0, len + 1);
    int lenr = 0;
    SubHelper(a._m_size, b._m_size, a._m_data, b._m_data, data);
    BigNumber nb("0");
    nb._m_data = data;
    nb._m_size = len;
    return nb;
  }

  /*
    add:
      Binary addition operation, directly add by bit
  */
  static short AddHelper(int len1, int len2, char *leftData, char *rightData, char *resultData, int &i);
  static short SubHelper(int len1, int len2, char *leftData, char *rightData, char *resultData);
  static short MulHelper(int resultLength, int leftLength, int rightLength, char *leftData, char *rightData,
                         char *resultData);

 private:
  char *_m_data;
  int _m_size;
};