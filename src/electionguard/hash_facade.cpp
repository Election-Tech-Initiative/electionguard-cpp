#include "electionguard/group.hpp"
#include "electionguard/hash.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/hash.h"
}

eg_element_mod_q_t *eg_hash_elems_string(const char *a)
{
    return AS_TYPE(eg_element_mod_q_t, electionguard::hash_elems(a));
}

eg_element_mod_q_t *eg_hash_elems_strings(const char **a, int size)
{
    std::vector<std::string> v(a, a + size);
    return AS_TYPE(eg_element_mod_q_t, electionguard::hash_elems(v));
}

eg_element_mod_q_t *eg_hash_elems_int(const uint64_t a)
{
    return AS_TYPE(eg_element_mod_q_t, electionguard::hash_elems(a));
}