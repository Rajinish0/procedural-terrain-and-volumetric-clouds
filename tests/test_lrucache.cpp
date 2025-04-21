#include <gtest/gtest.h>
#include <lrucache.h>

struct IntHasher {
	std::size_t operator()(int key) const {
		return std::hash<int>()(key);
	}
};

using Cache = LRUCache<int, std::string, IntHasher>;

TEST(LRUCacheTest, InsertAndRetrieve) {
	Cache cache(3);
	cache.insert(1, "one");
	cache.insert(2, "two");

	EXPECT_EQ(cache[1], "one");
	EXPECT_EQ(cache[2], "two");
	EXPECT_EQ(cache.size(), 2);
}

TEST(LRUCacheTest, EvictionPolicy) {
	Cache cache(2);
	cache.insert(1, "one");
	cache.insert(2, "two");
	cache.insert(3, "three"); // should evict key 1

	EXPECT_EQ(cache.count(1), 0);
	EXPECT_EQ(cache.count(2), 1);
	EXPECT_EQ(cache.count(3), 1);
}

TEST(LRUCacheTest, AccessUpdatesOrder) {
	Cache cache(2);
	cache.insert(1, "one");
	cache.insert(2, "two");

	std::string val = cache[1];

	cache.insert(3, "three"); // should evict key 2

	EXPECT_EQ(cache.count(1), 1);
	EXPECT_EQ(cache.count(2), 0);
	EXPECT_EQ(cache.count(3), 1);
}

TEST(LRUCacheTest, OverwriteValue) {
	Cache cache(2);
	cache.insert(1, "one");
	cache.insert(1, "uno");

	EXPECT_EQ(cache[1], "uno");
	EXPECT_EQ(cache.size(), 1);
}

TEST(LRUCacheTest, SizeAndCount) {
	Cache cache(2);
	EXPECT_EQ(cache.size(), 0);
	EXPECT_EQ(cache.count(1), 0);

	cache.insert(1, "one");

	EXPECT_EQ(cache.size(), 1);
	EXPECT_EQ(cache.count(1), 1);
}

TEST(LRUCacheTest, RepeatedAccessPreservesMRU) {
	Cache cache(3);
	cache.insert(1, "one");
	cache.insert(2, "two");
	cache.insert(3, "three");

	// Access key 1 and 2 multiple times
	cache[1];
	cache[2];
	cache[1];
	cache[2];

	cache.insert(4, "four"); // Should evict key 3 (least recently used)

	EXPECT_EQ(cache.count(1), 1);
	EXPECT_EQ(cache.count(2), 1);
	EXPECT_EQ(cache.count(3), 0);
	EXPECT_EQ(cache.count(4), 1);
}


TEST(LRUCacheTest, StressTestLargeVolume) {
	const int CAP = 100;
	Cache cache(CAP);

	for (int i = 0; i < 10000; ++i) {
		cache.insert(i, "val" + std::to_string(i));
		EXPECT_LE(cache.size(), CAP);
	}

	// Only the last CAP entries should remain
	for (int i = 0; i < 9900; ++i)
		EXPECT_EQ(cache.count(i), 0);

	for (int i = 9900; i < 10000; ++i)
		EXPECT_EQ(cache.count(i), 1);
}
