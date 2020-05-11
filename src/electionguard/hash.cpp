#include "hash.hpp"

#include <iostream>

namespace electionguard
{
    int hash_elems(string const &a)
    {
        std::cout << __func__ << " : hashing " << a << std::endl;
        return 5;
    }
} // namespace electionguard
