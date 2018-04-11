#ifndef _HASH_H
#define _HASH_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class hashTable {

public:

	// The constructor initializes the hash table.
	// Uses getPrime to choose a prime number at least as large as
	// the specified size for the initial size of the hash table.
	hashTable(int size = 0);

	// Insert the specified key into the hash table.
	// If an optional pointer is provided,
	// associate that pointer with the key.
	// Returns 0 on success,
	// 1 if key already exists in hash table,
	// 2 if rehash fails.
	int insert(const string &key, void *pv = NULL);

	// Check if the specified key is in the hash table.
	// If so, return true; otherwise, return false.
	bool contains(const string &key);

	// Get the pointer associated with the specified key.
	// If the key does not exist in the hash table, return NULL.
	// If an optional pointer to a bool is provided,
	// set the bool to true if the key is in the hash table,
	// and set the bool to false otherwise.
	void *getPointer(const string &key, bool *b = NULL);

	// Set the pointer associated with the specified key.
	// Returns 0 on success,
	// 1 if the key does not exist in the hash table.
	int setPointer(const string &key, void *pv);

	// Delete the item with the specified key.
	// Returns true on success,
	// false if the specified key is not in the hash table.
	bool remove(const string &key);

private:

	// Each item in the hash table contains:
	// key - a string used as a key.
	// isOccupied - if false, this entry is empty,
	//              and the other fields are meaningless.
	// isDeleted - if true, this item has been lazily deleted.
	// pv - a pointer related to the key;
	//      NULL if no pointer was provided to insert.
	class hashItem {
	public:
		string key;
		bool isOccupied;
		bool isDeleted;
		void *pv;
		hashItem();
		hashItem(string key, bool isOccupied, bool isDeleted, void *pv);
	};


	int capacity; // The current capacity of the hash table.
	int filled; // Number of occupied items in the table.

	vector<hashItem> data; // The actual entries are here.

	// The hash function.
	int hash(const string &key);

	// Search for an item with the specified key.
	// Return the position if found, -1 otherwise.
	int findPos(const string &key);

	// The rehash function; makes the hash table bigger.
	// Returns true on success, false if memory allocation fails.
	bool rehash();

	// Return a prime number at least as large as size.
	// Uses a precomputed sequence of selected prime numbers.
	static unsigned int getPrime(int size);
};

#endif //_HASH_H