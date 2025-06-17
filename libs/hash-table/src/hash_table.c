#include "hash_table.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX(_X, _Y) ( ((_X) > (_Y)) ? (_X) : (_Y) )

static const uint32_t _HT_DEFAULT_CAPACITY = 16;
static const uint32_t _HT_GROWTH_DIVISOR = 4;
static const uint32_t _HT_GROWTH_AMOUNT = 2;

typedef struct _ht_entry    _ht_entry;

struct _ht_entry
{
    ht_tableValue key;

    uint32_t hash;

    ht_tableValue value;
};

struct ht
{
    _ht_entry* entries;

    uint32_t entry_count;

    uint32_t capacity;

    uint32_t growth_threshold;
};

/* Implements the 32-bit FNV-1a non-cryptographic hash function 
 * https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
 */
static uint32_t _ht_hash(void* const key, uint32_t size)
{
    //https://datatracker.ietf.org/doc/html/draft-eastlake-fnv-17.html#section-5
    static const uint32_t FNV_PRIME = 0x01000193;
    static const uint32_t FNV_INIT = 0x811c9dc5;

    uint32_t hash = FNV_INIT;

    uint8_t* stream = (uint8_t*)key;

    for(uint32_t i = 0; i < size; i++)
    {
        hash ^= (uint32_t)stream[i];
        hash *= FNV_PRIME;
    }
    return hash;
}

static void _heap_value_free(ht_tableValue* const hv)
{
    if(hv->free_fn != NULL && hv->data != NULL)
        hv->free_fn(hv->data);

    *hv = (ht_tableValue){0};
}

static void _ht_free_entry(_ht_entry* const kvp)
{
    _heap_value_free(&kvp->key);
    _heap_value_free(&kvp->value);
    kvp->hash = 0;
}

static void _ht_resize_capacity(ht* const table)
{
    if(table->entry_count >= table->growth_threshold)
    {
        table->capacity *= _HT_GROWTH_AMOUNT;
        table->growth_threshold = table->capacity / _HT_GROWTH_DIVISOR;
        table->entries = realloc(table->entries, table->capacity * sizeof(_ht_entry));
    }
}

static uint32_t _ht_find_hash(ht* const table, uint32_t key_hash, uint32_t target_hash)
{
    //Get the starting index from the key hash
    uint32_t idx = key_hash % table->capacity;

    //Infinite loop counter
    uint32_t ilc = 0;

    while((ilc++) < table->capacity)
    {
        _ht_entry ent = table->entries[idx];

        //We found the hash we wanted
        if(ent.hash == target_hash)
            return idx;

        //Else keep going
        idx++;

        //Loop around if needed
        idx *= idx < table->capacity;
    }

    //We detected an infinite loop
    return UINT32_MAX;
}



ht* ht_init(uint32_t capacity)
{
    ht* table = calloc(1, sizeof(ht));

    table->entry_count = 0;
    table->capacity = MAX(capacity, _HT_DEFAULT_CAPACITY);
    table->growth_threshold = table->capacity / _HT_GROWTH_DIVISOR;

    table->entries = calloc(table->capacity, sizeof(_ht_entry));

    return table;
}

void ht_set_entry(ht* const table, ht_tableValue key, ht_tableValue value)
{
    if(key.data == NULL || key.size == 0)
    {
        fprintf(stderr, "Invalid hash table key\n");
        return;
    }

    //Hash the key data and compute its index
    uint32_t key_hash = _ht_hash(key.data, key.size);

    //Try to find the key already in the table
    uint32_t key_idx = _ht_find_hash(table, key_hash, key_hash);

    //The hash table does not contain this key yet
    if(key_idx == UINT32_MAX)
    {
        //Find the next empty slot
        key_idx = _ht_find_hash(table, key_hash, 0);

        //We failed to find an empty slot
        if(key_idx == UINT32_MAX)
            return;
    }

    //We have found either the key slot or an empty one
    _ht_entry* slot = &table->entries[key_idx];

    //Set the key-value-pair data
    slot->key = key;
    slot->hash = key_hash;
    slot->value = value;

    //Increment the count and resize if at capacity
    table->entry_count++;
    _ht_resize_capacity(table);
}

bool ht_contains_key(ht* const table, ht_tableValue key, uint32_t* const idx)
{
    uint32_t key_hash = _ht_hash(key.data, key.size);
    uint32_t key_idx = _ht_find_hash(table, key_hash, key_hash);
    
    if(idx != NULL)
        *idx = key_idx;

    return key_idx != UINT32_MAX;
}

ht_tableValue ht_get_value(ht* const table, ht_tableValue key)
{
    uint32_t key_hash = _ht_hash(key.data, key.size);
    uint32_t key_idx = _ht_find_hash(table, key_hash, key_hash);

    if(key_idx == UINT32_MAX)
        return (ht_tableValue){0};

    return table->entries[key_idx].value;
}

void ht_remove_entry(ht* const restrict table, void* const restrict data, uint32_t size)
{
    //Find the KVP
    uint32_t key_hash = _ht_hash(data, size);
    uint32_t key_idx = _ht_find_hash(table, key_hash, key_hash);

    //We failed to find the key
    if(key_idx == UINT32_MAX)
        return;

    //Free the entry
    _ht_free_entry(&table->entries[key_idx]);
    table->entry_count--;
}


void ht_free(ht* const table)
{
    for(uint32_t i = 0; i < table->capacity; i++)
    {
        _ht_free_entry(&table->entries[i]);
    }

    free(table->entries);
    free(table);
}