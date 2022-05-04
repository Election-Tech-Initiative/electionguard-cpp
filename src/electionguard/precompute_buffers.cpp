#include "../kremlin/Lib_Memzero0.h"
#include "async.hpp"
#include "electionguard/group.hpp"
#include "utils.hpp"

#include <array>
#include <cstdint>
#include <electionguard/constants.h>
#include <electionguard/export.h>
#include <electionguard/precompute_buffers.hpp>
#include <iomanip>
#include <iostream>
#include <memory>

using std::begin;
using std::copy;
using std::end;
using std::lock_guard;
using std::make_unique;
using std::unique_ptr;

namespace electionguard
{
    Triple::Triple(unique_ptr<ElementModQ> exp, unique_ptr<ElementModP> g_to_exp,
                   unique_ptr<ElementModP> pubkey_to_exp)
    {
        this->exp = move(exp);
        this->g_to_exp = move(g_to_exp);
        this->pubkey_to_exp = move(pubkey_to_exp);
    }

    Triple::Triple(const Triple &triple)
    {
        this->exp = triple.exp->clone();
        this->g_to_exp = triple.g_to_exp->clone();
        this->pubkey_to_exp = triple.pubkey_to_exp->clone();
    }

    Triple::Triple(Triple &&triple)
    {
        this->exp = move(triple.exp);
        this->g_to_exp = move(triple.g_to_exp);
        this->pubkey_to_exp = move(triple.pubkey_to_exp);
    }

    Triple::~Triple() = default;

    Triple &Triple::operator=(const Triple &triple)
    {
        this->exp = triple.exp->clone();
        this->g_to_exp = triple.g_to_exp->clone();
        this->pubkey_to_exp = triple.pubkey_to_exp->clone();
        return *this;
    }

    Triple &Triple::operator=(Triple &&triple)
    {
        this->exp = move(triple.exp);
        this->g_to_exp = move(triple.g_to_exp);
        this->pubkey_to_exp = move(triple.pubkey_to_exp);
        return *this;
    }

    void Triple::generateTriple(const ElementModP &publicKey)
    {
        // generate a random rho
        exp = rand_q();
        g_to_exp = g_pow_p(*exp);
        pubkey_to_exp = pow_mod_p(publicKey, *exp);
    }

    unique_ptr<Triple> Triple::clone()
    {
        return make_unique<Triple>(exp->clone(), g_to_exp->clone(), pubkey_to_exp->clone());
    }

    Quadruple::Quadruple(unique_ptr<ElementModQ> exp1, unique_ptr<ElementModQ> exp2,
                         unique_ptr<ElementModP> g_to_exp1,
                         unique_ptr<ElementModP> g_to_exp2_mult_by_pubkey_to_exp1)
    {
        this->exp1 = move(exp1);
        this->exp2 = move(exp2);
        this->g_to_exp1 = move(g_to_exp1);
        this->g_to_exp2_mult_by_pubkey_to_exp1 = move(g_to_exp2_mult_by_pubkey_to_exp1);
    }

    Quadruple::Quadruple(const Quadruple &quadruple)
    {
        this->exp1 = quadruple.exp1->clone();
        this->exp2 = quadruple.exp2->clone();
        this->g_to_exp1 = quadruple.g_to_exp1->clone();
        this->g_to_exp2_mult_by_pubkey_to_exp1 =
          quadruple.g_to_exp2_mult_by_pubkey_to_exp1->clone();
    }

    Quadruple::Quadruple(Quadruple &&quadruple)
    {
        this->exp1 = move(quadruple.exp1);
        this->exp2 = move(quadruple.exp2);
        this->g_to_exp1 = move(quadruple.g_to_exp1);
        this->g_to_exp2_mult_by_pubkey_to_exp1 = move(quadruple.g_to_exp2_mult_by_pubkey_to_exp1);
    }

    Quadruple::~Quadruple() = default;

