#include <iostream>

#include "BigNumber.h"

int main() {
  // BigNumber a("65535");
  BigNumber b("10100");
  BigNumber a("10100");
  // BigNumber c = a + b;
  // BigNumber d = a - b;
  // BigNumber e = a * b;

  std::cout << (a == b) << std::endl;
}

/*
code clean last!!!

round 44 : DataInShorts[0] = 16997 DataInShorts[1] = 40531 DataInShorts[2] = 63104 DataInShorts[3] = 48460
DataInShorts[4] = 65414 DataInShorts[5] = 28978 DataInShorts[6] = 57736 DataInShorts[7] = 34413 DataInShorts[8] = 25764
DataInShorts[9] = 44 DataInShorts[10] = 0 DataInShorts[11] = 0 round 0 : DataInShorts[0] = 1

storage length = ((precision*log2(10) + 1)/16)

1        2
2        2
3        2
4        2
5        4
6        4
7        4
8        4
9        2
10       3


123456
1 11100010 01000000
1 E2 40
1 57920

123456 / 65536 == 1 ... 57920
[0] = 57920
[1] = 1

123456789
111 01011011 11001101 00010101
7   5B       CD       15
1883         52501

123456789 / 65536 = 1883 ... 52501
1883 * 2^16 + 52501 = 123456789

65536
1 00000000 00000000
1 00       00

1 * 2^16 + 0 = 65536


65538 / 65536 == 1 ... 2
[0] = 2
[1] = 1
1 * 2^16 + 2 = 65538

6553 8
6553



18446744073709551616

123456789123456789

1 10110110 10011011 01001011 10101100 11010000 01011111 00010101
1 b6       9b       4b       ac       d0       5f       15
438        39755             44240             2431

123456789123456789 / 65536 == 1883801103568 .. 24341 = [0]
     1883801103568 / 65536 == 28744523      .. 44240 = [1]
          28744523 / 65536 == 438           .. 39755 = [2]
               438 / 65536 == 0             .. 438   = [3]

(((1234 * 10^4 + 5678) * 10^4 + 9123) * 10^4 + 4567) * 10^4 + 89
((438 * 2^16 + 39755) * 2^16 + 44240) * 2^16 + 24341
 (28744523 * 2^16 + 44240) * 2^16 + 24341
  1883801103568 * 2^16 + 24341
  123456789123456789

1234 5678 9123 4567 89

1234 [0] = 1234
12345678  10111100(188) 01100001 01001110(24910) [1] = 188 [0] = 24910


                              1234           5678     9123 4567 89
                              1234                                    r = 4D2  c = 0
                               4D2 * 10000 + 5678                     r = 614E c = BC
                                       614E * 10000 + 9123            r = 1A83 c = ED9




1234  ->                       [0] 4d2                                                    1234 % 2^16 = 1234        1234
/ 2^16 = 0 4d2 *10000+5678  = 12345678  = [0] 614E [1] BC                                            12345678 % 2^16 =
614E    12345678 / 2^16 = BC

1234 5678 9123
614E*10000+9123  = 249109123 = [0] 1A83 [1] = BC*10000+ED9 = BE99  [3]  1C                249109123% 2^16 = 1A83
249109123 / 2^16 = ED9


9900000023941                                       +           446744073709551616
[0] = 5509 [1] = 1532 [2] = 2305                                [0] = 0 [1] = 15096 [2] = 10078  [3] = 1587

100100000001 10111111100 1010110000101
               100100000001 0000010111111100 0001010110000101
110001100110010011101011110 0011101011111000 0000000000000000
110001100110011000001011111 0100000011110100 0001010110000101
110001100110011000001011111 0100000011110100 0001010110000101

446753973709575557
11000110011001100000101111101000000111101000001010110000101


  1111 1111 1111
        111 1111
1 0000 0111 1110
1 0000 0111 1110
4095 + 127 = 4222


4095 * 127 = 520065
111 1110 1111 1000 0001

1111 * 1111 =  1110 0001
11111111 * 11111111 = 1111 1110 0000 0001
11111111 * 1111 = 1110 1111 0001
111111111111 * 111111111111 = 1111 1111 1110 0000 0000 0001
111111111111 * 1111 =  1110 1111 1111 0001


4 * 4 = 8
8 * 4 = 12
8 * 8 = 16
12 * 12 = 24
12 * 4  = 16

00004034534563456345695
round 16 : DataInShorts[0] = 58975 DataInShorts[1] = 16601 DataInShorts[2] = 35791 DataInShorts[3] = 14333
DataInShorts[4] = 0
* 127

7489825                  = 111 0010 0100 1001 0010 0001    =     114... 18721
2108327  + 114 = 2108441 =  10 0000 0010 1100 0001 1001    =      32... 11289
4545457  + 32  = 4545489 = 100 0101 0101 1011 1101 0001    =      69... 23505
1820291  + 69  = 1820360 =   1 1011 1100 0110 1100 1000    =      27... 50888
                                                                           27
DataInShorts[0] = 18721 DataInShorts[1] = 11289 DataInShorts[2] = 23505 DataInShorts[3] = 50888 DataInShorts[4] = 27
[0]                [2] = 27   [3] = 0
512385889558955903265
*/