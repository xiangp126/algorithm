## Single Number II
### Illustrate
<https://leetcode.com/problems/single-number-ii/>

```
Given a non-empty array of integers, every element appears three times except for one, which appears exactly once. Find that single one.

Note:

Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?

Example 1:

Input: [2,2,3,2]
Output: 3
Example 2:

Input: [0,1,0,1,0,1,99]
Output: 99
```

### Core Concept

> assume `22` is wanted, and `35` is three times repeated<br>
> we calculate each bit of the `wanted number`, 32 bit total of type `int`

```bash
   7654 3210   th
0x 0010 0011   (35)
+  0010 0011   (35)
+  0010 0011   (35)
+  0001 0110   (22)
-------------
   0031 0143
% 3
-------------
   0001 0110   yields 22

(1 + 1 + 1) % 3 = 0
```

### Code
C++

```c++
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        const int bitOfInt = sizeof(int) * 8;
        int bits[bitOfInt];
        int ret = 0, temp = 0;
        for (int i = 0; i < bitOfInt; ++i) {
            for (auto num : nums) {
                temp += (num >> i) & 0x1;
            }
            // thus temp was sum of the i th bit of each num
            bits[i] = temp % 3;
            temp = 0;
        }
        // make ret with each bit
        for (int i = 0; i < bitOfInt; ++i) {
            ret |= (bits[i] << i);
        }
        return ret;
    }
};
```

Java

Similarly, there is is no **sizeof()** operator in Java. All primitive values have a predefined size, e.g. int is 4 bytes, char is 2 byte, short is 2 byte, long and float is 8 byte, and so on.

```java
class Solution {
    public int singleNumber(int[] nums) {
        int ret = 0;
        int[] mark = new int[32];
        for (int i = 0; i < nums.length; ++i) {
            for (int j = 0; j < 32; ++j) {
                mark[j] += (nums[i] >> j) & 0x01;
            }
        }

        for (int i = 0; i < 32; ++i) {
            ret |= (mark[i] % 3) << i;
        }
        return ret;
    }
}
```
