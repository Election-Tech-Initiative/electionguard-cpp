#include "async.hpp"
#include "utils.hpp"
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

    TripleEntry::TripleEntry(unique_ptr<ElementModQ> exp, unique_ptr<ElementModP> g_to_exp,
                             unique_ptr<ElementModP> pubkey_to_exp)
    {
        copy(exp->get(), exp->get() + MAX_Q_LEN, this->exp);
        copy(g_to_exp->get(), g_to_exp->get() + MAX_P_LEN, this->g_to_exp);
        copy(pubkey_to_exp->get(), pubkey_to_exp->get() + MAX_P_LEN, this->pubkey_to_exp);
    }

    unique_ptr<Triple> TripleEntry::get_triple()
    {
        unique_ptr<ElementModQ> temp_exp = make_unique<ElementModQ>(exp);
        unique_ptr<ElementModP> temp_g_to_exp = make_unique<ElementModP>(g_to_exp);
        unique_ptr<ElementModP> temp_pubkey_to_exp = make_unique<ElementModP>(pubkey_to_exp);

        return make_unique<Triple>(move(temp_exp), move(temp_g_to_exp), move(temp_pubkey_to_exp));
    }
 
    QuadrupleEntry::QuadrupleEntry(unique_ptr<ElementModQ> exp1, unique_ptr<ElementModQ> exp2,
                                   unique_ptr<ElementModP> g_to_exp1,
                                   unique_ptr<ElementModP> g_to_exp2_mult_by_pubkey_to_exp1)
    {
        copy(exp1->get(), exp1->get() + MAX_Q_LEN, this->exp1);
        copy(exp2->get(), exp2->get() + MAX_Q_LEN, this->exp2);
        copy(g_to_exp1->get(), g_to_exp1->get() + MAX_P_LEN,
             this->g_to_exp1);
        copy(g_to_exp2_mult_by_pubkey_to_exp1->get(),
             g_to_exp2_mult_by_pubkey_to_exp1->get() + MAX_P_LEN,
             this->g_to_exp2_mult_by_pubkey_to_exp1);
    }

    unique_ptr<Quadruple> QuadrupleEntry::get_quadruple()
    {
        unique_ptr<ElementModQ> temp_exp1 = make_unique<ElementModQ>(exp1);
        unique_ptr<ElementModQ> temp_exp2 = make_unique<ElementModQ>(exp2);
        unique_ptr<ElementModP> temp_g_to_exp1 = make_unique<ElementModP>(g_to_exp1);
        unique_ptr<ElementModP> temp_g_to_exp2_mult_by_pubkey_to_exp1 =
          make_unique<ElementModP>(g_to_exp2_mult_by_pubkey_to_exp1);

        return make_unique<Quadruple>(move(temp_exp1), move(temp_exp2), move(temp_g_to_exp1),
                                 move(temp_g_to_exp2_mult_by_pubkey_to_exp1));
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

                TripleEntry *triple_entry1 = new TripleEntry(triple1->get_exp(),
                    triple1->get_g_to_exp(), triple1->get_pubkey_to_exp());
                getInstance().triple_queue.push(triple_entry1);
               
                TripleEntry *triple_entry2 = new TripleEntry(
                  triple2->get_exp(), triple2->get_g_to_exp(), triple2->get_pubkey_to_exp());
                getInstance().triple_queue.push(triple_entry2);

                QuadrupleEntry *quad_entry = new QuadrupleEntry(quad->get_exp1(),
                    quad->get_exp2(), quad->get_g_to_exp1(),
                    quad->get_g_to_exp2_mult_by_pubkey_to_exp1());
                getInstance().quadruple_queue.push(quad_entry);
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
            TripleEntry *triple_entry1 = getInstance().triple_queue.front();
            unique_ptr<Triple> triple1 = triple_entry1->get_triple();
            delete triple_entry1;
            getInstance().triple_queue.pop();
           
            TripleEntry *triple_entry2 = getInstance().triple_queue.front();
            unique_ptr<Triple> triple2 = triple_entry2->get_triple();
            delete triple_entry2;
            getInstance().triple_queue.pop();

            QuadrupleEntry *quadruple_entry = getInstance().quadruple_queue.front();
            unique_ptr<Quadruple> quad = quadruple_entry->get_quadruple();
            delete quadruple_entry;
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

