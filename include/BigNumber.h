#pragma once

#include <cstring>
#include <iostream>

class BigNumber {
 public:
  BigNumber(std::string s);
  BigNumber(long long l);
  explicit BigNumber(int l, char *s);
  ~BigNumber() {
    if (_m_data) delete _m_data;
  }

  bool ConvBcdToBigNumHelper(int sourceLength, int targetLength, char *sourceData, char *targetData);
  std::string ConvBigNumToBcdHelper() const;

  friend std::ostream &operator<<(std::ostream &os, const BigNumber &number) {
    os << number.ConvBigNumToBcdHelper();
    return os;
  }

  BigNumber operator+(const BigNumber &b) { return AddHelper(b); }

  BigNumber operator-(const BigNumber &b) { return SubHelper(b); }

  BigNumber operator*(const BigNumber &b) {
    auto len = this->_m_size + b._m_size;
    auto data = new char[len];
    std::memset(data, 0, len);
    MulHelper(this->_m_size, b._m_size, this->_m_data, b._m_data, data);
    BigNumber nb("0");
    nb._m_data = data;
    nb._m_size = len;
    return nb;
  }

  /*
    add:
      Binary addition operation, directly add by bit
  */
  BigNumber AddHelper(const BigNumber &b);
  BigNumber SubHelper(const BigNumber &b);
  short MulHelper(int leftLength, int rightLength, char *leftData, char *rightData, char *resultData);
  // static short DivHelper(int dividendLength, int divisorLength, char *dividendData, char *divisorData,
  //                        char *quotientData, char *tempData);

 private:
  char *_m_data;
  int _m_size;
};