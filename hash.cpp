#include "hash.h"

using namespace std;

hashTable::hashTable(int size) {
	filled = 0;
	capacity = getPrime(size);
	data.assign(capacity, hashItem("", false, false, NULL));
}

hashTable::hashItem::hashItem() {
	key = "";
	isOccupied = false;
	isDeleted = false;
	pv = NULL;
}

hashTable::hashItem::hashItem(string key, bool isOccupied, bool isDeleted, void *pv) :
	key(key), isOccupied(isOccupied), isDeleted(isDeleted), pv(pv) {};

// Insert the specified key into the hash table.
// If an optional pointer is provided,
// associate that pointer with the key.
// Returns 0 on success,
// 1 if key already exists in hash table,
// 2 if rehash fails.
int hashTable::insert(const string &key, void *pv) {
	//rehash if half of the table is filled
	if(capacity == 0 || filled > capacity/2){
		if(rehash() == false){
			return 2;
		}
	}

	//return 1 if key exists
	if(contains(key)) return 1;

	//otherwise insert
	hashItem *itemToInsert = new hashItem(key, true, false, pv);
	int position = hash(key);

	//linear probing
	while(data[position].isOccupied){
		position++;
		if(position == capacity) position = 0;
	}

	data.at(position) = *itemToInsert;
	filled++;
	return 0;
}

// Check if the specified key is in the hash table.
// If so, return true; otherwise, return false.
bool hashTable::contains(const string &key) {
	if (findPos(key) == -1) return false;
	else return true;
}

// Get the pointer associated with the specified key.
// If the key does not exist in the hash table, return NULL.
// If an optional pointer to a bool is provided,
// set the bool to true if the key is in the hash table,
// and set the bool to false otherwise.
void *hashTable::getPointer(const string &key, bool *b) {
	int index = findPos(key);
	if (index == -1) {
		*b = false;
		return NULL;
	}
	else {
		*b = true;
		return data[index].pv;
	}
}

// Set the pointer associated with the specified key.
// Returns 0 on success,
// 1 if the key does not exist in the hash table.
int hashTable::setPointer(const string &key, void *pv){
	int index = findPos(key);
	if (index == -1) {
		return 1;
	}
	else {
		data[index].pv = pv;
		return 0;
	}
}


// Delete the item with the specified key.
// Returns true on success,
// false if the specified key is not in the hash table.
bool hashTable::remove(const string &key) {
	int tmpHash = findPos(key);
	if (tmpHash == -1) {
		return false;
	}
	else {
		data[tmpHash].isDeleted = true;
		return true;
	}
}

// The hash function.
int hashTable::hash(const string &key) {
	unsigned int hash = 5381;
	for(int i = 0; i < key.length(); i++)  hash = ((hash << 5) + hash) + key.at(i); 
	hash = hash%capacity;
    return (int) hash;
}

// Search for an item with the specified key.
// Return the position if found, -1 otherwise.
int hashTable::findPos(const string &key) {
	int position = hash(key);
	while(data[position].isOccupied == true){
		if((data[position].key.compare(key) == 0)&&(!data[position].isDeleted)){
			return position;
		}
		position++;
		if(position == capacity) position = 0;
	}
	return -1;
}

// The rehash function; makes the hash table bigger.
// Returns true on success, false if memory allocation fails.
bool hashTable::rehash() {
	capacity = getPrime(filled);
	if(capacity == 0){
		cout<< "Error: Rehash Failed. Data set too big." <<endl;
		return false;
	}
	vector<hashItem> tmp = data;
	data.assign(capacity,hashItem("",false,false,NULL));
	filled = 0;
	for(hashItem it:tmp){
		if(it.isOccupied && !it.isDeleted) insert(it.key, it.pv);
	}
	return true;
}

// Return a prime number at least as large as size.
// Uses a precomputed sequence of selected prime numbers.
unsigned int hashTable::getPrime(int size) {
	static unsigned int primeList[] = {50021, 100003, 200003, 500009, 1000003, 2000003, 4000037, 10000019};
	for(int i = 0; i < 6; i++){
		if(primeList[i]/2 > size){
			return primeList[i];
		}
	}
	return -1;
}