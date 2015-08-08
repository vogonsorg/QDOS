/*
 *
 * File: hashtable.c
 * ---------------
 *	David Wright
 *	10/8/98
 *
 * See hashtable.h for function comments
 * Implmentation is straight-forward, using a fixed dynamically allocated
 * array for the buckets, and a DArray for each individual bucket
 */

#include <stdlib.h>
#include <string.h>
#include "darray.h"
#include "hashtable.h"
#include "../../sys.h" /* FS: For Sys_Error */

struct HashImplementation
{
	DArray *buckets;
	int nbuckets;
	TableElementFreeFn freefn;
	TableHashFn hashfn;
	TableCompareFn compfn;
};


HashTable TableNew(int elemSize, int nBuckets,
                   TableHashFn hashFn, TableCompareFn compFn,
                   TableElementFreeFn freeFn)
{
	HashTable table;
	int i;

	if(!hashFn)
		Sys_Error("TableNew: hashFn is NULL.");
	if(!compFn)
		Sys_Error("TableNew: compFn is NULL.");
	if(elemSize <= 0)
		Sys_Error("TableNew: bad elemSize.");
	if(nBuckets <= 0)
		Sys_Error("TableNew: bad nBuckets.");

	table = (HashTable)malloc(sizeof(struct HashImplementation));
	if(!table)
		Sys_Error("TableNew: table is NULL.");

	table->buckets = (DArray *)malloc(nBuckets * sizeof(DArray));
	if(!table->buckets)
		Sys_Error("TableNew: table->buckets is NULL.");
	for (i = 0; i < nBuckets; i++) //ArrayNew will assert if allocation fails
		table->buckets[i] = ArrayNew(elemSize, 0, freeFn);
	table->nbuckets = nBuckets;
	table->freefn = freeFn;
	table->compfn = compFn;
	table->hashfn = hashFn;

	return table;
}

void TableFree(HashTable table)
{
	int i;

	if(!table)
		Sys_Error("TableFree: table is NULL.");

	for (i = 0 ; i < table->nbuckets ; i++)
	{
		ArrayFree(table->buckets[i]);
		table->buckets[i] = NULL;
	}

	free(table->buckets);
	table->buckets = NULL;
	free(table);
	table = NULL;
}

int TableCount(HashTable table)
{
	int i, count = 0;

	for (i = 0 ; i < table->nbuckets ; i++)
		count += ArrayLength(table->buckets[i]);

	return count;
}

void TableEnter(HashTable table, const void *newElem)
{
	int hash, itempos;

	hash = table->hashfn(newElem, table->nbuckets);
	itempos = ArraySearch(table->buckets[hash], newElem, table->compfn, 0, 0);
	if (itempos == NOT_FOUND)
		ArrayAppend(table->buckets[hash], newElem);
	else
		ArrayReplaceAt(table->buckets[hash], newElem, itempos);
}

void *TableLookup(HashTable table, const void *elemKey)
{
	int hash, itempos;

	hash = table->hashfn(elemKey, table->nbuckets);
	itempos = ArraySearch(table->buckets[hash], elemKey, table->compfn, 0, 0);
	if (itempos == NOT_FOUND)
		return NULL;
	else
		return ArrayNth(table->buckets[hash], itempos);
}

void TableMap(HashTable table, TableMapFn fn, void *clientData)
{
	int i;

	if(!fn)
		Sys_Error("TableMap: fn is NULL.");

	for (i = 0 ; i < table->nbuckets ; i++)
		ArrayMap(table->buckets[i], fn, clientData);
}
