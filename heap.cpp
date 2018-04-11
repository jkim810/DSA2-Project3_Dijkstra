#include "heap.h"

using namespace std;
//heap declaration
heap::heap(int capacity) {
	heap::capacity = capacity;
	currSize = 0;
	data.resize(capacity + 1);
	mapping = new hashTable(capacity * 2);
}

//percolate up used for insertions and increase key operations
void heap::percolateUp(int posCur) {
	if (posCur == 0) return;
	
	node tmp = data[posCur];
	int hole = posCur;
	//traverse up (bubble up) the heap
	for (; (hole > 1) && (tmp.key < data[hole / 2].key); hole /= 2) {
		data[hole] = data[hole / 2];
		mapping->setPointer(data[hole].id, &data[hole]);
	}
	//modify heap and hashtable
	data[hole] = tmp;
	mapping->setPointer(data[hole].id,&data[hole]);
}

//percolate down used for deletions and decrease key operations
void heap::percolateDown(int posCur) {
	if (posCur == capacity || posCur == 0) return;
	node tmp = data[posCur];
	//traverse down the heap
	for (; posCur * 2 <= currSize;) {
		if ((posCur * 2 == capacity) || (data[posCur * 2].key <= data[posCur * 2 + 1].key) && (tmp.key > data[posCur * 2].key)) {
			data[posCur] = data[posCur * 2];
			mapping->setPointer(data[posCur].id, &data[posCur]);
			posCur = posCur * 2;
		}
		else if ((tmp.key > data[posCur*2+1].key)&&(data[posCur*2].key>data[posCur*2+1].key)){
			data[posCur] = data[posCur * 2+1];
			mapping->setPointer(data[posCur].id, &data[posCur]);
			posCur = posCur * 2+1;
		}
		else break;
	}
	//modify heap and hashtable
	data[posCur] = tmp;
	mapping->setPointer(data[posCur].id, &data[posCur]);
}

//simple function that returns whether the heap is empty
bool heap::isEmpty() {
	return currSize == 0?true:false;
}

//take a pointer from hashtable and return the position in heap
int heap::getPos(node *pn) {
	int pos = pn - &data[0];
	return pos;
}


int heap::insert(const string &id, int key, void *pv) {
	if (currSize == data.size() - 1) return 1;
	if (mapping->contains(id)) return 2;
	node tmp;
	tmp.id = id;
	tmp.key = key;
	tmp.pData = pv;
	data[++currSize] = tmp;

	mapping->insert(id, &data[currSize]);
	percolateUp(currSize);
	return 0;
}

int heap::setKey(const string &id, int key) {
	bool b;
	node *pn = static_cast<node *>(mapping->getPointer(id, &b));
	if (!b) return 1;
	int pos = getPos(pn);
	pn->key = key;
	percolateUp(pos);
	percolateDown(pos);
	return 0;
}


int heap::deleteMin(string *pId, int *pKey, void *ppData) {
	if (isEmpty()) return 1;
	if (pId) *pId = data[1].id;
	if (pKey) *pKey = data[1].key;
	if(ppData) *(static_cast<void **> (ppData)) = data[1].pData;

	mapping->remove(data[1].id);
	data[1] = data[currSize--];
	percolateDown(1);
	return 0;
}

int heap::remove(const string &id, int *pKey, void *ppData) {
	bool b;
	node *pn = static_cast<node *>(mapping->getPointer(id, &b));
	int pos = getPos(pn);
	if (b == false || pos == 0) return 1;
	if (pKey) *pKey = data[pos].key;
	if(ppData) *(static_cast<void **> (ppData)) = data[pos].pData;

	mapping->remove(data[pos].id);
	data[pos] = data[currSize--];

	percolateDown(pos);
	percolateUp(pos);
	return 0;
}