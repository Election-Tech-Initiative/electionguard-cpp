#include "electionguard/group.hpp"

extern "C" {
#include "electionguard/group.h"
}

#define AS_TYPE(Type, Obj) reinterpret_cast<Type *>(Obj)
#define AS_CTYPE(Type, Obj) reinterpret_cast<const Type *>(Obj)

eg_element_mod_p_t *eg_element_mod_p_new(uint64_t *elem)
{
    return AS_TYPE(eg_element_mod_p_t, new electionguard::ElementModP(elem));
}

void eg_element_mod_p_free(eg_element_mod_p_t *element)
{
    if (!element)
        return;
    delete AS_TYPE(electionguard::ElementModP, element);
}

eg_element_mod_q_t *eg_element_mod_q_new(uint64_t *elem)
{
    return AS_TYPE(eg_element_mod_q_t, new electionguard::ElementModQ(elem));
}

void eg_element_mod_q_free(eg_element_mod_q_t *element)
{
    if (!element)
        return;
    delete AS_TYPE(electionguard::ElementModQ, element);
}