#include "electionguard/group.hpp"

#include "memory_cache.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/group.h"
}

#include "../log.hpp"

#include <cstring>

using electionguard::Cache;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::rand_q;

using std::string;

// TODO: safe initialization
static Cache<ElementModP> cache_element_mod_p;
static Cache<ElementModQ> cache_element_mod_q;

#pragma region ElementModP

eg_element_mod_p_t *eg_element_mod_p_create(uint64_t *elem)
{
    uint64_t data[MAX_P_LEN] = {};
    memcpy(static_cast<uint64_t *>(data), elem, MAX_P_SIZE);
    auto element = make_unique<ElementModP>(data);
    auto *reference = cache_element_mod_p.retain(move(element));
    return AS_TYPE(eg_element_mod_p_t, reference);
}

eg_element_mod_p_t *eg_element_mod_p_create_unchecked(uint64_t *elem)
{
    uint64_t data[MAX_P_LEN] = {};
    memcpy(static_cast<uint64_t *>(data), elem, MAX_P_SIZE);
    auto element = make_unique<ElementModP>(data, true);
    auto *reference = cache_element_mod_p.retain(move(element));
    return AS_TYPE(eg_element_mod_p_t, reference);
}

uint8_t eg_element_mod_p_get(eg_element_mod_p_t *elem, uint64_t **data)
{
    auto *element = AS_TYPE(ElementModP, elem);
    *data = element->get();
    return MAX_P_LEN;
}

#pragma endregion

#pragma region ElementModQ

eg_element_mod_q_t *eg_element_mod_q_create(const uint64_t elem[MAX_Q_LEN])
{
    uint64_t data[MAX_Q_LEN] = {};
    memcpy(static_cast<uint64_t *>(data), elem, MAX_Q_SIZE);
    auto element = make_unique<ElementModQ>(data);
    auto *reference = cache_element_mod_q.retain(move(element));
    return AS_TYPE(eg_element_mod_q_t, reference);
}

eg_element_mod_q_t *eg_element_mod_q_create_unchecked(uint64_t *elem)
{
    uint64_t data[MAX_Q_LEN] = {};
    memcpy(static_cast<uint64_t *>(data), elem, MAX_Q_SIZE);
    auto element = make_unique<ElementModQ>(data, false);
    auto *reference = cache_element_mod_q.retain(move(element));
    return AS_TYPE(eg_element_mod_q_t, reference);
}

uint8_t eg_element_mod_q_get(eg_element_mod_q_t *elem, uint64_t **data)
{
    auto *element = AS_TYPE(ElementModQ, elem);
    *data = element->get();
    return MAX_Q_LEN;
}

#pragma endregion

#pragma region ElementModQ Functions

eg_element_mod_q_t *eg_rand_q()
{
    auto random = rand_q();
    auto reference = cache_element_mod_q.retain(move(random));
    return AS_TYPE(eg_element_mod_q_t, reference);
}

#pragma endregion