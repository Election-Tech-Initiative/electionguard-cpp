#ifndef __ELECTIONGUARD_CPP_PRECOMPUTE_BUFFERS_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_PRECOMPUTE_BUFFERS_HPP_INCLUDED__

#include "..\..\src\electionguard\async.hpp"
#include "electionguard/group.hpp"


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

namespace electionguard
{
    /// <summary>
    /// </summary>
    class EG_INTERNAL_API Triple
    {
      unique_ptr<ElementModQ> rho;
      unique_ptr<ElementModP> g_to_rho;
      unique_ptr<ElementModP> pubkey_to_rho;

      public:
      explicit Triple(const ElementModP &publicKey) { generateTriple(publicKey); }
      Triple() { }

      unique_ptr<ElementModQ> get_rho()
      {
          unique_ptr<ElementModQ> new_rho = make_unique<ElementModQ>(*rho);
          return move(new_rho);
      }

      unique_ptr<ElementModP> get_g_to_rho()
      {
          unique_ptr<ElementModP> new_g_to_rho = make_unique<ElementModP>(*g_to_rho);
          return move(new_g_to_rho);
      }

      unique_ptr<ElementModP> get_pubkey_to_rho()
      {
          unique_ptr<ElementModP> new_pubkey_to_rho = make_unique<ElementModP>(*pubkey_to_rho);
          return move(new_pubkey_to_rho);
      }

      void set(unique_ptr<ElementModQ> in_rho,
               unique_ptr<ElementModP> in_g_to_rho,
               unique_ptr<ElementModP> in_pubkey_to_rho);
    protected:
        void generateTriple(const ElementModP &publicKey);
    };
    
    class EG_INTERNAL_API Quadruple
    {
        unique_ptr<ElementModQ> sigma;
        unique_ptr<ElementModQ> rho;
        unique_ptr<ElementModP> g_to_rho;
        unique_ptr<ElementModP> g_to_sigma_mult_by_pubkey_to_rho;

      public:
        explicit Quadruple(const ElementModP &publicKey)
        {
            generateQuadruple(publicKey);
        }
        Quadruple() {};

        unique_ptr<ElementModQ> get_sigma()
        {
            unique_ptr<ElementModQ> new_sigma = make_unique<ElementModQ>(*sigma);
            return move(new_sigma);
        }

        unique_ptr<ElementModQ> get_rho()
        {
            unique_ptr<ElementModQ> new_rho = make_unique<ElementModQ>(*rho);
            return move(new_rho);
        }

        unique_ptr<ElementModP> get_g_to_rho()
        {
            unique_ptr<ElementModP> new_g_to_rho = make_unique<ElementModP>(*g_to_rho);
            return move(new_g_to_rho);
        }

        unique_ptr<ElementModP> get_g_to_sigma_mult_by_pubkey_to_rho()
        {
            unique_ptr<ElementModP> new_g_to_sigma_mult_by_pubkey_to_rho =
              make_unique<ElementModP>(*g_to_sigma_mult_by_pubkey_to_rho);
            return move(new_g_to_sigma_mult_by_pubkey_to_rho);
        }

        void set(unique_ptr<ElementModQ> in_rho, unique_ptr<ElementModQ> in_sigma,
                 unique_ptr<ElementModP> in_g_to_rho,
                 unique_ptr<ElementModP> in_g_to_sigma_mult_by_pubkey_to_rho);

      protected:
        void generateQuadruple(const ElementModP &publicKey);
    };

    class EG_INTERNAL_API TripleEntry
    {
      public:
        explicit TripleEntry() {}

        uint64_t *get_rho() { return rho; }
        uint64_t *get_g_to_rho() { return g_to_rho; }
        uint64_t *get_pubkey_to_rho() { return pubkey_to_rho; }

        void set(unique_ptr<Triple> triple);
        unique_ptr<Triple> get_triple();

      private:
        uint64_t rho[MAX_Q_LEN];
        uint64_t g_to_rho[MAX_P_LEN];
        uint64_t pubkey_to_rho[MAX_P_LEN];
    };
    
    class EG_INTERNAL_API QuadrupleEntry
    {
      public:
        explicit QuadrupleEntry() {}

        uint64_t *get_rho() { return rho; }
        uint64_t *get_sigma() { return sigma; }
        uint64_t *get_g_to_rho() { return g_to_rho; }
        uint64_t *get_g_to_sigma_mult_by_pubkey_to_rho()
        {
            return g_to_sigma_mult_by_pubkey_to_rho;
        }

