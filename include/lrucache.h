#ifndef LRU_CACHE_H
#define LRU_CACHE_H
#include <iostream>
#include <map>
#include <unordered_map>
#include <optional>
#include <ostream>

#define DEBUG

template <class Key, class Val>
struct Node{
	Key key;
	Val val;
	Node *prev =nullptr;
	Node *next =nullptr;

	Node(Key key, Val val, Node *prev, Node *next)
		:key(key), val(val), prev(prev), next(next){}

};


#ifdef DEBUG
template <class Key, class Val>
std::ostream& operator<<(std::ostream& stream, const Node<Key, Val>& node){
	stream << '(' << node.key << ':' << node.val << ')' 
		   << std::endl;
	return stream;
}
#endif


template <class Key, class Val>
class LRUCache{
public:
	std::unordered_map<Key, Node<Key, Val>*> hashmap;
	LRUCache(int capacity)
		:capacity(capacity){}

	std::optional<Val> operator[](const Key k){
		if (count(k) != 0){
			get(k);
			return hashmap[k]->val;
		}
		return std::nullopt;
	}

	void insert(Key k, Val v){
		update(k, v);
	}

	#ifdef DEBUG
	void print(){
		std::cout << size() << " { ";
		for (auto& [k, v] : hashmap)
			std::cout << *v;
		std::cout << "}" << std::endl;
	}
	#endif

	size_t size(){
		return hashmap.size();
	}

	size_t count(Key k){
		return hashmap.count(k);
	}

	~LRUCache(){
		for (auto& [k, v] : hashmap)
			delete v;
		hashmap.clear(); // not necessary
	}

private:
	Node<Key, Val> *currentNode =nullptr;
	Node<Key, Val> *lastNode =nullptr;
	int capacity;
	void update(Key k, Val v){
		if (hashmap.count(k) == 0){
			if (hashmap.size() >= capacity)
				remLRU();
			Node<Key, Val> *newNode = new Node<Key, Val>(k, v, lastNode, nullptr);
			hashmap[k] = newNode;
			if (lastNode) lastNode->next = newNode;
			lastNode = newNode;
			if (!currentNode) 
				currentNode = newNode;
		} else {
			hashmap[k]->val = v;
			get(k);
		}
	}

	void get(Key k){
		Node<Key, Val> *n = hashmap[k];
		if (!n->next) return;
		if (n->prev) n->prev->next = n->next;
		n->next->prev = n->prev;
		lastNode->next = n;
		n->prev = lastNode;
		if (n == currentNode) currentNode = n->next;
		n->next = nullptr;
		lastNode = n;
	}

	void remLRU(){
		if (!currentNode) return;
		hashmap.erase(currentNode->key);
		Node<Key, Val> *cNode = currentNode;
		currentNode = currentNode->next;
		if (lastNode == cNode) lastNode = nullptr;
		if (currentNode == cNode) currentNode = nullptr;
		delete cNode;
		if (currentNode) currentNode->prev = nullptr;
	}
};
#endif
