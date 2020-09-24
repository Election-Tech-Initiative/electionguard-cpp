#include "electionguard/hash.hpp"

#include "electionguard/group.hpp"
#include "memory_cache.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/hash.h"
}

#include "../log.hpp"

using electionguard::Cache;
using electionguard::ElementModQ;
using electionguard::hash_elems;
using std::vector;

// TODO: safe initialization
static Cache<ElementModQ> cache_element_mod_q;

eg_element_mod_q_t *eg_hash_elems_string(const char *a)
{
    auto hash = hash_elems(a);
    auto *reference = cache_element_mod_q.retain(move(hash));
    return AS_TYPE(eg_element_mod_q_t, reference);
}

eg_element_mod_q_t *eg_hash_elems_strings(const char **a, int size)
{
    vector<string> v(a, a + size);
    auto hash = hash_elems(v);
    auto *reference = cache_element_mod_q.retain(move(hash));
    return AS_TYPE(eg_element_mod_q_t, reference);
}

eg_element_mod_q_t *eg_hash_elems_int(const uint64_t a)
{
    auto hash = hash_elems(a);
    auto *reference = cache_element_mod_q.retain(move(hash));
    return AS_TYPE(eg_element_mod_q_t, reference);
}