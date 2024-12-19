#include "m61.hh"
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <cinttypes>
#include <cassert>
#include <map>
#include <sys/mman.h>

static m61_statistics gstats {
    .nactive = 0,         // # active allocations
    .active_size = 0,     // # bytes in active allocations
    .ntotal = 0,          // # total allocations
    .total_size = 0,      // # bytes in total allocations
    .nfail = 0,           // # failed allocation attempts
    .fail_size = 0,       // # bytes in failed alloc attempts
    .heap_min = 0,                 // smallest allocated addr
    .heap_max = 0,                 // largest allocated addr
};

static std::map<void*, size_t> freed_allocation;
static std::map<void*, size_t> active_allocation;

struct m61_memory_buffer {
    char* buffer;
    size_t pos = 0;
    size_t size = 8 << 20; /* 8 MiB */

    m61_memory_buffer();
    ~m61_memory_buffer();
};

static m61_memory_buffer default_buffer;


m61_memory_buffer::m61_memory_buffer() {
    void* buf = mmap(nullptr,    // Place the buffer at a random address
        this->size,              // Buffer should be 8 MiB big
        PROT_WRITE,              // We want to read and write the buffer
        MAP_ANON | MAP_PRIVATE, -1, 0);
                                 // We want memory freshly allocated by the OS
    assert(buf != MAP_FAILED);
    this->buffer = (char*) buf;
}

m61_memory_buffer::~m61_memory_buffer() {
    munmap(this->buffer, this->size);
}

static void* m61_find_free_space(size_t sz) {
    // do we have a freed allocation that will work?
    for (auto& it : freed_allocation) {
        if (it.second >= sz) {
            return it.first;
        }
    }
    return nullptr;
}


/// m61_malloc(sz, file, line)
///    Returns a pointer to `sz` bytes of freshly-allocated dynamic memory.
///    The memory is not initialized. If `sz == 0`, then m61_malloc may
///    return either `nullptr` or a pointer to a unique allocation.
///    The allocation request was made at source code location `file`:`line`.

void* m61_malloc(size_t sz, const char* file, int line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    // Your code here.
    if (sz > default_buffer.pos - default_buffer.size) {
        // Not enough space left in default buffer for allocation
        ++gstats.nfail;
        gstats.fail_size = sz;
        return nullptr;
    }

    ++gstats.ntotal;
    if (sz == 0) {
        return nullptr;
    }

    // Otherwise there is enough space; claim the next `sz` bytes
    ++gstats.nactive;
    gstats.total_size += sz;
    void* ptr = m61_find_free_space(sz);
    size_t allocate_sz = 0;
    if (!ptr) {
        // if not exists free place
        ptr = &default_buffer.buffer[default_buffer.pos];
        allocate_sz = sz + (16 - sz % 16);
        //heap increases
        default_buffer.pos += allocate_sz;
        uintptr_t heap_first = (uintptr_t)ptr;
        uintptr_t heap_last = (uintptr_t)ptr + allocate_sz;
        if (!gstats.heap_min || heap_first < gstats.heap_min) {
            gstats.heap_min = heap_first; 
        }

        if (!gstats.heap_max || heap_last > gstats.heap_max) {
            gstats.heap_max = heap_last;
        }
    } else {
        // if exists free place
        allocate_sz = freed_allocation[ptr];
        freed_allocation.erase(ptr);
    }
    // update active allocation map
    active_allocation.insert({ptr, allocate_sz});
    return ptr;
}


/// m61_free(ptr, file, line)
///    Frees the memory allocation pointed to by `ptr`. If `ptr == nullptr`,
///    does nothing. Otherwise, `ptr` must point to a currently active
///    allocation returned by `m61_malloc`. The free was called at location
///    `file`:`line`.

void m61_free(void* ptr, const char* file, int line) {
    // avoid uninitialized variable warnings
    (void) ptr, (void) file, (void) line;
    // Your code here. The handout code does nothing!
    if (!ptr) {
        return;
    }

    // update freed allocation place
    freed_allocation.insert({ptr, active_allocation[ptr]});
    // update active allocation place
    active_allocation.erase(ptr);
    --gstats.nactive;
}


/// m61_calloc(count, sz, file, line)
///    Returns a pointer a fresh dynamic memory allocation big enough to
///    hold an array of `count` elements of `sz` bytes each. Returned
///    memory is initialized to zero. The allocation request was at
///    location `file`:`line`. Returns `nullptr` if out of memory; may
///    also return `nullptr` if `count == 0` or `size == 0`.

void* m61_calloc(size_t count, size_t sz, const char* file, int line) {
    // Your code here (not needed for first tests).
    if (count == 0) {
        ++gstats.ntotal;
        return nullptr;
    }

    if (sz > (default_buffer.pos - default_buffer.size) / count) {
        ++gstats.nfail;
        gstats.fail_size = sz * count;
        return nullptr;
    }
    void* ptr = m61_malloc(count * sz, file, line);
    if (ptr) {
        memset(ptr, 0, count * sz);
    }
    return ptr;
}


/// m61_get_statistics()
///    Return the current memory statistics.

m61_statistics m61_get_statistics() {
    // Your code here.
    // The handout code sets all statistics to enormous numbers.
    m61_statistics stats = gstats;
    return stats;
}


/// m61_print_statistics()
///    Prints the current memory statistics.

void m61_print_statistics() {
    m61_statistics stats = m61_get_statistics();
    printf("alloc count: active %10llu   total %10llu   fail %10llu\n",
           stats.nactive, stats.ntotal, stats.nfail);
    printf("alloc size:  active %10llu   total %10llu   fail %10llu\n",
           stats.active_size, stats.total_size, stats.fail_size);
}


/// m61_print_leak_report()
///    Prints a report of all currently-active allocated blocks of dynamic
///    memory.

void m61_print_leak_report() {
    // Your code here.
}