    Quadruple &Quadruple::operator=(const Quadruple &quadruple)
    {
        this->exp1 = quadruple.exp1->clone();
        this->exp2 = quadruple.exp2->clone();
        this->g_to_exp1 = quadruple.g_to_exp1->clone();
        this->g_to_exp2_mult_by_pubkey_to_exp1 =
          quadruple.g_to_exp2_mult_by_pubkey_to_exp1->clone();
        return *this;
    }

    Quadruple &Quadruple::operator=(Quadruple &&quadruple)
    {
        this->exp1 = move(quadruple.exp1);
        this->exp2 = move(quadruple.exp2);
        this->g_to_exp1 = move(quadruple.g_to_exp1);
        this->g_to_exp2_mult_by_pubkey_to_exp1 = move(quadruple.g_to_exp2_mult_by_pubkey_to_exp1);
        return *this;
    }

    void Quadruple::generateQuadruple(const ElementModP &publicKey)
    {
        // generate a random sigma and rho
        exp1 = rand_q();
        exp2 = rand_q();
        g_to_exp1 = g_pow_p(*exp1);
        g_to_exp2_mult_by_pubkey_to_exp1 = mul_mod_p(*g_pow_p(*exp2), *pow_mod_p(publicKey, *exp1));
    }

    unique_ptr<Quadruple> Quadruple::clone()
    {
        return make_unique<Quadruple>(exp1->clone(), exp2->clone(), g_to_exp1->clone(),
                                      g_to_exp2_mult_by_pubkey_to_exp1->clone());
    }

    TwoTriplesAndAQuadruple::TwoTriplesAndAQuadruple(unique_ptr<Triple> triple1,
                                                     unique_ptr<Triple> triple2,
                                                     unique_ptr<Quadruple> quad)
    {
        this->triple1 = move(triple1);
        this->triple2 = move(triple2);
        this->quad = move(quad);
        populated = true;
    }

    TwoTriplesAndAQuadruple::TwoTriplesAndAQuadruple(const TwoTriplesAndAQuadruple &other)
    {
        this->triple1 = other.triple1->clone();
        this->triple2 = other.triple2->clone();
        this->quad = other.quad->clone();
    }

    TwoTriplesAndAQuadruple::TwoTriplesAndAQuadruple(TwoTriplesAndAQuadruple &&other)
    {
        this->triple1 = move(other.triple1);
        this->triple2 = move(other.triple2);
        this->quad = move(other.quad);
    }

    TwoTriplesAndAQuadruple::~TwoTriplesAndAQuadruple() = default;

    TwoTriplesAndAQuadruple &
    TwoTriplesAndAQuadruple::operator=(const TwoTriplesAndAQuadruple &other)
    {
        this->triple1 = other.triple1->clone();
        this->triple2 = other.triple2->clone();
        this->quad = other.quad->clone();
        return *this;
    }

    TwoTriplesAndAQuadruple &TwoTriplesAndAQuadruple::operator=(TwoTriplesAndAQuadruple &&other)
    {
        this->triple1 = move(other.triple1);
        this->triple2 = move(other.triple2);
        this->quad = move(other.quad);
        return *this;
    }

    unique_ptr<TwoTriplesAndAQuadruple> TwoTriplesAndAQuadruple::clone()
    {
        return make_unique<TwoTriplesAndAQuadruple>(triple1->clone(), triple2->clone(),
                                                    quad->clone());
    }

    void PrecomputeBufferContext::init(uint32_t size_of_queue /* = 0 */)
    {
        std::lock_guard<std::mutex> lock(queue_lock);
        getInstance().populate_OK = true;

        // default size of quadruple_queue will be 5000
        if (size_of_queue != 0) {
            getInstance().max = size_of_queue;
        } else {
            getInstance().max = DEFAULT_PRECOMPUTE_SIZE;
        }
    }