        void set(unique_ptr<Quadruple> quadruple);
        unique_ptr<Quadruple> get_quadruple();

      private:
        uint64_t rho[MAX_Q_LEN];
        uint64_t sigma[MAX_Q_LEN];
        uint64_t g_to_rho[MAX_P_LEN];
        uint64_t g_to_sigma_mult_by_pubkey_to_rho[MAX_P_LEN];
    };
    
    class EG_INTERNAL_API TwoTriplesAndAQuadruple
    {
        unique_ptr<Triple> triple1;
        unique_ptr<Triple> triple2;
        unique_ptr<Quadruple> quad;
        bool populated = false;

      public:
        explicit TwoTriplesAndAQuadruple() { populated = false; }

        unique_ptr<Triple> get_triple1()
        {
            unique_ptr<Triple> new_triple1 = make_unique<Triple>();
            new_triple1->set(triple1->get_rho(), triple1->get_g_to_rho(),
                triple1->get_pubkey_to_rho());
            return move(new_triple1);
        }

        unique_ptr<Triple> get_triple2()
        {
            unique_ptr<Triple> new_triple2 = make_unique<Triple>();
            new_triple2->set(triple2->get_rho(), triple2->get_g_to_rho(),
                triple2->get_pubkey_to_rho());
            return move(new_triple2);
        }

        unique_ptr<Quadruple> get_quad()
        {
            unique_ptr<Quadruple> new_quad = make_unique<Quadruple>();
            new_quad->set(quad->get_rho(), quad->get_sigma(), quad->get_g_to_rho(),
                          quad->get_g_to_sigma_mult_by_pubkey_to_rho());
            return move(new_quad);
        }

        bool isPopulated()
        {
            return populated;
        }

        void set(unique_ptr<Triple> in_triple1, unique_ptr<Triple> in_triple2,
                 unique_ptr<Quadruple> in_quad);
    };
    
    /// <summary>
    /// A singleton context for a collection of precomputed triples and quadruples.
    /// </summary>
    class EG_INTERNAL_API PrecomputeBufferContext
    {
      public:
        PrecomputeBufferContext(const PrecomputeBufferContext &) = delete;
        PrecomputeBufferContext(PrecomputeBufferContext &&) = delete;

      private:
        PrecomputeBufferContext() {}
        ~PrecomputeBufferContext() {}

      public:
        static PrecomputeBufferContext &getInstance()
        {
            static PrecomputeBufferContext instance;
            return instance;
        }

        /// <summary>
        /// The populate method populates the precomputations queues with
        /// values used by encryptSelection. The function is stopped by calling
        /// stop_populate. Pre-computed values are currently computed by generating
        /// two triples and a quad. We do this because two triples and a quad
        /// are need for an encryptSelection.The triple queue is twice the size of 
        /// the quad queue. We use two different queues in case we need to
        /// make the stop more granular at some point, in other words currently
        /// if we call stop it will finish the two triples and a quad before
        /// stopping. If we want it to stop more granularly (for example after
        /// each queue item then we can do that but it makes the code a bit
        /// more complicated). In anticipation of possibly needing this we
        /// use two queues.
        ///
        /// <param name="elgamalPublicKey">the elgamal public key for the election</param>
        /// <param name="size_of_queue">by default the quad queue size is 500, so
        ///                             1000 triples, if the caller wants the
        ///                             queue size to be different then this
        ///                             parameter is used</param>
        /// <returns>void</returns>
        /// </summary>
        /// 
        static void
        populate(const ElementModP &elgamalPublicKey, uint32_t size_of_queue = 0);

        /// <summary>
        /// The stop_populate method stops the population of the
        /// precomputations queues started by the populate method.
        /// <returns>void</returns>
        /// </summary>
        /// 
        static void stop_populate();
        
        static std::unique_ptr<TwoTriplesAndAQuadruple> getTwoTriplesAndAQuadruple();

      private:
        //AsyncSemaphore queue_lock;
        std::mutex queue_lock;
        bool populate_OK = false;
        std::queue<TripleEntry *> triple_queue;
        std::queue<QuadrupleEntry *> quadruple_queue;
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_PRECOMPUTE_BUFFERS_HPP_INCLUDED__ */
