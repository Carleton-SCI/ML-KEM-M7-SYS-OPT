#ifndef API_H
#define API_H

#include "params.h"

#define CRYPTO_SECRETKEYBYTES  KYBER_SECRETKEYBYTES
#define CRYPTO_PUBLICKEYBYTES  KYBER_PUBLICKEYBYTES
#define CRYPTO_CIPHERTEXTBYTES KYBER_CIPHERTEXTBYTES
#define CRYPTO_BYTES           KYBER_SSBYTES

#define CRYPTO_ALGNAME "Kyber768"
#define CRYPTO_HAS_CACHED_A 1
#define CRYPTO_HAS_PUBLIC_CACHE 1
#if defined(MLKEM_PUBLIC_CACHE_HPK) && defined(MLKEM_PUBLIC_CACHE_PK)
#if defined(MLKEM_CACHED_A_STACK)
#define KYBER_PUBLICCACHEBYTES (KYBER_SYMBYTES + KYBER_K * KYBER_N * 2)
#else
#define KYBER_PUBLICCACHEBYTES (KYBER_SYMBYTES + 2 * KYBER_K * KYBER_N * 2)
#endif
#elif defined(MLKEM_PUBLIC_CACHE_HPK)
#define KYBER_PUBLICCACHEBYTES KYBER_SYMBYTES
#elif defined(MLKEM_PUBLIC_CACHE_PK)
#if defined(MLKEM_CACHED_A_STACK)
#define KYBER_PUBLICCACHEBYTES (KYBER_K * KYBER_N * 2)
#else
#define KYBER_PUBLICCACHEBYTES (2 * KYBER_K * KYBER_N * 2)
#endif
#else
#define KYBER_PUBLICCACHEBYTES 0
#endif

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);

int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

int crypto_kem_keypair_public(unsigned char *pk, unsigned char *sk,
                              unsigned char *public_cache);

int crypto_kem_enc_public(unsigned char *ct, unsigned char *ss,
                          const unsigned char *pk,
                          const unsigned char *public_cache);

int crypto_kem_dec_public(unsigned char *ss, const unsigned char *ct,
                          const unsigned char *sk,
                          const unsigned char *public_cache);

int crypto_kem_keypair_cached_a(unsigned char *pk, unsigned char *sk,
                                unsigned char *a_cache);

int crypto_kem_enc_cached_a(unsigned char *ct, unsigned char *ss,
                            const unsigned char *pk,
                            const unsigned char *a_cache);

int crypto_kem_dec_cached_a(unsigned char *ss, const unsigned char *ct,
                            const unsigned char *sk,
                            const unsigned char *a_cache);

int crypto_kem_keypair_cached_a_public(unsigned char *pk, unsigned char *sk,
                                       unsigned char *a_cache,
                                       unsigned char *public_cache);

int crypto_kem_enc_cached_a_public(unsigned char *ct, unsigned char *ss,
                                   const unsigned char *pk,
                                   const unsigned char *a_cache,
                                   const unsigned char *public_cache);

int crypto_kem_dec_cached_a_public(unsigned char *ss, const unsigned char *ct,
                                   const unsigned char *sk,
                                   const unsigned char *a_cache,
                                   const unsigned char *public_cache);

#endif
