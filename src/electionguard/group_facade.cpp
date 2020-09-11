#include "electionguard/group.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/group.h"
}

eg_element_mod_p_t *eg_element_mod_p_new(uint64_t *elem)
{
    return AS_TYPE(eg_element_mod_p_t, new electionguard::ElementModP(elem));
}

eg_element_mod_p_t *eg_element_mod_p_new_unchecked(uint64_t *elem)
{
    return AS_TYPE(eg_element_mod_p_t, new electionguard::ElementModP(elem, true));
}

void eg_element_mod_p_free(eg_element_mod_p_t *element)
{
    if (element == nullptr) {
        return;
    }
    // TODO: safety
    delete AS_TYPE(electionguard::ElementModP, element);
}

uint8_t eg_element_mod_p_get(eg_element_mod_p_t *element, uint64_t **data)
{
    *data = AS_TYPE(electionguard::ElementModP, element)->get();
    return (uint8_t)64; //TODO: return the real size
}

eg_element_mod_q_t *eg_element_mod_q_new(uint64_t *elem)
{
    return AS_TYPE(eg_element_mod_q_t, new electionguard::ElementModQ(elem));
}

eg_element_mod_q_t *eg_element_mod_q_new_unchecked(uint64_t *elem)
{
    return AS_TYPE(eg_element_mod_q_t, new electionguard::ElementModQ(elem, true));
}

void eg_element_mod_q_free(eg_element_mod_q_t *element)
{
    if (element == nullptr) {
        return;
    }
    // TODO: safety
    delete AS_TYPE(electionguard::ElementModQ, element);
}

uint8_t eg_element_mod_q_get(eg_element_mod_q_t *element, uint64_t **data)
{
    *data = AS_TYPE(electionguard::ElementModQ, element)->get();
    return (uint8_t)4; //TODO: return the real size
}