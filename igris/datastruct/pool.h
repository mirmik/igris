#ifndef IGRIS_DATASTRUCT_POOL_H
#define IGRIS_DATASTRUCT_POOL_H

#include <igris/datastruct/slist.h>
#include <assert.h>

struct memzone_head {
	struct slist_head lnk;
	void* zone;
	size_t size;
};

struct pool_head {
	struct slist_head free_blocks;
};

struct zonepool_head {
	struct pool_head pool;
	struct slist_head zones;
};

__BEGIN_DECLS

static inline void pool_init(struct pool_head* head) 
{
	slist_init(&head->free_blocks);
}

static inline void pool_engage(struct pool_head* pool, 
			void* zone, size_t size, size_t elemsz) 
{
	assert(size % elemsz == 0);

	char* stop = (char*)zone + size;
	char* it = (char*)zone;

	while(it < stop) {
		slist_add((slist_head*)it, &pool->free_blocks);
		it += elemsz;
	}
}

static inline void* pool_alloc(struct pool_head* head) {
	if (slist_empty(&head->free_blocks)) 
		return nullptr; 
	
	return (void*) slist_pop_first(&head->free_blocks);
}

static inline void pool_free(struct pool_head* head, void* ptr) {
	slist_add((slist_head*) ptr, &head->free_blocks);
}

static inline size_t pool_avail(const struct pool_head* head) {
	return slist_size(&head->free_blocks);
}





static inline void memzone_init(struct memzone_head* head, void* zone, size_t size) {
	head->zone = zone; 
	head->size = size;
}


static inline void pool_engage_memzone(struct pool_head* pool, struct memzone_head* zone) {

}

static inline void pool_empty_zones(struct pool_head* pool, int* retarr);

__END_DECLS

#endif