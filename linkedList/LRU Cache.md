## LRU Cache
### Illustrate
<https://leetcode.com/problems/lru-cache/description>

### Code
```c++
/*
 * @lc app=leetcode id=146 lang=cpp
 *
 * [146] LRU Cache
 */

// @lc code=start
class LRUCache {
public:
    LRUCache(int capacity) {
        this->capacity = capacity;
    }

    int get(int key) {
        auto iter = map.find(key);
        if (iter != map.end()) {
            lruList.splice(lruList.begin(), lruList, iter->second);
            return iter->second->second;
        }
        return -1;
    }

    void put(int key, int value) {
        auto iter = map.find(key);
        if (iter != map.end()) {
            iter->second->second = value;
            lruList.splice(lruList.begin(), lruList, iter->second);
        } else {
            if (map.size() == capacity) {
                map.erase(lruList.back().first);
                lruList.pop_back();
            }
            lruList.push_front({key, value});
            map[key] = lruList.begin();
        }
    }

private:
    unordered_map<int, list<pair<int, int>>::iterator> map;
    list<pair<int, int>> lruList;
    int capacity;
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */
// @lc code=end
```
