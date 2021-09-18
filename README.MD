# BigNumber

BigNumber is a new type can handle numbers of any length, Including integers and decimals

## Designer

* exception
* regex

storage length = ceil((precision*log2(10) + 1)/16)      
 strSize = (_m_size / 2 - 0.062 ) /  0.208
 strSize = (_m_size / 2 - 0.062 ) /  0.208
 _m_size = (2 * static_cast<int>(std::ceil((strSize * 0.208) + 0.062)));


 _m_size   strSize
 4         10  

