#include "BigNumber.h"

#include <limits.h>

#include <cmath>
#include <regex>
#include <stdexcept>

static const unsigned short powersOfTen[] = {10,      // 10^1
                                             100,     // 10^2
                                             1000,    // 10^3
                                             10000};  // 10^4

#define log2(m) (log(m) / log(2))

BigNumber::BigNumber(std::string s) {
  auto strSize = s.length();

  // check input
  std::regex r(R"((\-|\+)?\d+(\.\d+)?)");

  if (std::regex_match(s, r)) {
    _m_size = (2 * static_cast<int>(std::ceil((strSize * 0.208) + 0.062)));

    auto largestr = new char[strSize];
    _m_data = new char[_m_size];
    for (auto i = 0; i < strSize; i++) largestr[i] = s[i] - '0';

    ConvBcdToBigNumHelper(strSize, _m_size, largestr, _m_data);

    ConvBigNumToBcdHelper();
  } else
    throw std::runtime_error("Illegal input !!, The input may not be a normal numeric type. ");
}

BigNumber::BigNumber(long long l) : BigNumber(std::to_string(l)) {}
// BigNumber::BigNumber(long long l) { new (this) BigNumber(std::to_string(l)); }

BigNumber::BigNumber(int l, char *s) : _m_size(l), _m_data(s) {}

bool BigNumber::ConvBcdToBigNumHelper(int sourceLength, int targetLength, char *sourceData, char *targetData) {
  auto targetLengthInShorts = targetLength / 2;

  auto i = 0;
  std::memset(targetData, 0, targetLength);
  auto targetDataInShorts = reinterpret_cast<unsigned short *>(targetData);
  auto finalTargetLengthInShorts = 1;

  auto zeros = 0;
  while ((zeros < sourceLength) && !sourceData[zeros]) zeros++;
  if (zeros == sourceLength) return 1;

  auto actualSourceLength = sourceLength - zeros;
  char *actualSourceData = sourceData + zeros;

  union {
    unsigned int temp;
    struct {
      unsigned short remainder;  // 余数
      unsigned short carry;      // 进位
    } tempParts;
  };

  int index = 0;
  for (i = 0; i < actualSourceLength; i += 4) {
    unsigned short temp1 = 0;
    auto j = 0;
    while ((j < 4) && (i + j < actualSourceLength)) {
      temp1 = temp1 * 10 + actualSourceData[i + j];
      j++;
    }
    unsigned short power = powersOfTen[j - 1];

    temp = ((unsigned int)targetDataInShorts[0]) * power + temp1;
    targetDataInShorts[0] = tempParts.remainder;
    for (j = 1; j < finalTargetLengthInShorts; j++) {
      temp = ((unsigned int)targetDataInShorts[j]) * power + tempParts.carry;
      targetDataInShorts[j] = tempParts.remainder;
    }
    if (tempParts.carry) {
      if (finalTargetLengthInShorts >= targetLengthInShorts) return -1;
      finalTargetLengthInShorts++;
      targetDataInShorts[finalTargetLengthInShorts - 1] = tempParts.carry;
    }

    index++;

    std::cout << "round " << i << " : ";
    std::cout << std::hex;
    for (int ii = 0; ii < index; ii++)
      std::cout << "DataInShorts[" << ii << "]"
                << " = " << targetDataInShorts[ii] << " ";
    std::cout << std::endl;
  }
  return 0;
}

std::string BigNumber::ConvBigNumToBcdHelper() const {
  auto targetLength = static_cast<int>(std::floor((_m_size / 2 - 0.062) / 0.208));
  auto targetData = new char[targetLength + 1];
  auto sourceLengthInShorts = _m_size / 2;
  auto sourceDataInShorts = reinterpret_cast<unsigned short *>(_m_data);

  auto tempSourceDataInShorts = new unsigned short[sourceLengthInShorts];
  std::copy(sourceDataInShorts, sourceDataInShorts + sourceLengthInShorts, tempSourceDataInShorts);

  std::memset(targetData, 0, targetLength + 1);
  char *finalTargetData = targetData + targetLength;

  auto actualSourceLengthInShorts = sourceLengthInShorts;
  while (!tempSourceDataInShorts[actualSourceLengthInShorts - 1] && actualSourceLengthInShorts > 0)
    actualSourceLengthInShorts--;
  if (!actualSourceLengthInShorts) {
    delete tempSourceDataInShorts;
    return "false";
  }

  union {
    unsigned int temp;
    unsigned short temp1[2];
  };

  unsigned short remainder = 0;

  auto finalTargetLength = 0;
  while ((actualSourceLengthInShorts != 1) || (tempSourceDataInShorts[actualSourceLengthInShorts - 1] >= 10000)) {
    int j = 0;
    for (j = actualSourceLengthInShorts - 1; j >= 0; j--) {
      temp1[0] = tempSourceDataInShorts[j];
      temp1[1] = remainder;

      tempSourceDataInShorts[j] = (unsigned short)(temp / 10000);
      remainder = (unsigned short)(temp % 10000);
    }
    if (!tempSourceDataInShorts[actualSourceLengthInShorts - 1]) actualSourceLengthInShorts--;

    for (j = 0; j < 4; j++) {
      if (finalTargetLength >= targetLength) {
        delete tempSourceDataInShorts;
        return "false";
      }
      finalTargetData--;
      finalTargetLength++;
      finalTargetData[0] = remainder % 10;
      remainder = remainder / 10;
    }
  }

  remainder = tempSourceDataInShorts[0];
  while (remainder) {
    if (finalTargetLength >= targetLength) {
      delete tempSourceDataInShorts;
      return "false";
    }
    finalTargetData--;
    finalTargetLength++;
    finalTargetData[0] = remainder % 10;
    remainder = remainder / 10;
  }

  delete tempSourceDataInShorts;

  for (auto i = 0; i < targetLength; i++) targetData[i] += '0';

  std::cout << targetData << std::endl;

  return targetData;
}

