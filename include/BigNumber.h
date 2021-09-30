#pragma once

#include <cmath>
#include <cstring>
#include <iostream>

#define sizeInChar(strSize) (2 * static_cast<int>(std::ceil((strSize * std::log2(10) + 1) / 16)))
#define sizeInStr(cSize) (static_cast<int>(std::round((cSize * 16 - 1) * std::log10(2))))
using std::cout;
using std::endl;

class BigNumber {
 public:
  BigNumber(std::string s);
  BigNumber(long long l);
  explicit BigNumber(int l, const char *s);
  explicit BigNumber(char *s, int l);
  ~BigNumber() {
    if (_m_data) delete _m_data;
  }

  BigNumber(const BigNumber &b) : _m_size(b._m_size) {
    cout << "call copy constructor  " << endl;
    _m_data = new char[sizeInChar(_m_size)];
    std::strncpy(_m_data, b._m_data, sizeInChar(_m_size));
  }

  BigNumber(BigNumber &&b) : _m_size(b._m_size), _m_data(b._m_data) {
    cout << "call move constructor  " << endl;
    b._m_data = nullptr;
  }

  BigNumber &operator=(const BigNumber &b) {
    cout << "call assignment constructor  " << endl;
    if (this != &b) {
      _m_size = b._m_size;
      _m_data = new char[sizeInChar(_m_size)];
      std::strncpy(_m_data, b._m_data, sizeInChar(b._m_size));
    }
    return *this;
  }

  BigNumber &operator=(BigNumber &&b) {
    cout << "call move assignment constructor  " << endl;
    if (this != &b) {
      _m_size = b._m_size;
      _m_data = b._m_data;
      b._m_data = nullptr;
    }
    return *this;
  }

  bool operator==(const BigNumber &b) { return (_m_size == b._m_size && std::strcmp(_m_data, b._m_data) == 0); }

  bool ConvBcdToBigNumHelper(int sourceLength, int targetLength, char *sourceData, char *targetData);
  std::string ConvBigNumToBcdHelper() const;

  friend std::ostream &operator<<(std::ostream &os, const BigNumber &number) {
    os << number.ConvBigNumToBcdHelper();
    return os;
  }

  BigNumber operator+(const BigNumber &b) { return AddHelper(b); }

  // BigNumber operator-(const BigNumber &b) { return SubHelper(b); }

  // BigNumber operator*(const BigNumber &b) { return MulHelper(b); }

  BigNumber AddHelper(const BigNumber &b);
  // BigNumber SubHelper(const BigNumber &b);
  // BigNumber MulHelper(const BigNumber &b);
  // static short DivHelper(int dividendLength, int divisorLength, char *dividendData, char *divisorData,
  //                        char *quotientData, char *tempData);

 private:
  char *_m_data;
  int _m_size;
};