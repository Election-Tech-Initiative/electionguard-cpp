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
    void Triple::generateTriple(const ElementModP &publicKey)
    {
        // generate a random rho
        rho = rand_q();
        g_to_rho = g_pow_p(*rho);
        pubkey_to_rho = pow_mod_p(publicKey, *rho);
    }

    void Triple::set(unique_ptr<ElementModQ> in_rho, unique_ptr<ElementModP> in_g_to_rho,
                     unique_ptr<ElementModP> in_pubkey_to_rho)
    {
        rho = move(in_rho);
        g_to_rho = move(in_g_to_rho);
        pubkey_to_rho = move(in_pubkey_to_rho);
    }

    void Quadruple::generateQuadruple(const ElementModP &publicKey)
    {
        // generate a random sigma and rho
        sigma = rand_q();
        rho = rand_q();
        g_to_rho = g_pow_p(*rho);
        g_to_sigma_mult_by_pubkey_to_rho = mul_mod_p(*g_pow_p(*sigma), *pow_mod_p(publicKey, *rho));
    }

    void Quadruple::set(unique_ptr<ElementModQ> in_rho, unique_ptr<ElementModQ> in_sigma,
                        unique_ptr<ElementModP> in_g_to_rho,
                        unique_ptr<ElementModP> in_g_to_sigma_mult_by_pubkey_to_rho)
    {
        rho = move(in_rho);
        sigma = move(in_sigma);
        g_to_rho = move(in_g_to_rho);
        g_to_sigma_mult_by_pubkey_to_rho = move(in_g_to_sigma_mult_by_pubkey_to_rho);
    }

    void TripleEntry::set(unique_ptr<Triple> triple)
    {
        unique_ptr<ElementModQ> p_rho = triple->get_rho();
        unique_ptr<ElementModP> p_g_to_rho = triple->get_g_to_rho();
        unique_ptr<ElementModP> p_pubkey_to_rho = triple->get_pubkey_to_rho();
        uint64_t *p_rho_u64 = p_rho->get();
        uint64_t *p_g_to_rho_u64 = p_g_to_rho->get();
        uint64_t *p_pubkey_to_rho_u64 = p_pubkey_to_rho->get();
        copy(p_rho_u64, p_rho_u64 + MAX_Q_LEN, rho);
        copy(p_g_to_rho_u64, p_g_to_rho_u64 + MAX_P_LEN, g_to_rho);
        copy(p_pubkey_to_rho_u64, p_pubkey_to_rho_u64 + MAX_P_LEN, pubkey_to_rho);
    }

    unique_ptr<Triple> TripleEntry::get_triple()
    {
        unique_ptr<Triple> result = make_unique<Triple>();
        
        unique_ptr<ElementModQ> temp_rho = make_unique<ElementModQ>(rho);
        unique_ptr<ElementModP> temp_g_to_rho = make_unique<ElementModP>(g_to_rho);
        unique_ptr<ElementModP> temp_pubkey_to_rho = make_unique<ElementModP>(pubkey_to_rho);

        result->set(move(temp_rho), move(temp_g_to_rho), move(temp_pubkey_to_rho));
        
        return result;
    }
    
    void QuadrupleEntry::set(unique_ptr<Quadruple> quadruple)
    { 
        unique_ptr<ElementModQ> p_sigma = quadruple->get_sigma();
        unique_ptr<ElementModQ> p_rho = quadruple->get_rho();
        unique_ptr<ElementModP> p_g_to_rho = quadruple->get_g_to_rho();
        unique_ptr<ElementModP> p_g_to_sigma_mult_by_pubkey_to_rho =
          quadruple->get_g_to_sigma_mult_by_pubkey_to_rho();
        uint64_t *p_sigma_u64 = p_sigma->get();
        uint64_t *p_rho_u64 = p_rho->get();
        uint64_t *p_g_to_rho_u64 = p_g_to_rho->get();
        uint64_t *p_g_to_sigma_mult_by_pubkey_to_rho_u64 = p_g_to_sigma_mult_by_pubkey_to_rho->get();
        copy(p_sigma_u64, p_sigma_u64 + MAX_Q_LEN, sigma);
        copy(p_rho_u64, p_rho_u64 + MAX_Q_LEN, rho);
        copy(p_g_to_rho_u64, p_g_to_rho_u64 + MAX_P_LEN, g_to_rho);
        copy(p_g_to_sigma_mult_by_pubkey_to_rho_u64,
             p_g_to_sigma_mult_by_pubkey_to_rho_u64 + MAX_P_LEN, g_to_sigma_mult_by_pubkey_to_rho);
    }
    
    unique_ptr<Quadruple> QuadrupleEntry::get_quadruple()
    {
        unique_ptr<Quadruple> result = make_unique<Quadruple>();

        unique_ptr<ElementModQ> temp_rho = make_unique<ElementModQ>(rho);
        unique_ptr<ElementModQ> temp_sigma = make_unique<ElementModQ>(sigma);
        unique_ptr<ElementModP> temp_g_to_rho = make_unique<ElementModP>(g_to_rho);
        unique_ptr<ElementModP> temp_g_to_sigma_mult_by_pubkey_to_rho =
          make_unique<ElementModP>(g_to_sigma_mult_by_pubkey_to_rho);

        result->set(move(temp_rho), move(temp_sigma), move(temp_g_to_rho),
                    move(temp_g_to_sigma_mult_by_pubkey_to_rho));

        return result;
    }
 
    void TwoTriplesAndAQuadruple::set(unique_ptr<Triple> in_triple1,
                                      unique_ptr<Triple> in_triple2,
                                      unique_ptr<Quadruple> in_quad)
    {
        triple1 = move(in_triple1);
        triple2 = move(in_triple2);
        quad = move(in_quad);
        populated = true;
    }
    
    void PrecomputeBufferContext::populate(
      const ElementModP &elgamalPublicKey,
      uint32_t size_of_queue /* = 0 */)
    {       
        // default size of quadruple_queue will be 500
        if (size_of_queue != 0) {
            getInstance().max = size_of_queue;
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
                unique_ptr<Triple> triple1;
                unique_ptr<Triple> triple2;
                unique_ptr<Quadruple> quad;

                // generate two triples and a quadruple
                triple1 = make_unique<Triple>(elgamalPublicKey);
                TripleEntry *triple_entry1 = new TripleEntry();
                triple_entry1->set(move(triple1));
                getInstance().triple_queue.push(triple_entry1);
               
                triple2 = make_unique<Triple>(elgamalPublicKey);
                TripleEntry *triple_entry2 = new TripleEntry();
                triple_entry2->set(move(triple2));
                getInstance().triple_queue.push(triple_entry2);

                quad = make_unique<Quadruple>(elgamalPublicKey);
                QuadrupleEntry *quad_entry = new QuadrupleEntry();
                quad_entry->set(move(quad));
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
            
            result->set(move(triple1), move(triple2), move(quad));
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

