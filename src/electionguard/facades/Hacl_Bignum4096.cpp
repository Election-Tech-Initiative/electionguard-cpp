#include "Hacl_Bignum4096.hpp"

#include "../../karamel/Hacl_Bignum4096.h"
#ifdef _WIN32
#include "../../karamel/Hacl_GenericField32.h"
#endif // _WIN32
#include "../../karamel/Hacl_GenericField64.h"
#include "../log.hpp"

using electionguard::Log;

namespace hacl
{
#ifdef _WIN32
    Bignum4096::Bignum4096(uint32_t *elem)
    {
        HaclBignumContext4096 ctx{Hacl_Bignum4096_32_mont_ctx_init(elem)};
        context = std::move(ctx);
    }
#else
    Bignum4096::Bignum4096(uint64_t *elem)
    {
        HaclBignumContext4096 ctx{Hacl_Bignum4096_mont_ctx_init(elem)};
        context = std::move(ctx);
    }
#endif // _WIN32
    Bignum4096::~Bignum4096() {}

    uint64_t Bignum4096::add(uint64_t *a, uint64_t *b, uint64_t *res)
    {
#ifdef _WIN32
        return Hacl_Bignum4096_32_add(reinterpret_cast<uint32_t *>(a),
                                      reinterpret_cast<uint32_t *>(b),
                                      reinterpret_cast<uint32_t *>(res));
#else
        return Hacl_Bignum4096_add(a, b, res);
#endif // _WIN32
    }

    uint64_t Bignum4096::sub(uint64_t *a, uint64_t *b, uint64_t *res)
    {
#ifdef _WIN32
        return Hacl_Bignum4096_32_sub(reinterpret_cast<uint32_t *>(a),
                                      reinterpret_cast<uint32_t *>(b),
                                      reinterpret_cast<uint32_t *>(res));
#else
        return Hacl_Bignum4096_sub(a, b, res);
#endif // _WIN32
    }

    void Bignum4096::mul(uint64_t *a, uint64_t *b, uint64_t *res)
    {
#ifdef _WIN32
        Hacl_Bignum4096_32_mul(reinterpret_cast<uint32_t *>(a),
                               reinterpret_cast<uint32_t *>(b),
                               reinterpret_cast<uint32_t *>(res));
#else
        Hacl_Bignum4096_mul(a, b, res);
#endif // _WIN32
    }

    bool Bignum4096::mod(uint64_t *n, uint64_t *a, uint64_t *res)
    {
        
#ifdef _WIN32
        return Hacl_Bignum4096_32_mod(reinterpret_cast<uint32_t *>(n),
                                      reinterpret_cast<uint32_t *>(a),
                                      reinterpret_cast<uint32_t *>(res));
#else
        return Hacl_Bignum4096_mod(n, a, res);
#endif // _WIN32
    }

    bool Bignum4096::modExp(uint64_t *n, uint64_t *a, uint32_t bBits, uint64_t *b, uint64_t *res,
                            bool useConstTime /* = true */)
    { 
        if (bBits <= 0) {
            Log::trace("Bignum4096::modExp:: bbits <= 0");
            return false;
        }
        if (useConstTime) {
#ifdef _WIN32
            return Hacl_Bignum4096_32_mod_exp_consttime(reinterpret_cast<uint32_t *>(n),
                                                        reinterpret_cast<uint32_t *>(a), bBits,
                                                        reinterpret_cast<uint32_t *>(b),
                                                        reinterpret_cast<uint32_t *>(res));
#else
            return Hacl_Bignum4096_mod_exp_consttime(n, a, bBits, b, res);
#endif // WIN32
        }
#ifdef _WIN32
        return Hacl_Bignum4096_32_mod_exp_vartime(reinterpret_cast<uint32_t *>(n),
                                                  reinterpret_cast<uint32_t *>(a),
                                                  bBits,
                                                  reinterpret_cast<uint32_t *>(b),
                                                  reinterpret_cast<uint32_t *>(res));
#else
        return Hacl_Bignum4096_mod_exp_vartime(n, a, bBits, b, res);
#endif // _WIN32
    }