BigNumber BigNumber::AddHelper(const BigNumber &adder) {
  // Recast from bytes to unsigned shorts.
  auto len = std::max(_m_size, adder._m_size) + 1;
  auto data = new char[len];
  std::memset(data, 0, len);
  auto leftDataInShorts = reinterpret_cast<unsigned short *>(_m_data);
  auto rightDataInShorts = reinterpret_cast<unsigned short *>(adder._m_data);
  auto resultDataInShorts = reinterpret_cast<unsigned short *>(data);

  union {
    unsigned int temp;
    struct {
      unsigned short remainder;
      unsigned short carry;
    } tempParts;
  };

  tempParts.carry = 0;
  for (auto j = 0; j < len / 2; j++) {
    int a, b;
    if (j > _m_size / 2)
      a = 0;
    else
      a = leftDataInShorts[j];

    if (j > adder._m_size / 2)
      b = 0;
    else
      b = rightDataInShorts[j];
    temp = ((unsigned int)a) + b + tempParts.carry;
    resultDataInShorts[j] = tempParts.remainder;
  }

  return BigNumber(len, data);
}

BigNumber BigNumber::SubHelper(const BigNumber &suber) {
  // Recast from bytes to unsigned shorts.
  auto len = std::max(_m_size, suber._m_size) + 1;
  auto data = new char[len];
  std::memset(data, 0, len);
  auto leftDataInShorts = reinterpret_cast<unsigned short *>(_m_data);
  auto rightDataInShorts = reinterpret_cast<unsigned short *>(suber._m_data);
  auto resultDataInShorts = reinterpret_cast<unsigned short *>(data);

  // Check if left is smaller than right, and
  // if so, switch left with right.
  unsigned short *left = leftDataInShorts;
  unsigned short *right = rightDataInShorts;

  int neg = 0;
  int j = 0;
  for (j = 0; j < len; j++) {
    int a, b;
    if (j > _m_size / 2)
      a = 0;
    else
      a = leftDataInShorts[j];

    if (j > suber._m_size / 2)
      b = 0;
    else
      b = rightDataInShorts[j];

    if (a > b)
      neg = -1;
    else if (a < b)
      neg = 0;
  }

  if (neg == -1) {
    left = rightDataInShorts;
    right = leftDataInShorts;
  }

  short carry = 0;
  int temp;

  union {
    unsigned int temp1;
    struct {
      unsigned short remainder;
      unsigned short filler;
    } tempParts;
  };

  for (j = 0; j < len / 2; j++) {
    int a, b;
    if (j > _m_size / 2)
      a = 0;
    else
      a = leftDataInShorts[j];

    if (j > suber._m_size / 2)
      b = 0;
    else
      b = rightDataInShorts[j];
    temp = ((int)a) - b + carry;
    temp1 = temp + (int)UINT16_MAX + 1;  // Note that USHRT_MAX + 1 = 2^16.
    resultDataInShorts[j] = tempParts.remainder;
    carry = (temp < 0 ? -1 : 0);
    std::cout << " left:" << a << " right:" << b << " result: " << resultDataInShorts[j] << std::endl;
  }

  return BigNumber(len, data);
}

short BigNumber::MulHelper(int leftLength, int rightLength, char *leftData, char *rightData, char *resultData) {
  // Recast from bytes to unsigned shorts.
  auto resultLength = leftLength + rightLength;
  unsigned short *leftDataInShorts = (unsigned short *)leftData;
  unsigned short *rightDataInShorts = (unsigned short *)rightData;
  unsigned short *resultDataInShorts = (unsigned short *)resultData;

  // Set result to zero.
  for (int k = 0; k < resultLength / 2; k++) resultDataInShorts[k] = 0;

  int rightEndInShorts = rightLength / 2 - 1;
  while (!rightDataInShorts[rightEndInShorts] && rightEndInShorts >= 0) rightEndInShorts--;
  if (rightEndInShorts < 0) return 0;

  int leftEndInShorts = leftLength / 2 - 1;
  while (!leftDataInShorts[leftEndInShorts] && leftEndInShorts >= 0) leftEndInShorts--;
  if (leftEndInShorts < 0) return 0;

  union {
    unsigned int temp;
    struct {
      unsigned short remainder;
      unsigned short carry;
    } tempParts;
  };

  int pos;
  for (int j = 0; j <= rightEndInShorts; j++) {
    tempParts.carry = 0;
    pos = j;
    for (int i = 0; i <= leftEndInShorts; i++, pos++) {
      temp = ((unsigned int)leftDataInShorts[i]) * rightDataInShorts[j] + resultDataInShorts[pos] + tempParts.carry;
      resultDataInShorts[pos] = tempParts.remainder;
    }

    if (tempParts.carry) resultDataInShorts[pos] = tempParts.carry;
  }

  return 0;
}

