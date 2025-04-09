#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// Define a doubly linked list node structure
typedef struct DLLNode {
    int key;
    int value;
    struct DLLNode* prev;
    struct DLLNode* next;
} DLLNode;

// Define a hash table node structure
typedef struct HashNode {
    int key;
    DLLNode* node;
    struct HashNode* next;  // For hash collision handling with chaining
} HashNode;

// Define LRU Cache structure
typedef struct {
    int capacity;
    int count;
    DLLNode* head;  // Most recently used
    DLLNode* tail;  // Least recently used
    HashNode** hashTable;
    int hashSize;
} LRUCache;

// Creates a new DLL node
DLLNode* createDLLNode(int key, int value) {
    DLLNode* newNode = (DLLNode*)malloc(sizeof(DLLNode));
    newNode->key = key;
    newNode->value = value;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Creates a new hash node
HashNode* createHashNode(int key, DLLNode* node) {
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->key = key;
    newNode->node = node;
    newNode->next = NULL;
    return newNode;
}

// Hash function
unsigned int hash(int key, int size) {
    return abs(key) % size;  // Using abs to handle negative keys
}

// Initialize the LRU cache
LRUCache* lRUCacheCreate(int capacity) {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->count = 0;
    cache->head = NULL;
    cache->tail = NULL;
    
    // Initialize hash table (size is typically a prime number larger than capacity)
    cache->hashSize = capacity * 2 + 1;
    cache->hashTable = (HashNode**)calloc(cache->hashSize, sizeof(HashNode*));
    
    return cache;
}

// Add node to head (most recently used)
void addToHead(LRUCache* cache, DLLNode* node) {
    node->next = cache->head;
    node->prev = NULL;
    
    if (cache->head != NULL) {
        cache->head->prev = node;
    }
    
    cache->head = node;
    
    if (cache->tail == NULL) {
        cache->tail = node;
    }
}

// Remove a node from the doubly linked list
void removeNode(LRUCache* cache, DLLNode* node) {
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        cache->head = node->next;
    }
    
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        cache->tail = node->prev;
    }
}

// Move a node to the head (mark as recently used)
void moveToHead(LRUCache* cache, DLLNode* node) {
    removeNode(cache, node);
    addToHead(cache, node);
}

// Add or update a key-value pair in the hash table
void addToHash(LRUCache* cache, int key, DLLNode* node) {
    unsigned int index = hash(key, cache->hashSize);
    HashNode* newNode = createHashNode(key, node);
    
    newNode->next = cache->hashTable[index];
    cache->hashTable[index] = newNode;
}

// Find a node in the hash table by key
DLLNode* findInHash(LRUCache* cache, int key) {
    unsigned int index = hash(key, cache->hashSize);
    HashNode* current = cache->hashTable[index];
    
    while (current != NULL) {
        if (current->key == key) {
            return current->node;
        }
        current = current->next;
    }
    
    return NULL;
}