    uint64_t *Bignum4096::fromBytes(uint32_t len, uint8_t *bytes)
    {
        return Hacl_Bignum4096_new_bn_from_bytes_be(len, bytes);
    }

    void Bignum4096::toBytes(uint64_t *bytes, uint8_t *res)
    {
        return Hacl_Bignum4096_bn_to_bytes_be(bytes, res);
    }

    uint64_t Bignum4096::lessThan(uint64_t *a, uint64_t *b)
    {
        return Hacl_Bignum4096_lt_mask(a, b);
    }

    void Bignum4096::mod(uint64_t *a, uint64_t *res) const
    {
#ifdef _WIN32
        Hacl_Bignum4096_32_mod_precomp(context.get(), reinterpret_cast<uint32_t *>(a),
                                       reinterpret_cast<uint32_t *>(res));
#else
        Hacl_Bignum4096_mod_precomp(context.get(), a, res);
#endif // _WIN32
    }

    void Bignum4096::modExp(uint64_t *a, uint32_t bBits, uint64_t *b, uint64_t *res,
                            bool useConstTime /* = true */) const
    {
#ifdef _WIN32
        if (bBits <= 0) {
            Log::trace("Bignum4096::modExp:: bbits <= 0");
            return throw;
        }
        if (useConstTime) {
            return Hacl_Bignum4096_32_mod_exp_consttime_precomp(context.get(),
                                                                reinterpret_cast<uint32_t *>(a), bBits,
                                                                reinterpret_cast<uint32_t *>(b),
                                                                reinterpret_cast<uint32_t *>(res));
        }
        return Hacl_Bignum4096_32_mod_exp_vartime_precomp(context.get(), reinterpret_cast<uint32_t *>(a),
                                                          bBits, reinterpret_cast<uint32_t *>(b),
                                                          reinterpret_cast<uint32_t *>(res));
#else
        if (bBits <= 0) {
            Log::trace("Bignum4096::modExp:: bbits <= 0");
            return throw;
        }
        if (useConstTime) {
            return Hacl_Bignum4096_mod_exp_consttime_precomp(context.get(), a, bBits, b, res);
        }
        return Hacl_Bignum4096_mod_exp_vartime_precomp(context.get(), a, bBits, b, res);
#endif // _WIN32
    }

    void Bignum4096::to_montgomery_form(uint64_t *a, uint64_t *aM) const
    {
#ifdef _WIN32
        Hacl_GenericField32_to_field(context.get(), reinterpret_cast<uint32_t *>(a),
                                     reinterpret_cast<uint32_t *>(aM));
#else
        Hacl_GenericField64_to_field(context.get(), a, aM);
#endif // _WIN32
    }

    void Bignum4096::from_montgomery_form(uint64_t *aM, uint64_t *a) const
    {
#ifdef _WIN32
        Hacl_GenericField32_from_field(context.get(), reinterpret_cast<uint32_t *>(aM),
                                       reinterpret_cast<uint32_t *>(a));
#else
        Hacl_GenericField64_from_field(context.get(), aM, a);
#endif // _WIN32
    }

    void Bignum4096::montgomery_mod_mul_stay_in_mont_form(uint64_t *aM, uint64_t *bM, uint64_t *cM) const
    {
#ifdef _WIN32
        //Log::debug("32 BIT ####################### MUL");
        Hacl_GenericField32_mul(context.get(), reinterpret_cast<uint32_t *>(aM),
                                reinterpret_cast<uint32_t *>(bM), reinterpret_cast<uint32_t *>(cM));
#else
        Hacl_GenericField64_mul(context.get(), aM, bM, cM);
#endif // _WIN32
    }

    const Bignum4096 &CONTEXT_P()
    {
#ifdef _WIN32
        static Bignum4096 instance{(uint32_t*)(P_ARRAY_REVERSE)};
#else
        static Bignum4096 instance{const_cast<uint64_t *>(P_ARRAY_REVERSE)};
#endif // _WIN32
        return instance;
    }
} // namespace hacl
