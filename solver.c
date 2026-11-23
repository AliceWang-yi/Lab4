/* Cache Lab Bonus: infer the cache parameters using only hit/miss probes. */
#include "solver.h"

static int all_fit(int count, uint32_t stride)
{
    cache_flush();
    for (int i = 0; i < count; ++i)
        cache_load((uint32_t)i * stride);
    for (int i = 0; i < count; ++i)
        if (!cache_load((uint32_t)i * stride))
            return 0;
    return 1;
}

static int find_victim(int E, uint32_t stride)
{
    cache_flush();
    for (int i = 0; i < E; ++i)
        cache_load((uint32_t)i * stride);
    cache_load(0);
    cache_load((uint32_t)E * stride);

    /* Stop at the first miss: that probe can itself cause another eviction. */
    for (int i = 0; i < E; ++i)
        if (!cache_load((uint32_t)i * stride))
            return i;
    return -1;
}

void solve(void)
{
    int b = 0;
    cache_flush();
    cache_load(0);
    while (cache_load(1u << b))
        ++b;

    /* Nine index bits is the largest permitted geometry. */
    uint32_t same_set = 1u << (b + 9);
    int E = 1;
    while (E < 16 && all_fit(E + 1, same_set))
        ++E;

    int s = 0;
    while (all_fit(E + 1, 1u << (b + s)))
        ++s;

    cache_flush();
    cache_store(0);
    int write_allocate = cache_load(0);
    int policy = POLICY_LRU;
    if (E > 1) {
        int victim = find_victim(E, same_set);
        if (victim == 0)
            policy = POLICY_FIFO;
        else if (victim != 1)
            policy = POLICY_RANDOM;

        for (int trial = 1; trial < 64 && policy != POLICY_RANDOM; ++trial)
            if (find_victim(E, same_set) != victim)
                policy = POLICY_RANDOM;
    }
    report(b, s, E, policy, write_allocate);
}