// short BigNumber::DivHelper(int dividendLength, int divisorLength, char *dividendData, char *divisorData,
//                            char *quotientData, char *tempData) {
//   int dividendLengthInShorts = dividendLength / 2;
//   int divisorLengthInShorts = divisorLength / 2;
//   unsigned short *divisorDataInShorts = (unsigned short *)divisorData;
//   unsigned short *quotientDataInShorts = (unsigned short *)quotientData;

//   char *tempDividendData = tempData;
//   unsigned short *tempDividendDataInShorts = (unsigned short *)tempDividendData;
//   unsigned short *tempDivisorDataInShorts = tempDividendDataInShorts + dividendLengthInShorts + 1;
//   unsigned short *tempDivisorData1InShorts = tempDivisorDataInShorts + divisorLengthInShorts + 1;
//   char *tempDivisorData = (char *)tempDivisorDataInShorts;
//   char *tempDivisorData1 = (char *)tempDivisorData1InShorts;

//   if (divisorDataInShorts[divisorLengthInShorts - 1] <= UCHAR_MAX) {  // Note that UCHAR_MAX = 0xFF

//     tempDivisorData[0] = 0;
//     int i;
//     for (i = 0; i < divisorLength; i++) tempDivisorData[i + 1] = divisorData[i];
//     tempDivisorData[divisorLength + 1] = 0;

//     tempDividendData[0] = 0;
//     for (i = 0; i < dividendLength; i++) tempDividendData[i + 1] = dividendData[i];
//     tempDividendData[dividendLength + 1] = 0;

//   } else {
//     int i;
//     // Multiply divisorData by 1 and store in tempDivisorData.
//     for (i = 0; i < divisorLength; i++) tempDivisorData[i] = divisorData[i];
//     tempDivisorDataInShorts[divisorLengthInShorts] = 0;

//     // Multiply dividendData by 1 and store in tempDividendData.
//     for (i = 0; i < dividendLength; i++) tempDividendData[i] = dividendData[i];
//     tempDividendDataInShorts[dividendLengthInShorts] = 0;
//   }

//   // Set the quotient to zero.
//   int j = 0;
//   for (j = 0; j < dividendLengthInShorts; j++) quotientDataInShorts[j] = 0;

//   unsigned short q;

//   union {
//     unsigned int temp1;
//     unsigned short temp2[2];
//   };

//   union {
//     long temp3;
//     unsigned short temp4[4];
//   };

//   int m = dividendLengthInShorts - divisorLengthInShorts;
//   int dividendPosInShorts = dividendLengthInShorts;

//   for (j = 0; j <= m; j++) {
//     temp4[3] = 0;
//     temp4[2] = tempDividendDataInShorts[dividendPosInShorts];
//     temp4[1] = tempDividendDataInShorts[dividendPosInShorts - 1];
//     temp4[0] = tempDividendDataInShorts[dividendPosInShorts - 2];
//     temp2[1] = tempDivisorDataInShorts[divisorLengthInShorts - 1];
//     temp2[0] = tempDivisorDataInShorts[divisorLengthInShorts - 2];

//     q = (unsigned short)(temp3 / temp1);

//     if (q == 0) {
//       if (tempDividendDataInShorts[dividendLengthInShorts - j] == tempDivisorDataInShorts[divisorLengthInShorts - 1])

//         q = USHRT_MAX;  // Note that USHRT_MAX is the largest digit in base 2^16.
//     }

//     BigNumber::MulHelper(divisorLength + 2, divisorLength, tempDivisorData, (char *)&q, tempDivisorData1);

//     int neg = BigNumber::SubHelper(
//         divisorLength + 2, (char *)&tempDividendDataInShorts[dividendPosInShorts - divisorLengthInShorts],
//         tempDivisorData1, (char *)&tempDividendDataInShorts[dividendPosInShorts - divisorLengthInShorts]);

//     if (neg) {
//       q--;

//       BigNumber::SubHelper(divisorLength + 2, tempDivisorData,
//                            (char *)&tempDividendDataInShorts[dividendPosInShorts - divisorLengthInShorts],
//                            (char *)&tempDividendDataInShorts[dividendPosInShorts - divisorLengthInShorts]);
//     }

//     quotientDataInShorts[dividendPosInShorts - divisorLengthInShorts] = q;

//     dividendPosInShorts--;

//   }  // for j

//   for (j = 0; j < divisorLengthInShorts; j++) {
//     if (tempDividendDataInShorts[j] != 0) return 1;
//   }
//   return 0;
// }