// Remove a key from the hash table
void removeFromHash(LRUCache* cache, int key) {
    unsigned int index = hash(key, cache->hashSize);
    HashNode* current = cache->hashTable[index];
    HashNode* prev = NULL;
    
    while (current != NULL) {
        if (current->key == key) {
            if (prev == NULL) {
                cache->hashTable[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Get value for a key from the cache
int lRUCacheGet(LRUCache* cache, int key) {
    DLLNode* node = findInHash(cache, key);
    
    if (node == NULL) {
        return -1;  // Key not found
    }
    
    // Move to head (mark as recently used)
    moveToHead(cache, node);
    
    return node->value;
}

// Put a key-value pair in the cache
void lRUCachePut(LRUCache* cache, int key, int value) {
    DLLNode* node = findInHash(cache, key);
    
    if (node != NULL) {
        // Key exists, update the value
        node->value = value;
        // Move to head (mark as recently used)
        moveToHead(cache, node);
    } else {
        // Key doesn't exist, create a new node
        DLLNode* newNode = createDLLNode(key, value);
        
        // If cache is full, remove the least recently used item
        if (cache->count >= cache->capacity) {
            // Remove from hash table first
            removeFromHash(cache, cache->tail->key);
            
            // Then remove from linked list
            DLLNode* tail = cache->tail;
            removeNode(cache, tail);
            free(tail);
            
            cache->count--;
        }
        
        // Add new node
        addToHead(cache, newNode);
        addToHash(cache, key, newNode);
        cache->count++;
    }
}

// Print the cache (for debugging)
void lRUCachePrint(LRUCache* cache) {
    printf("LRU Cache (MRU -> LRU): ");
    DLLNode* current = cache->head;
    while (current != NULL) {
        printf("(%d:%d) ", current->key, current->value);
        current = current->next;
    }
    printf("\n");
}

// Free all memory allocated for the cache
void lRUCacheFree(LRUCache* cache) {
    // Free the doubly linked list
    DLLNode* current = cache->head;
    while (current != NULL) {
        DLLNode* temp = current;
        current = current->next;
        free(temp);
    }
    
    // Free the hash table
    for (int i = 0; i < cache->hashSize; i++) {
        HashNode* current = cache->hashTable[i];
        while (current != NULL) {
            HashNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    
    free(cache->hashTable);
    free(cache);
}

// Test case runner
void runTest(const char* testName, void (*testFunc)()) {
    printf("Running test: %s\n", testName);
    testFunc();
    printf("Test passed: %s\n\n", testName);
}

// Test case 1: Basic operations
void testBasicOperations() {
    LRUCache* cache = lRUCacheCreate(2);
    
    lRUCachePut(cache, 1, 1);
    lRUCachePrint(cache);
    
    lRUCachePut(cache, 2, 2);
    lRUCachePrint(cache);
    
    assert(lRUCacheGet(cache, 1) == 1);
    lRUCachePrint(cache);
    
    lRUCachePut(cache, 3, 3);  // This should evict key 2
    lRUCachePrint(cache);
    
    assert(lRUCacheGet(cache, 2) == -1);
    
    lRUCachePut(cache, 4, 4);  // This should evict key 1
    lRUCachePrint(cache);
    
    assert(lRUCacheGet(cache, 1) == -1);
    assert(lRUCacheGet(cache, 3) == 3);
    assert(lRUCacheGet(cache, 4) == 4);
    
    lRUCacheFree(cache);
}

// Test case 2: Update existing key
void testUpdateExistingKey() {
    LRUCache* cache = lRUCacheCreate(2);
    
    lRUCachePut(cache, 1, 1);
    lRUCachePut(cache, 2, 2);
    
    assert(lRUCacheGet(cache, 1) == 1);
    
    lRUCachePut(cache, 1, 10);  // Update value for key 1
    
    assert(lRUCacheGet(cache, 1) == 10);
    assert(lRUCacheGet(cache, 2) == 2);
    
    lRUCacheFree(cache);
}

// Test case 3: LRU eviction order
void testLRUEvictionOrder() {
    LRUCache* cache = lRUCacheCreate(3);
    
    lRUCachePut(cache, 1, 1);
    lRUCachePut(cache, 2, 2);
    lRUCachePut(cache, 3, 3);
    
    lRUCachePrint(cache);  // Order should be 3,2,1
    
    // Access key 1, making it the most recently used
    assert(lRUCacheGet(cache, 1) == 1);
    lRUCachePrint(cache);  // Order should be 1,3,2
    
    // Access key 2, making it the most recently used
    assert(lRUCacheGet(cache, 2) == 2);
    lRUCachePrint(cache);  // Order should be 2,1,3
    
    // Add a new key, which should evict key 3
    lRUCachePut(cache, 4, 4);
    lRUCachePrint(cache);  // Order should be 4,2,1
    
    assert(lRUCacheGet(cache, 3) == -1);  // Key 3 should be evicted
    assert(lRUCacheGet(cache, 1) == 1);
    assert(lRUCacheGet(cache, 2) == 2);
    assert(lRUCacheGet(cache, 4) == 4);
    
    lRUCacheFree(cache);
}

// Test case 4: Capacity edge cases
void testCapacityEdgeCases() {
    // Test with capacity 1
    LRUCache* cache1 = lRUCacheCreate(1);
    
    lRUCachePut(cache1, 1, 1);
    assert(lRUCacheGet(cache1, 1) == 1);
    
    lRUCachePut(cache1, 2, 2);  // This should evict key 1
    assert(lRUCacheGet(cache1, 1) == -1);
    assert(lRUCacheGet(cache1, 2) == 2);
    
    lRUCacheFree(cache1);
    
    // Test with larger capacity
    LRUCache* cache2 = lRUCacheCreate(5);
    
    for (int i = 1; i <= 5; i++) {
        lRUCachePut(cache2, i, i);
    }
    
    for (int i = 1; i <= 5; i++) {
        assert(lRUCacheGet(cache2, i) == i);
    }
    
    // Adding a 6th item should evict the oldest one (key 1)
    lRUCachePut(cache2, 6, 6);
    assert(lRUCacheGet(cache2, 1) == -1);
    assert(lRUCacheGet(cache2, 6) == 6);
    
    lRUCacheFree(cache2);
}

// Test case 5: Negative keys
void testNegativeKeys() {
    LRUCache* cache = lRUCacheCreate(2);
    
    lRUCachePut(cache, -1, 10);
    lRUCachePut(cache, -2, 20);
    
    assert(lRUCacheGet(cache, -1) == 10);
    assert(lRUCacheGet(cache, -2) == 20);
    
    lRUCachePut(cache, -3, 30);  // This should evict key -1
    
    assert(lRUCacheGet(cache, -1) == -1);
    assert(lRUCacheGet(cache, -2) == 20);
    assert(lRUCacheGet(cache, -3) == 30);
    
    lRUCacheFree(cache);
}

// Main function to run all tests
int main() {
    printf("===== LRU Cache Tests =====\n\n");
    
    runTest("Basic Operations", testBasicOperations);
    runTest("Update Existing Key", testUpdateExistingKey);
    runTest("LRU Eviction Order", testLRUEvictionOrder);
    runTest("Capacity Edge Cases", testCapacityEdgeCases);
    runTest("Negative Keys", testNegativeKeys);
    
    printf("All tests passed successfully!\n");
    
    return 0;
}
