# BigNumber

BigNumber is a new type can handle numbers of any length, Including integers and decimals

## Designer

定点计数
* exception
* regex
* 运算符重载
    * + len = max(lenr, lenl) + 1
    * X len = lenr + lenl

* 小数
    * precision
    * scale


 1234345.345283764              p = 16 s = 9
+ 342342.34354                  0 = 11 s = 5

1.23 * 1.2 =  1.476
3 2    2 1    4 3

11.23 * 1.2 =  13.476
4 2     2 1     5  3

11.23 * 11.23 =  126.1129
 4 2     4 2       7 4

99.999 * 99.999 = 9999.800001
 5 3      5 3       10 6

* scale = scale1 + scale2 


storage length = ceil((precision*log2(10) + 1)/16)      
 strSize = (_m_size / 2 - 0.062 ) /  0.208
 strSize = (_m_size / 2 - 0.062 ) /  0.208
 _m_size = (2 * static_cast<int>(std::ceil((strSize * 0.208) + 0.062)));


 _m_size   strSize
 4         10  


(ﾟДﾟ*)ﾉ 吐槽。为啥http://www.chendianrong.com/ 这个的热门分类第一个是两性关系


一个b进制的数转换为10进制的方式为：
(...a_3 a_2 a_1 a_0. a_{-1} a_{-2}..)_b = ...+ a_3 b^3 + a_2 b^2 +a_1 b^1 + a_0 + a_{-1} b^{-1} +a_{-2} b^{-2} +..

如果这里只有一位b进制数据a0，则转换为10进制数m为

a0 = m
此时m进行变换M = m*n + x，对于m来讲，在10进制内进行变换不会有任何溢出问题，但是对于b进制下的数据，则会有溢出的风险，所以需要进行进制转换。
所以在考虑溢出的情况下，a0的计算为
A0 = (a0 * n + x)b，j假设溢出计算之后新的数据为a1 a0，则此时数据为
a1 *b + a0 = M，此时再对M进行相同的变换，此时假设a0有溢出，进位为y，则

(a1 * b)*n + y + a0 = nm
以1234 5678 9123 为例子
首先以4位进行分割

    --         2^16
1   --         1234
2   --         BC * 2^16 + 614E         a1 = BC a0 = 614E
3   --         (BC * 2^16 + 614E) * 10^4 +  9123
                = BC * 2^16 * 10^4  + 614E * 10^4 +  9123
                = BC * 2^16 * 10^4  + ED9  * 2^16 +  1A83
                = (BC * 10^4 + ED9) * 2^16 + 1A83
                = (1C * 2^16 + BE99) * 2^16 + 1A83
                = 1C * 2^(16*2) + BE99 * 2^16 + 1A83
                a2 = 1C a1 = BE99 a0 = 1A83

                                                        之所以使用10000的方式扩展是因为即使是10进制的8个9也可以可以使用2^16的形式表示，而且这样可以避免直接操作一盒大数，可以是使用分段的方式来处理
使用代码表示为
```
string origin = x;
auto len = origin.size();
auto len1 = xx;
auto targetDataInShorts = new unsigned short[len1]
auto finalTargetLengthInShorts = 1;

// 这里以2^16进制为例，求求余的时候，可以使用使用下列的结构快速求余
  union {
    unsigned int temp;
    struct {
      unsigned short remainder;  // 余数
      unsigned short carry;      // 进位
    } tempParts;
  };
for (int i = 0; i <len; i+=4)
    unsiged short xvalue = 0;
    while (j<4 && a + j < len)
        xvalue= xvalue * 10 + (origin[i+j] - '0'), j++;
    // if there are not 4 bites number
    auto power = pow(10, j-1);

    temp = ((unsigned int)targetDataInShorts[0]) * power + xvalue;
    for (j = 1; j < finalTargetLengthInShorts; j++) {
      temp = ((unsigned int)targetDataInShorts[j]) * power + tempParts.carry;
      targetDataInShorts[j] = tempParts.remainder;
    }
    if (tempParts.carry) {
      if (finalTargetLengthInShorts >= targetLengthInShorts) return -1;
      finalTargetLengthInShorts++;
      targetDataInShorts[finalTargetLengthInShorts - 1] = tempParts.carry;
    }


```
逆操作：
a2 = 1C a1 = BE99 a0 = 1A83 = 
1C * 2^(16*2) + BE99 * 2^16 + 1A83 = (BC * 2^16 + 614E) * 10^4 +  9123
使用除发操作，可以提取出每一步的最后的新添加的数值
(1C * 2^(16*2) / 10000 + BE99 * 2^16 / 10000 + 1A83 / 10000) 
(1C * 2^16 + BE99) * 2^16 + 1A83
= (BC * 10^4 + ED9) * 2^16 + 1A83
= BC * 2^16 * 10^4  + ED9  * 2^16 +  1A83
= BC * 2^16 * 10^4  + 614E * 10^4 +  9123
= (BC * 2^16 + 614E) * 10^4 +  9123                 -- 结束取得9123，再次对内部遍历

BC * 2^16 + 614E
= 1234 * 10^4 + 5678 


12345678
BC 614E

249109123
ED9 1A83

123207680000
1C AFC0 0000