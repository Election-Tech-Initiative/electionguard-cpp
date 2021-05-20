#ifndef __ELECTIONGUARD_CPP_TEST_MOCKS_ELECTION_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_TEST_MOCKS_ELECTION_HPP_INCLUDED__

#include <electionguard/election.hpp>
#include <electionguard/export.h>
#include <electionguard/manifest.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace electionguard;
using namespace std;

namespace electionguard::test::mocks
{
    class ElectionGenerator
    {
      public:
        static unique_ptr<CiphertextElectionContext>
        getFakeContext(const InternalManifest &manifest, const ElementModP &publicKey)
        {
            auto context = CiphertextElectionContext::make(
              1UL, 1UL, make_unique<ElementModP>(publicKey), make_unique<ElementModQ>(TWO_MOD_Q()),
              make_unique<ElementModQ>(*manifest.getManifestHash()));
            return context;
        }
    };
} // namespace electionguard::test::mocks

#endif /* __ELECTIONGUARD_CPP_TEST_MOCKS_ELECTION_HPP_INCLUDED__ */
