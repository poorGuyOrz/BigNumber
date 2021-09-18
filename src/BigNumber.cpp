#include "BigNumber.h"

#include <cmath>
#include <cstring>
#include <regex>
#include <stdexcept>

static const unsigned short powersOfTen[] = {10,      // 10^1
                                             100,     // 10^2
                                             1000,    // 10^3
                                             10000};  // 10^4

#define log2(m) (log(m) / log(2))

BigNumber::BigNumber(std::string s) {
  auto strSize = s.length();

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

bool BigNumber::ConvBcdToBigNumHelper(int sourceLength, int targetLength, char *sourceData, char *targetData) {
  auto targetLengthInShorts = targetLength / 2;

  auto i = 0;
  std::memset(targetData, 0, targetLength);
  auto targetDataInShorts = reinterpret_cast<unsigned short *>(targetData);
  auto finalTargetLengthInShorts = 1;

  // Ignore leading zeros in BCD. If all zeros, return.
  auto zeros = 0;
  while ((zeros < sourceLength) && !sourceData[zeros]) zeros++;
  if (zeros == sourceLength) return 1;  // indicate that it is all zeros

  auto actualSourceLength = sourceLength - zeros;
  char *actualSourceData = sourceData + zeros;

  union {
    unsigned int temp;
    struct {
      unsigned short remainder;  // 余数
      unsigned short carry;      // 进位
    } tempParts;
  };

  // Compute the Big Num as follows. First, chunk up the BCD
  // string into chunks of 4-digit unsigned short numbers. Iterate
  // over these chunks, and at each iteration, multiply the current
  // Big Num by 10^4 and then add the numeric value of this chunk
  // to it.
  int index = 0;
  for (i = 0; i < actualSourceLength; i += 4) {
    // Compute the numeric value of the next 4-digit chunk.
    unsigned short temp1 = 0;
    auto j = 0;
    while ((j < 4) && (i + j < actualSourceLength)) {
      temp1 = temp1 * 10 + actualSourceData[i + j];
      j++;
    }
    unsigned short power = powersOfTen[j - 1];

    // Multiply the previous Big Num by 10^4 and add the value
    // of the current chunk to it. It is more efficient to insert the
    // multiplication and addition code here than to call the Helper()
    // methods.
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
    for (int ii = 0; ii < index; ii++)
      std::cout << "DataInShorts[" << ii << "]"
                << " = " << targetDataInShorts[ii] << " ";
    std::cout << std::endl;
  }

  return 0;
}

bool BigNumber::ConvBigNumToBcdHelper() {
  auto targetLength = static_cast<int>(std::floor((_m_size / 2 - 0.062) / 0.208));
  // std::cout <<_m_size << " xx " << xx << " targetLength: " << targetLength << std::endl;
  auto targetData = new char[targetLength];
  auto sourceLengthInShorts = _m_size / 2;
  unsigned short *sourceDataInShorts = (unsigned short *)_m_data;

  unsigned short tempSourceDataInShortsBuf[128 / 2];
  unsigned short *tempSourceDataInShorts = tempSourceDataInShortsBuf;

  tempSourceDataInShorts = new unsigned short[_m_size / 2];

  auto i = 0;
  for (i = 0; i < sourceLengthInShorts; i++) tempSourceDataInShorts[i] = sourceDataInShorts[i];

  // Initialize the BCD to zero.
  for (i = 0; i < targetLength; i++) targetData[i] = 0;
  char *finalTargetData = targetData + targetLength - 1;
  auto finalTargetLength = 1;

  // Ignore trailing zeros in the Big Num. If all zeros, return.
  auto actualSourceLengthInShorts = sourceLengthInShorts;
  while (!tempSourceDataInShorts[actualSourceLengthInShorts - 1] && actualSourceLengthInShorts > 0)
    actualSourceLengthInShorts--;
  if (!actualSourceLengthInShorts) {
    delete tempSourceDataInShorts;
    return false;
  }

  union {
    unsigned int temp;
    unsigned short temp1[2];
  };

  unsigned short remainder = 0;

  // Compute the final BCD as follows. Keep dividing the Big Num by 10^4
  // until the quotient becomes less than 10^4. At each iteration, compute
  // the 4-digit BCD representation of the remainder, and append it to the
  // left of the final BCD. For the final remainder, compute its BCD
  // representation (which may be less than 4-digits) and append it to the
  // left of the final BCD.

  finalTargetData++;
  finalTargetLength = 0;
  while ((actualSourceLengthInShorts != 1) || (tempSourceDataInShorts[actualSourceLengthInShorts - 1] >= 10000)) {
    // Divide the Big Num by 10^4. It is more efficient to insert
    // the division code than to call SimpleDivideHelper();
    int j = 0;
    for (j = actualSourceLengthInShorts - 1; j >= 0; j--) {
      temp1[0] = tempSourceDataInShorts[j];
      temp1[1] = remainder;

      tempSourceDataInShorts[j] = (unsigned short)(temp / 10000);
      remainder = (unsigned short)(temp % 10000);
    }
    if (!tempSourceDataInShorts[actualSourceLengthInShorts - 1]) actualSourceLengthInShorts--;

    // Compute the BCD representation of the remainder and append to the
    // left of the final BCD.
    for (j = 0; j < 4; j++) {
      if (finalTargetLength >= targetLength) {
        delete tempSourceDataInShorts;
        return false;
      }
      finalTargetData--;
      finalTargetLength++;
      finalTargetData[0] = remainder % 10;
      remainder = remainder / 10;
    }
  }

  // Compute the BCD representation of the final remainder and append to the
  // left of the final BCD.
  remainder = tempSourceDataInShorts[0];
  while (remainder) {
    if (finalTargetLength >= targetLength) {
      delete tempSourceDataInShorts;
      return -1;
    }
    finalTargetData--;
    finalTargetLength++;
    finalTargetData[0] = remainder % 10;
    remainder = remainder / 10;
  }

  delete tempSourceDataInShorts;

  for (auto i = 0; i < targetLength; i++) targetData[i] += '0';

  std::cout << targetData << std::endl;

  return 0;
}
