# BigNumber

BigNumber is a new type can handle numbers of any length, Including integers and decimals

## Designer

首先我们可以看看int类型的数据是怎么存储的，
例如
65535
存储为int的时候，是 00000000 11111111
6553 * 10000 + 5 = 65535

65538
6553 * 10000 + 8 = 1*2^16 + 1
