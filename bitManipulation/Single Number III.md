## Single Number III
### Illustrate
<https://leetcode.com/problems/single-number-iii/description/>
<http://www.ruanyifeng.com/blog/2009/08/twos_complement.html>

```
Given an array of numbers nums, in which exactly two elements appear only once and all the other elements appear exactly twice. Find the two elements that appear only once.

Example:

Input:  [1,2,1,3,2,5]
Output: [3,5]
Note:

The order of the result is not important. So in the above example, [5, 3] is also correct.
Your algorithm should run in linear runtime complexity. Could you implement it using only constant space complexity?
```

### Core Concept
> Number stored in computer was it's `complement code`, which was itself for positive ones.<br>
> `Complement code` was used to sum correctly between positive number and negative one

数字 | 原码 | 反码 | 补码
--- | --- | --- | ---
10 | 0000 1010 | 0000 1010 | 000 1010
-10 | 1000 1010 | 1111 0101 | 1111 0110

&radic;

```
10 + (-10) =
        0000 1010
(+)     1111 0110
    -------------
        0000 0000
```

> num &= -num

#### How to get the least significant bit?

```
24 & (-24)
        0001 1000
(&)     1110 1000
    -------------
        0000 1000 (8D)
```

_Derivation Formula_

UK  /ˌder.ɪˈveɪ.ʃən/&nbsp;&nbsp;&nbsp;&nbsp;US  /ˌder.əˈveɪ.ʃən/

```bash
a ^ b = c
a ^ c = a ^ (a ^ b) = (a ^ a) ^ b = 0 ^ b = b
```

### Code
C++

```c++
class Solution {
public:
    vector<int> singleNumber(vector<int>& nums) {
        vector<int> ret(2, 0);
        unsigned int tsum = 0;
        for (int num : nums) {
            tsum ^= num;
        }
        // tsum = a ^ b, of which a, b is the wanted number

        // (n & -n) returns the rightmost 1-bit in the binary representation of n
        // Exp: k: 0x0001 0000
        // int k = tsum & -tsum;
        int k = tsum & (~tsum + 1); // Alternative to -tsum

        for (int num : nums) {
            if (num & k) {
                ret[0] ^= num;
            }
        }
        ret[1] = ret[0] ^ tsum;
        return ret;
    }
};
```

Java

```java
class Solution {
    public int[] singleNumber(int[] nums) {
        int[] ret = new int[2];
        int tsum = 0;
        for (int i = 0; i < nums.length; ++i) {
            tsum ^= nums[i];
        }

        int k = tsum & -tsum;
        for (int i = 0; i < nums.length; ++i) {
            if ((nums[i] & k) != 0) {
                ret[0] ^= nums[i];
            }
        }
        ret[1] = ret[0] ^ tsum;
        return ret;
    }
}
```
