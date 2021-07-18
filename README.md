# LRU Cache
Not a usable LRU Cache, just the basic idea.
It uses an array to store the values and a parallel array to store nodes to a linked list.
It does a linear scan through the cache to find an item, if it is found it simply updates this value to the most recently used item.
If it isn't found it appends to the end of the cache if it isn't full and if not it evicts the least recently used value.