    void PrecomputeBufferContext::populate(const ElementModP &elgamalPublicKey)
    {
        // This loop goes through until the queues are full but can be stopped
        // between generations of two triples and a quad. By full it means
        // we check how many quads are in the queue, to start with we will
        // try 5000 and see how that works. If the vendor wanted to pass the
        // queue size in we could use that.
        // for now we just go through the loop once
        do {
            // TODO - Can we tolerate not returning until we have finished
            // generating two triples and a quadruple?
            // If not we can get more elaborate with the populate_OK checking
            std::lock_guard<std::mutex> lock(queue_lock);
            int iteration_count = 0;
            if (getInstance().populate_OK) {
                // generate two triples and a quadruple
                //
                // these constuctors will generate the precomputed values
                unique_ptr<Triple> triple1 = make_unique<Triple>(elgamalPublicKey);
                unique_ptr<Triple> triple2 = make_unique<Triple>(elgamalPublicKey);
                unique_ptr<Quadruple> quad = make_unique<Quadruple>(elgamalPublicKey);

                unique_ptr<TwoTriplesAndAQuadruple> twoTriplesAndAQuadruple =
                  make_unique<TwoTriplesAndAQuadruple>(move(triple1), move(triple2), move(quad));

                getInstance().twoTriplesAndAQuadruple_queue.push(move(twoTriplesAndAQuadruple));

                // This is very rudimentary. We can add a more complex algorithm in
                // the future, that would look at the queues and increase production if one
                // is getting lower than expected.
                // Every third iteration we generate two extra triples, one for use with
                // the contest constant chaum pedersen proof and one for hashed elgamal encryption
                // we need less of these because this exponentiation is done only every contest
                // encryption whereas the two triples and a quadruple is used every selection
                // encryption. The generating two triples every third iteration is a guess
                // on how many precomputes we will need.
                if ((iteration_count % 3) == 0) {
                    unique_ptr<Triple> contest_triple1 = make_unique<Triple>(elgamalPublicKey);
                    getInstance().triple_queue.push(move(contest_triple1));
                    unique_ptr<Triple> contest_triple2 = make_unique<Triple>(elgamalPublicKey);
                    getInstance().triple_queue.push(move(contest_triple2));
                }
                iteration_count++;
            } else {
                return;
            }
        } while (getInstance().twoTriplesAndAQuadruple_queue.size() < getInstance().max);
    }

    void PrecomputeBufferContext::stop_populate() { getInstance().populate_OK = false; }

    uint32_t PrecomputeBufferContext::get_max_queue_size() { return getInstance().max; }

    uint32_t PrecomputeBufferContext::get_current_queue_size()
    {
        return getInstance().twoTriplesAndAQuadruple_queue.size();
    }

    std::unique_ptr<TwoTriplesAndAQuadruple> PrecomputeBufferContext::getTwoTriplesAndAQuadruple()
    {
        unique_ptr<TwoTriplesAndAQuadruple> result = make_unique<TwoTriplesAndAQuadruple>();

        // take a lock while we get the triples and a quadruple
        std::lock_guard<std::mutex> lock(queue_lock);

        // make sure there are enough in the queues
        if (!getInstance().twoTriplesAndAQuadruple_queue.empty()) {
            result = std::move(getInstance().twoTriplesAndAQuadruple_queue.front());
            getInstance().twoTriplesAndAQuadruple_queue.pop();
        }

        return result;
    }

    std::unique_ptr<Triple> PrecomputeBufferContext::getTriple()
    {
        unique_ptr<Triple> result = nullptr;

        // take a lock while we get the triples and a quadruple
        std::lock_guard<std::mutex> lock(queue_lock);

        // make sure there are enough in the queues
        if (!getInstance().triple_queue.empty()) {
            result = std::move(getInstance().triple_queue.front());
            getInstance().triple_queue.pop();
        }

        return result;
    }

    void PrecomputeBufferContext::empty_queues()
    {
        std::lock_guard<std::mutex> lock(queue_lock);
        uint32_t triple_size = getInstance().triple_queue.size();
        for (int i = 0; i < (int)triple_size; i++) {
            getInstance().triple_queue.pop();
        }
        uint32_t twoTriplesAndAQuadruple_size = getInstance().twoTriplesAndAQuadruple_queue.size();
        for (int i = 0; i < (int)twoTriplesAndAQuadruple_size; i++) {
            getInstance().twoTriplesAndAQuadruple_queue.pop();
        }
    }

    std::mutex PrecomputeBufferContext::queue_lock;

} // namespace electionguard
