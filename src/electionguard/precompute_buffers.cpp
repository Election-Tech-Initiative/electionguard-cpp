#include "async.hpp"
#include "utils.hpp"
#include "../kremlin/Lib_Memzero0.h"
#include "electionguard/group.hpp"
#include <electionguard/precompute_buffers.hpp>

#include <array>
#include <cstdint>
#include <electionguard/constants.h>
#include <electionguard/export.h>
#include <iomanip>
#include <iostream>
#include <memory>

using std::begin;
using std::copy;
using std::end;
using std::unique_ptr;
using std::make_unique;
using std::begin;
using std::copy;
using std::end;

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
        return make_unique<Triple>(exp->clone(), g_to_exp->clone(),
                                   pubkey_to_exp->clone());
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
        this->g_to_exp2_mult_by_pubkey_to_exp1 =
          move(quadruple.g_to_exp2_mult_by_pubkey_to_exp1);
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
        return make_unique<TwoTriplesAndAQuadruple>(triple1->clone(),
                                                    triple2->clone(),
                                                    quad->clone());
    }
    
    void PrecomputeBufferContext::populate(
      const ElementModP &elgamalPublicKey,
      uint32_t size_of_queue /* = 0 */)
    {       
        // default size of quadruple_queue will be 5000
        if (size_of_queue != 0) {
            getInstance().max = size_of_queue;
        } else {
            getInstance().max = 5000;
        }

        getInstance().populate_OK = true;

        // This loop goes through until the queues are full but can be stopped
        // between generations of two triples and a quad. By full it means
        // we check how many quads are in the queue, to start with we will
        // try 500 and see how that works. If the vendor wanted to pass the
        // queue size in we could use that.
        // for now we just go through the loop once
        do {
            // TODO - Can we tolerate not returning until we have finished
            // generating two triples and a quadruple?
            // If not we can get more elaborate with the populate_OK checking
            getInstance().queue_lock.lock();
            if (getInstance().populate_OK) {
                // generate two triples and a quadruple
                // 
                // these constuctors will generate the precomputed values
                unique_ptr<Triple> triple1 = make_unique<Triple>(elgamalPublicKey);
                unique_ptr<Triple> triple2 = make_unique<Triple>(elgamalPublicKey);
                unique_ptr<Quadruple> quad = make_unique<Quadruple>(elgamalPublicKey);

                getInstance().triple_queue.push(move(triple1));
               
                getInstance().triple_queue.push(move(triple2));

                getInstance().quadruple_queue.push(move(quad));

                getInstance().queue_lock.unlock();
            } else {
                getInstance().queue_lock.unlock();
                return;
            }
        } while (getInstance().quadruple_queue.size() < getInstance().max);
    }

    void PrecomputeBufferContext::stop_populate()
    {
        getInstance().queue_lock.lock();
        getInstance().populate_OK = false;
        getInstance().queue_lock.unlock();
    }

    uint32_t PrecomputeBufferContext::get_max_queue_size()
    {
        return getInstance().max;
    }

    uint32_t PrecomputeBufferContext::get_current_queue_size()
    {
        return getInstance().quadruple_queue.size();
    }
      
    std::unique_ptr<TwoTriplesAndAQuadruple> PrecomputeBufferContext::getTwoTriplesAndAQuadruple()
    {
        unique_ptr<TwoTriplesAndAQuadruple> result = make_unique<TwoTriplesAndAQuadruple>();

        // take a lock while we get the triples and a quadruple
        getInstance().queue_lock.lock();

        // make sure there are enough in the queues
        if ((getInstance().triple_queue.size() >= 2) && (getInstance().quadruple_queue.size() >= 1)) {

            unique_ptr<Triple> triple1 = std::move(getInstance().triple_queue.front());
            getInstance().triple_queue.pop();
            unique_ptr<Triple> triple2 = std::move(getInstance().triple_queue.front());
            getInstance().triple_queue.pop();
           
            unique_ptr<Quadruple> quad = std::move(getInstance().quadruple_queue.front());
            getInstance().quadruple_queue.pop();

            result = make_unique<TwoTriplesAndAQuadruple>(move(triple1),
                                                          move(triple2),
                                                          move(quad));
        }
        getInstance().queue_lock.unlock();

        return result;
    }

    void PrecomputeBufferContext::empty_queues()
    { 
        getInstance().queue_lock.lock();
        uint32_t size = getInstance().quadruple_queue.size();
        for (int i = 0; i < (int)size; i++) {
            getInstance().triple_queue.pop();
            getInstance().triple_queue.pop();
            getInstance().quadruple_queue.pop();
        }
        getInstance().queue_lock.unlock();
    }

} // namespace electionguard

