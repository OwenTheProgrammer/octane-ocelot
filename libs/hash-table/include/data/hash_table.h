#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct ht   ht;

typedef struct ht_tableValue ht_tableValue;

typedef void (*ht_free_pfn)(void* const ptr);

struct ht_tableValue
{
    void* data;

    uint32_t size;

    ht_free_pfn free_fn;
};


ht* ht_init(uint32_t capacity);

void ht_set_entry(ht* const table, ht_tableValue key, ht_tableValue value);

bool ht_contains_key(ht* const table, ht_tableValue key, uint32_t* const idx);

ht_tableValue ht_get_value(ht* const table, ht_tableValue key);

void ht_remove_entry(ht* const restrict table, void* const restrict data, uint32_t size);

void ht_free(ht* const table);