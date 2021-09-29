#pragma once

#include <cmath>
#include <cstring>
#include <iostream>

class BigNumber {
 public:
  BigNumber(std::string s);
  BigNumber(long long l);
  explicit BigNumber(int l, char *s);
  ~BigNumber() {
    if (_m_data) delete _m_data, _m_data = nullptr;
  }

  BigNumber(const BigNumber &b) : _m_size(b._m_size) { std::strncpy(_m_data, b._m_data, _m_size); }

  BigNumber(BigNumber &&b) : _m_size(b._m_size), _m_data(b._m_data) { b._m_data = nullptr; }

  BigNumber &operator=(const BigNumber &b) {
    if (this != &b) {
      _m_size = b._m_size;
      std::strncpy(_m_data, b._m_data, b._m_size);
    }
    return *this;
  }

  BigNumber &operator=(BigNumber &&b) {
    if (this != &b) {
      _m_size = b._m_size;
      _m_data = b._m_data;
      b._m_data = nullptr;
    }
    return *this;
  }

  bool operator==(const BigNumber &b) { return std::strcmp(_m_data, b._m_data) == 0; }

  bool ConvBcdToBigNumHelper(int sourceLength, int targetLength, char *sourceData, char *targetData);
  std::string ConvBigNumToBcdHelper() const;

  friend std::ostream &operator<<(std::ostream &os, const BigNumber &number) {
    os << number.ConvBigNumToBcdHelper();
    return os;
  }

  // BigNumber operator+(const BigNumber &b) { return AddHelper(b); }

  // BigNumber operator-(const BigNumber &b) { return SubHelper(b); }

  // BigNumber operator*(const BigNumber &b) { return MulHelper(b); }

  // BigNumber AddHelper(const BigNumber &b);
  // BigNumber SubHelper(const BigNumber &b);
  // BigNumber MulHelper(const BigNumber &b);
  // static short DivHelper(int dividendLength, int divisorLength, char *dividendData, char *divisorData,
  //                        char *quotientData, char *tempData);

 private:
  char *_m_data;
  double _m_size;
};