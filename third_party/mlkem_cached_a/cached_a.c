#include "api.h"
#include "indcpa.h"
#include "matacc.h"
#include "ntt.h"
#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include "randombytes.h"
#include "symmetric.h"
#include "verify.h"

#include <stdint.h>
#include <string.h>

#define GEN_MATRIX_NBLOCKS                                                   \
  ((12 * KYBER_N / 8 * (1 << 12) / KYBER_Q + XOF_BLOCKBYTES) / XOF_BLOCKBYTES)

#if KYBER_K == 2
#define cached_getnoise_s poly_getnoise_eta1
#define cached_getnoise_e poly_getnoise_eta1
#define cached_addnoise_e_keypair poly_addnoise_eta1
#define cached_addnoise_e_enc poly_addnoise_eta2
#else
#define cached_getnoise_s poly_getnoise
#define cached_getnoise_e poly_getnoise
#define cached_addnoise_e_keypair poly_addnoise
#define cached_addnoise_e_enc poly_addnoise
#endif

#if defined(MLKEM_PUBLIC_CACHE_PRIME_ASM)
extern void mlkem_public_cache_make_prime_asm(int16_t *out,
                                              const int16_t *in,
                                              const int32_t zetas[64]);
#endif

#if defined(MLKEM_CACHED_A_CAPTURE_M7)
extern void mlkem_matacc_capture_16_32_opt_m7(
    int32_t *r_tmp, const int16_t *b, int16_t c[4],
    unsigned char buf[XOF_BLOCKBYTES + 2], const int32_t zetas[64],
    xof_state *state, int16_t *aprimeptr, int16_t *captureptr);
extern void mlkem_matacc_capture_32_32_opt_m7(
    int32_t *r_tmp, const int16_t *b, int16_t c[4],
    unsigned char buf[XOF_BLOCKBYTES + 2], const int32_t zetas[64],
    xof_state *state, int16_t *aprimeptr, int16_t *captureptr);
extern void mlkem_matacc_capture_32_16_opt_m7(
    int16_t *r, const int16_t *b, int16_t c[4],
    unsigned char buf[XOF_BLOCKBYTES + 2], const int32_t zetas[64],
    xof_state *state, int16_t *aprimeptr, const int32_t *r_tmp,
    int16_t *captureptr);

extern void mlkem_matacc_capture_opt_16_32_opt_m7(
    int32_t *r_tmp, const int16_t *b, int16_t c[4],
    unsigned char buf[XOF_BLOCKBYTES + 2], xof_state *state,
    const int16_t *aprimeptr, int16_t *captureptr);
extern void mlkem_matacc_capture_opt_32_32_opt_m7(
    int32_t *r_tmp, const int16_t *b, int16_t c[4],
    unsigned char buf[XOF_BLOCKBYTES + 2], xof_state *state,
    const int16_t *aprimeptr, int16_t *captureptr);
extern void mlkem_matacc_capture_opt_32_16_opt_m7(
    int16_t *r, const int16_t *b, int16_t c[4],
    unsigned char buf[XOF_BLOCKBYTES + 2], xof_state *state,
    const int16_t *aprimeptr, const int32_t *r_tmp, int16_t *captureptr);

#define MLKEM_CAPTURE_CLOBBER()                                               \
    asm volatile(""                                                            \
                 :                                                             \
                 :                                                             \
                 : "s16", "s17", "s18", "s19", "s20", "s21", "s25", "s26", \
                   "s27", "s28", "s29")

static inline void capture_cache_16_32(
    int32_t *r_tmp, const int16_t *b, int16_t c[4],
    unsigned char buf[XOF_BLOCKBYTES + 2], xof_state *state,
    int16_t *aprimeptr, int16_t *captureptr) {
    MLKEM_CAPTURE_CLOBBER();
    mlkem_matacc_capture_16_32_opt_m7(r_tmp, b, c, buf, zetas, state,
                                      aprimeptr, captureptr);
}

static inline void capture_cache_32_32(
    int32_t *r_tmp, const int16_t *b, int16_t c[4],
    unsigned char buf[XOF_BLOCKBYTES + 2], xof_state *state,
    int16_t *aprimeptr, int16_t *captureptr) {
    MLKEM_CAPTURE_CLOBBER();
    mlkem_matacc_capture_32_32_opt_m7(r_tmp, b, c, buf, zetas, state,
                                      aprimeptr, captureptr);
}

static inline void capture_cache_32_16(
    int16_t *r, const int16_t *b, int16_t c[4],
    unsigned char buf[XOF_BLOCKBYTES + 2], xof_state *state,
    int16_t *aprimeptr, const int32_t *r_tmp, int16_t *captureptr) {
    MLKEM_CAPTURE_CLOBBER();
    mlkem_matacc_capture_32_16_opt_m7(r, b, c, buf, zetas, state,
                                      aprimeptr, r_tmp, captureptr);
}

static inline void capture_opt_16_32(
    int32_t *r_tmp, const int16_t *b, int16_t c[4],
    unsigned char buf[XOF_BLOCKBYTES + 2], xof_state *state,
    const int16_t *aprimeptr, int16_t *captureptr) {
    MLKEM_CAPTURE_CLOBBER();
    mlkem_matacc_capture_opt_16_32_opt_m7(r_tmp, b, c, buf, state, aprimeptr,
                                          captureptr);
}

static inline void capture_opt_32_32(
    int32_t *r_tmp, const int16_t *b, int16_t c[4],
    unsigned char buf[XOF_BLOCKBYTES + 2], xof_state *state,
    const int16_t *aprimeptr, int16_t *captureptr) {
    MLKEM_CAPTURE_CLOBBER();
    mlkem_matacc_capture_opt_32_32_opt_m7(r_tmp, b, c, buf, state, aprimeptr,
                                          captureptr);
}

static inline void capture_opt_32_16(
    int16_t *r, const int16_t *b, int16_t c[4],
    unsigned char buf[XOF_BLOCKBYTES + 2], xof_state *state,
    const int16_t *aprimeptr, const int32_t *r_tmp, int16_t *captureptr) {
    MLKEM_CAPTURE_CLOBBER();
    mlkem_matacc_capture_opt_32_16_opt_m7(r, b, c, buf, state, aprimeptr,
                                          r_tmp, captureptr);
}
#endif

static unsigned char *matrix_entry(unsigned char *a_cache, int row, int col) {
    return a_cache + ((row * KYBER_K + col) * KYBER_POLYBYTES);
}

static const unsigned char *matrix_entry_const(const unsigned char *a_cache,
                                               int row, int col) {
    return a_cache + ((row * KYBER_K + col) * KYBER_POLYBYTES);
}

typedef struct {
#if defined(MLKEM_PUBLIC_CACHE_HPK)
    unsigned char hpk[KYBER_SYMBYTES];
#endif
#if defined(MLKEM_PUBLIC_CACHE_PK)
    polyvec pkpv;
#if !defined(MLKEM_CACHED_A_STACK)
    polyvec pkpv_prime;
#endif
#endif
#if !defined(MLKEM_PUBLIC_CACHE_HPK) && !defined(MLKEM_PUBLIC_CACHE_PK)
    unsigned char unused;
#endif
} cached_public_state;

static cached_public_state *public_state(unsigned char *public_cache) {
    return (cached_public_state *)public_cache;
}

static const cached_public_state *
public_state_const(const unsigned char *public_cache) {
    return (const cached_public_state *)public_cache;
}

#if !defined(MLKEM_CACHED_A_CAPTURE_M7)
static unsigned int rej_uniform(int16_t *r,
                                unsigned int len,
                                const unsigned char *buf,
                                unsigned int buflen) {
    unsigned int ctr = 0;
    unsigned int pos = 0;
    uint16_t val0, val1;

    while (ctr < len && pos + 3 <= buflen) {
        val0 = ((buf[pos + 0] >> 0) | ((uint16_t)buf[pos + 1] << 8)) & 0xFFF;
        val1 = ((buf[pos + 1] >> 4) | ((uint16_t)buf[pos + 2] << 4)) & 0xFFF;
        pos += 3;

        if (val0 < KYBER_Q) {
            r[ctr++] = val0;
        }
        if (ctr < len && val1 < KYBER_Q) {
            r[ctr++] = val1;
        }
    }

    return ctr;
}
#endif

#if defined(MLKEM_CACHED_A_CAPTURE_M7)
static void poly_tobytes_noreduce(unsigned char *r, const poly *a) {
    for (int i = 0; i < KYBER_N / 2; i++) {
        uint16_t t0 = (uint16_t)a->coeffs[2 * i];
        uint16_t t1 = (uint16_t)a->coeffs[2 * i + 1];

        r[3 * i] = t0 & 0xff;
        r[3 * i + 1] = (uint8_t)((t0 >> 8) | ((t1 & 0xf) << 4));
        r[3 * i + 2] = (uint8_t)((t1 >> 4) & 0xff);
    }
}

static void matacc_cache32_capture(poly *r, const polyvec *b,
                                   polyvec *b_prime, unsigned char row,
                                   const unsigned char *seed,
                                   unsigned char *a_cache) {
    unsigned char buf[XOF_BLOCKBYTES + 2];
    xof_state state;
    int16_t c[4];
    int32_t r_tmp[KYBER_N];
    poly a;
    int j = 0;

    xof_absorb(&state, seed, (uint8_t)j, row);
    xof_squeezeblocks(buf, 1, &state);
    /* The capture assembly keeps the original matacc multiply path, but also
     * writes each accepted public A coefficient into a.coeffs. */
    capture_cache_16_32(r_tmp, b->vec[j].coeffs, c, buf, &state,
                        b_prime->vec[j].coeffs, a.coeffs);
    poly_tobytes_noreduce(matrix_entry(a_cache, row, j), &a);

    for (j = 1; j < KYBER_K - 1; j++) {
        xof_absorb(&state, seed, (uint8_t)j, row);
        xof_squeezeblocks(buf, 1, &state);
        /* a.coeffs receives the generated A[row][j] coefficients here. */
        capture_cache_32_32(r_tmp, b->vec[j].coeffs, c, buf, &state,
                            b_prime->vec[j].coeffs, a.coeffs);
        poly_tobytes_noreduce(matrix_entry(a_cache, row, j), &a);
    }

    xof_absorb(&state, seed, (uint8_t)j, row);
    xof_squeezeblocks(buf, 1, &state);
    /* Final row component: keep producing pkp while storing A[row][j]. */
    capture_cache_32_16(r->coeffs, b->vec[j].coeffs, c, buf, &state,
                        b_prime->vec[j].coeffs, r_tmp, a.coeffs);
    poly_tobytes_noreduce(matrix_entry(a_cache, row, j), &a);
}

static void matacc_opt32_capture(poly *r, const polyvec *b,
                                 const polyvec *b_prime, unsigned char row,
                                 const unsigned char *seed,
                                 unsigned char *a_cache) {
    unsigned char buf[XOF_BLOCKBYTES + 2];
    xof_state state;
    int16_t c[4];
    int32_t r_tmp[KYBER_N];
    poly a;
    int j = 0;

    xof_absorb(&state, seed, (uint8_t)j, row);
    xof_squeezeblocks(buf, 1, &state);
    /* Same capture mechanism for subsequent rows, using the optimized path
     * that reuses b_prime instead of rebuilding it. */
    capture_opt_16_32(r_tmp, b->vec[j].coeffs, c, buf, &state,
                      b_prime->vec[j].coeffs, a.coeffs);
    poly_tobytes_noreduce(matrix_entry(a_cache, row, j), &a);

    for (j = 1; j < KYBER_K - 1; j++) {
        xof_absorb(&state, seed, (uint8_t)j, row);
        xof_squeezeblocks(buf, 1, &state);
        /* a.coeffs receives the generated A[row][j] coefficients here. */
        capture_opt_32_32(r_tmp, b->vec[j].coeffs, c, buf, &state,
                          b_prime->vec[j].coeffs, a.coeffs);
        poly_tobytes_noreduce(matrix_entry(a_cache, row, j), &a);
    }

    xof_absorb(&state, seed, (uint8_t)j, row);
    xof_squeezeblocks(buf, 1, &state);
    /* Final row component: keep producing pkp while storing A[row][j]. */
    capture_opt_32_16(r->coeffs, b->vec[j].coeffs, c, buf, &state,
                      b_prime->vec[j].coeffs, r_tmp, a.coeffs);
    poly_tobytes_noreduce(matrix_entry(a_cache, row, j), &a);
}
#endif

#if !defined(MLKEM_CACHED_A_CAPTURE_M7)
static void gen_a_cache(unsigned char *a_cache,
                        const unsigned char seed[KYBER_SYMBYTES]) {
    unsigned int ctr, buflen;
    unsigned char buf[GEN_MATRIX_NBLOCKS * XOF_BLOCKBYTES];
    xof_state state;
    poly a;

    for (int i = 0; i < KYBER_K; i++) {
        for (int j = 0; j < KYBER_K; j++) {
            xof_absorb(&state, seed, (uint8_t)j, (uint8_t)i);
            xof_squeezeblocks(buf, GEN_MATRIX_NBLOCKS, &state);

            buflen = GEN_MATRIX_NBLOCKS * XOF_BLOCKBYTES;
            ctr = rej_uniform(a.coeffs, KYBER_N, buf, buflen);

            while (ctr < KYBER_N) {
                xof_squeezeblocks(buf, 1, &state);
                buflen = XOF_BLOCKBYTES;
                ctr += rej_uniform(a.coeffs + ctr, KYBER_N - ctr, buf, buflen);
            }

            xof_ctx_release(&state);
            poly_tobytes(matrix_entry(a_cache, i, j), &a);
        }
    }
}
#endif

#if defined(MLKEM_PUBLIC_CACHE_HPK)
static void public_cache_init_hpk_from_sk(cached_public_state *cache,
                                          const unsigned char *sk) {
    memcpy(cache->hpk, sk + KYBER_SECRETKEYBYTES - 2 * KYBER_SYMBYTES,
           KYBER_SYMBYTES);
}
#endif

#if defined(MLKEM_PUBLIC_CACHE_PK)
static void public_cache_init_pk(cached_public_state *cache,
                                 const unsigned char *pk) {
    polyvec_frombytes(&cache->pkpv, pk);
#if !defined(MLKEM_CACHED_A_STACK)
#if defined(MLKEM_PUBLIC_CACHE_PRIME_ASM)
    for (int i = 0; i < KYBER_K; i++) {
        mlkem_public_cache_make_prime_asm(cache->pkpv_prime.vec[i].coeffs,
                                          cache->pkpv.vec[i].coeffs, zetas);
    }
#else
    {
        poly dummy;
        const unsigned char *seed = pk + KYBER_POLYVECBYTES;

        matacc_cache32(&dummy, &cache->pkpv, &cache->pkpv_prime, 0, seed, 0);
    }
#endif
#endif
}
#endif

static void public_cache_init_from_keypair(cached_public_state *cache,
                                           const unsigned char *pk,
                                           const unsigned char *sk) {
#if defined(MLKEM_PUBLIC_CACHE_HPK)
    public_cache_init_hpk_from_sk(cache, sk);
#else
    (void)sk;
#endif
#if defined(MLKEM_PUBLIC_CACHE_PK)
    public_cache_init_pk(cache, pk);
#else
    (void)pk;
#endif
}

#if defined(MLKEM_PUBLIC_CACHE_HPK)
static inline void public_hpk_from_cache(unsigned char *out,
                                         const cached_public_state *cache) {
    memcpy(out, cache->hpk, KYBER_SYMBYTES);
}
#endif

#if defined(MLKEM_CACHED_A_STACK)
static void matacc_cached_row(poly *r,
                              const polyvec *b,
                              const unsigned char *row) {
    poly_frombytes_mul(r, &b->vec[0], row);
    for (int i = 1; i < KYBER_K; i++) {
        poly_frombytes_mul_acc(r, &b->vec[i], row + i * KYBER_POLYBYTES);
    }
}

static void matacc_cached_transposed_row(poly *r,
                                         const polyvec *b,
                                         const unsigned char *a_cache,
                                         int row) {
    poly_frombytes_mul(r, &b->vec[0], matrix_entry_const(a_cache, 0, row));
    for (int i = 1; i < KYBER_K; i++) {
        poly_frombytes_mul_acc(r, &b->vec[i],
                               matrix_entry_const(a_cache, i, row));
    }
}
#else
static void matacc_cached_row(poly *r,
                              const polyvec *b,
                              const unsigned char *row) {
    int i;
    int32_t r_tmp[KYBER_N];

    poly_frombytes_mul_16_32(r_tmp, &b->vec[0], row);
    for (i = 1; i < KYBER_K - 1; i++) {
        poly_frombytes_mul_32_32(r_tmp, &b->vec[i],
                                 row + i * KYBER_POLYBYTES);
    }
    poly_frombytes_mul_32_16(r, &b->vec[i], row + i * KYBER_POLYBYTES, r_tmp);
}

static void matacc_cached_transposed_row(poly *r,
                                         const polyvec *b,
                                         const unsigned char *a_cache,
                                         int row) {
    int i;
    int32_t r_tmp[KYBER_N];

    poly_frombytes_mul_16_32(r_tmp, &b->vec[0],
                             matrix_entry_const(a_cache, 0, row));
    for (i = 1; i < KYBER_K - 1; i++) {
        poly_frombytes_mul_32_32(r_tmp, &b->vec[i],
                                 matrix_entry_const(a_cache, i, row));
    }
    poly_frombytes_mul_32_16(r, &b->vec[i],
                             matrix_entry_const(a_cache, i, row), r_tmp);
}

#endif

#if defined(MLKEM_PUBLIC_CACHE_PK)
static void matacc_public_cached_pk(poly *r,
                                    const polyvec *sp,
                                    const cached_public_state *cache) {
#if defined(MLKEM_CACHED_A_STACK)
    poly_basemul(r, &cache->pkpv.vec[0], &sp->vec[0]);
    for (int i = 1; i < KYBER_K; i++) {
        poly_basemul_acc(r, &cache->pkpv.vec[i], &sp->vec[i]);
    }
#else
    int i;
    int32_t r_tmp[KYBER_N];

    poly_basemul_opt_16_32(r_tmp, &cache->pkpv.vec[0],
                           &sp->vec[0], &cache->pkpv_prime.vec[0]);
    for (i = 1; i < KYBER_K - 1; i++) {
        poly_basemul_acc_opt_32_32(r_tmp, &cache->pkpv.vec[i],
                                   &sp->vec[i],
                                   &cache->pkpv_prime.vec[i]);
    }
    poly_basemul_acc_opt_32_16(r, &cache->pkpv.vec[i],
                               &sp->vec[i],
                               &cache->pkpv_prime.vec[i], r_tmp);
#endif
}
#endif

#if defined(MLKEM_PUBLIC_CACHE_ENABLED)
static void matacc_regenerated_transposed(poly *r,
                                          const polyvec *sp,
#if !defined(MLKEM_CACHED_A_STACK)
                                          polyvec *sp_prime,
#endif
                                          unsigned char row,
                                          const unsigned char *seed) {
#if defined(MLKEM_CACHED_A_STACK)
    matacc(r, sp, row, seed, 1);
#else
    if (row == 0) {
        matacc_cache32(r, sp, sp_prime, row, seed, 1);
    } else {
        matacc_opt32(r, sp, sp_prime, row, seed, 1);
    }
#endif
}

static void matacc_regenerated_pk(poly *r,
                                  poly *scratch,
                                  const polyvec *sp,
#if !defined(MLKEM_CACHED_A_STACK)
                                  const polyvec *sp_prime,
#endif
                                  const unsigned char *pk,
                                  const cached_public_state *public_cache) {
#if defined(MLKEM_PUBLIC_CACHE_PK)
    if (public_cache != 0) {
        matacc_public_cached_pk(r, sp, public_cache);
        return;
    }
#else
    (void)public_cache;
#endif

#if defined(MLKEM_CACHED_A_STACK)
    poly_frombytes(scratch, pk);
    poly_basemul(r, scratch, &sp->vec[0]);
    for (int i = 1; i < KYBER_K; i++) {
        poly_frombytes(scratch, pk + i * KYBER_POLYBYTES);
        poly_basemul_acc(r, scratch, &sp->vec[i]);
    }
#else
    int i;
    int32_t r_tmp[KYBER_N];

    poly_frombytes(scratch, pk);
    poly_basemul_opt_16_32(r_tmp, &sp->vec[0], scratch, &sp_prime->vec[0]);
    for (i = 1; i < KYBER_K - 1; i++) {
        poly_frombytes(scratch, pk + i * KYBER_POLYBYTES);
        poly_basemul_acc_opt_32_32(r_tmp, &sp->vec[i], scratch,
                                   &sp_prime->vec[i]);
    }
    poly_frombytes(scratch, pk + i * KYBER_POLYBYTES);
    poly_basemul_acc_opt_32_16(r, &sp->vec[i], scratch, &sp_prime->vec[i],
                               r_tmp);
#endif
}

static void indcpa_enc_public_internal(unsigned char *c,
                                       const unsigned char *m,
                                       const unsigned char *pk,
                                       const cached_public_state *public_cache,
                                       const unsigned char *coins) {
    polyvec sp;
#if !defined(MLKEM_CACHED_A_STACK)
    polyvec sp_prime;
#endif
    poly bp;
    poly *k = &bp;
    poly *v = &sp.vec[0];
    const unsigned char *seed = pk + KYBER_POLYVECBYTES;
    unsigned char nonce = 0;

    for (int i = 0; i < KYBER_K; i++) {
        cached_getnoise_e(sp.vec + i, coins, nonce++);
    }

    polyvec_ntt(&sp);

    for (int i = 0; i < KYBER_K; i++) {
        matacc_regenerated_transposed(&bp, &sp,
#if !defined(MLKEM_CACHED_A_STACK)
                                      &sp_prime,
#endif
                                      i, seed);
        poly_invntt(&bp);
        cached_addnoise_e_enc(&bp, coins, nonce++);
        poly_reduce(&bp);
        poly_packcompress(c, &bp, i);
    }

    matacc_regenerated_pk(v, &bp, &sp,
#if !defined(MLKEM_CACHED_A_STACK)
                          &sp_prime,
#endif
                          pk, public_cache);
    poly_invntt(v);
    cached_addnoise_e_enc(v, coins, nonce++);

    poly_frommsg(k, m);
    poly_add(v, v, k);
    poly_reduce(v);

    poly_compress(c + KYBER_POLYVECCOMPRESSEDBYTES, v);
}

#if defined(MLKEM_PUBLIC_CACHE_PK)
static unsigned char
indcpa_enc_cmp_public_internal(const unsigned char *c,
                               const unsigned char *m,
                               const unsigned char *pk,
                               const cached_public_state *public_cache,
                               const unsigned char *coins) {
    uint64_t rc = 0;
    polyvec sp;
#if !defined(MLKEM_CACHED_A_STACK)
    polyvec sp_prime;
#endif
    poly bp;
    poly *k = &bp;
    poly *v = &sp.vec[0];
    const unsigned char *seed = pk + KYBER_POLYVECBYTES;
    unsigned char nonce = 0;

    for (int i = 0; i < KYBER_K; i++) {
        cached_getnoise_e(sp.vec + i, coins, nonce++);
    }

    polyvec_ntt(&sp);

    for (int i = 0; i < KYBER_K; i++) {
        matacc_regenerated_transposed(&bp, &sp,
#if !defined(MLKEM_CACHED_A_STACK)
                                      &sp_prime,
#endif
                                      i, seed);
        poly_invntt(&bp);
        cached_addnoise_e_enc(&bp, coins, nonce++);
        poly_reduce(&bp);
        rc |= cmp_poly_packcompress(c, &bp, i);
    }

    matacc_regenerated_pk(v, &bp, &sp,
#if !defined(MLKEM_CACHED_A_STACK)
                          &sp_prime,
#endif
                          pk, public_cache);
    poly_invntt(v);
    cached_addnoise_e_enc(v, coins, nonce++);

    poly_frommsg(k, m);
    poly_add(v, v, k);
    poly_reduce(v);

    rc |= cmp_poly_compress(c + KYBER_POLYVECCOMPRESSEDBYTES, v);
    rc = ~rc + 1;
    rc >>= 63;

    return (unsigned char)rc;
}
#endif
#endif

static void indcpa_keypair_derand_cached_a(unsigned char *pk,
                                           unsigned char *sk,
                                           unsigned char *a_cache,
                                           const unsigned char *coins) {
    polyvec skpv;
#if defined(MLKEM_CACHED_A_CAPTURE_M7)
    polyvec skpv_prime;
#endif
    poly pkp;
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char *publicseed = buf;
    unsigned char *noiseseed = buf + KYBER_SYMBYTES;
    unsigned char nonce = 0;

    memcpy(buf, coins, KYBER_SYMBYTES);
    buf[KYBER_SYMBYTES] = KYBER_K;
    hash_g(buf, buf, KYBER_SYMBYTES + 1);

#if !defined(MLKEM_CACHED_A_CAPTURE_M7)
    gen_a_cache(a_cache, publicseed);
#endif

    for (int i = 0; i < KYBER_K; i++) {
        cached_getnoise_s(skpv.vec + i, noiseseed, nonce++);
    }

    polyvec_ntt(&skpv);

    for (int i = 0; i < KYBER_K; i++) {
#if defined(MLKEM_CACHED_A_CAPTURE_M7)
        if (i == 0) {
            matacc_cache32_capture(&pkp, &skpv, &skpv_prime, 0, publicseed,
                                   a_cache);
        } else {
            matacc_opt32_capture(&pkp, &skpv, &skpv_prime, (unsigned char)i,
                                 publicseed, a_cache);
        }
#else
        matacc_cached_row(&pkp, &skpv,
                          matrix_entry_const(a_cache, i, 0));
#endif
        poly_invntt(&pkp);
        cached_addnoise_e_keypair(&pkp, noiseseed, nonce++);
        poly_ntt(&pkp);
        poly_tobytes(pk + i * KYBER_POLYBYTES, &pkp);
    }

    polyvec_tobytes(sk, &skpv);
    memcpy(pk + KYBER_POLYVECBYTES, publicseed, KYBER_SYMBYTES);
}

static void indcpa_enc_cached_a_internal(unsigned char *c,
                                         const unsigned char *m,
                                         const unsigned char *pk,
                                         const unsigned char *a_cache,
                                         const cached_public_state *public_cache,
                                         const unsigned char *coins) {
    polyvec sp;
    poly bp;
    poly *k = &bp;
    poly *v = &sp.vec[0];
    unsigned char nonce = 0;

    for (int i = 0; i < KYBER_K; i++) {
        cached_getnoise_e(sp.vec + i, coins, nonce++);
    }

    polyvec_ntt(&sp);

    for (int i = 0; i < KYBER_K; i++) {
        matacc_cached_transposed_row(&bp, &sp, a_cache, i);
        poly_invntt(&bp);
        cached_addnoise_e_enc(&bp, coins, nonce++);
        poly_reduce(&bp);
        poly_packcompress(c, &bp, i);
    }

#if defined(MLKEM_PUBLIC_CACHE_PK)
    if (public_cache != 0) {
        matacc_public_cached_pk(v, &sp, public_cache);
    } else
#else
    (void)public_cache;
#endif
    {
        matacc_cached_row(v, &sp, pk);
    }
    poly_invntt(v);
    cached_addnoise_e_enc(v, coins, nonce++);

    poly_frommsg(k, m);
    poly_add(v, v, k);
    poly_reduce(v);

    poly_compress(c + KYBER_POLYVECCOMPRESSEDBYTES, v);
}

static unsigned char indcpa_enc_cmp_cached_a_internal(const unsigned char *c,
                                                      const unsigned char *m,
                                                      const unsigned char *pk,
                                                      const unsigned char *a_cache,
                                                      const cached_public_state *public_cache,
                                                      const unsigned char *coins) {
    uint64_t rc = 0;
    polyvec sp;
    poly bp;
    poly *k = &bp;
    poly *v = &sp.vec[0];
    unsigned char nonce = 0;

    for (int i = 0; i < KYBER_K; i++) {
        cached_getnoise_e(sp.vec + i, coins, nonce++);
    }

    polyvec_ntt(&sp);

    for (int i = 0; i < KYBER_K; i++) {
        matacc_cached_transposed_row(&bp, &sp, a_cache, i);
        poly_invntt(&bp);
        cached_addnoise_e_enc(&bp, coins, nonce++);
        poly_reduce(&bp);
        rc |= cmp_poly_packcompress(c, &bp, i);
    }

#if defined(MLKEM_PUBLIC_CACHE_PK)
    if (public_cache != 0) {
        matacc_public_cached_pk(v, &sp, public_cache);
    } else
#else
    (void)public_cache;
#endif
    {
        matacc_cached_row(v, &sp, pk);
    }
    poly_invntt(v);
    cached_addnoise_e_enc(v, coins, nonce++);

    poly_frommsg(k, m);
    poly_add(v, v, k);
    poly_reduce(v);

    rc |= cmp_poly_compress(c + KYBER_POLYVECCOMPRESSEDBYTES, v);
    rc = ~rc + 1;
    rc >>= 63;

    return (unsigned char)rc;
}

static unsigned char indcpa_enc_cmp_cached_a(const unsigned char *c,
                                             const unsigned char *m,
                                             const unsigned char *pk,
                                             const unsigned char *a_cache,
                                             const unsigned char *coins) {
    return indcpa_enc_cmp_cached_a_internal(c, m, pk, a_cache, 0, coins);
}

static int crypto_kem_keypair_derand_cached_a(unsigned char *pk,
                                              unsigned char *sk,
                                              unsigned char *a_cache,
                                              const unsigned char *coins) {
    indcpa_keypair_derand_cached_a(pk, sk, a_cache, coins);
    memcpy(sk + KYBER_INDCPA_SECRETKEYBYTES, pk, KYBER_PUBLICKEYBYTES);
    hash_h(sk + KYBER_SECRETKEYBYTES - 2 * KYBER_SYMBYTES,
           pk, KYBER_PUBLICKEYBYTES);
    memcpy(sk + KYBER_SECRETKEYBYTES - KYBER_SYMBYTES,
           coins + KYBER_SYMBYTES, KYBER_SYMBYTES);
    return 0;
}

int crypto_kem_keypair_cached_a(unsigned char *pk,
                                unsigned char *sk,
                                unsigned char *a_cache) {
    unsigned char coins[2 * KYBER_SYMBYTES];

    randombytes(coins, 2 * KYBER_SYMBYTES);
    crypto_kem_keypair_derand_cached_a(pk, sk, a_cache, coins);
    return 0;
}

int crypto_kem_keypair_cached_a_public(unsigned char *pk,
                                       unsigned char *sk,
                                       unsigned char *a_cache,
                                       unsigned char *public_cache) {
    unsigned char coins[2 * KYBER_SYMBYTES];

    randombytes(coins, 2 * KYBER_SYMBYTES);
    crypto_kem_keypair_derand_cached_a(pk, sk, a_cache, coins);
    public_cache_init_from_keypair(public_state(public_cache), pk, sk);
    return 0;
}

#if defined(MLKEM_PUBLIC_CACHE_ENABLED)
int crypto_kem_keypair_public(unsigned char *pk,
                              unsigned char *sk,
                              unsigned char *public_cache) {
    crypto_kem_keypair(pk, sk);
    public_cache_init_from_keypair(public_state(public_cache), pk, sk);
    return 0;
}

static int crypto_kem_enc_derand_public(unsigned char *ct,
                                        unsigned char *ss,
                                        const unsigned char *pk,
                                        const cached_public_state *public_cache,
                                        const unsigned char *coins) {
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char kr[2 * KYBER_SYMBYTES];

    memcpy(buf, coins, KYBER_SYMBYTES);
#if defined(MLKEM_PUBLIC_CACHE_HPK)
    public_hpk_from_cache(buf + KYBER_SYMBYTES, public_cache);
#else
    hash_h(buf + KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);
#endif
    hash_g(kr, buf, 2 * KYBER_SYMBYTES);

    indcpa_enc_public_internal(ct, buf, pk, public_cache,
                               kr + KYBER_SYMBYTES);

    memcpy(ss, kr, KYBER_SYMBYTES);
    return 0;
}

int crypto_kem_enc_public(unsigned char *ct,
                          unsigned char *ss,
                          const unsigned char *pk,
                          const unsigned char *public_cache) {
    unsigned char coins[KYBER_SYMBYTES];

    randombytes(coins, KYBER_SYMBYTES);
    crypto_kem_enc_derand_public(ct, ss, pk, public_state_const(public_cache),
                                 coins);
    return 0;
}

int crypto_kem_dec_public(unsigned char *ss,
                          const unsigned char *ct,
                          const unsigned char *sk,
                          const unsigned char *public_cache) {
#if defined(MLKEM_PUBLIC_CACHE_PK)
    int fail;
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char kr[2 * KYBER_SYMBYTES];
    const unsigned char *pk = sk + KYBER_INDCPA_SECRETKEYBYTES;
    const cached_public_state *cache = public_state_const(public_cache);

    indcpa_dec(buf, ct, sk);

    memcpy(buf + KYBER_SYMBYTES,
           sk + KYBER_SECRETKEYBYTES - 2 * KYBER_SYMBYTES,
           KYBER_SYMBYTES);
    hash_g(kr, buf, 2 * KYBER_SYMBYTES);

    fail = indcpa_enc_cmp_public_internal(ct, buf, pk, cache,
                                          kr + KYBER_SYMBYTES);

    rkprf(ss, sk + KYBER_SECRETKEYBYTES - KYBER_SYMBYTES, ct);
    cmov(ss, kr, KYBER_SYMBYTES, (uint8_t)(1 - fail));

    return 0;
#else
    (void)public_cache;
    return crypto_kem_dec(ss, ct, sk);
#endif
}
#endif

static int crypto_kem_enc_derand_cached_a_internal(unsigned char *ct,
                                                   unsigned char *ss,
                                                   const unsigned char *pk,
                                                   const unsigned char *a_cache,
                                                   const cached_public_state *public_cache,
                                                   const unsigned char *coins) {
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char kr[2 * KYBER_SYMBYTES];

    memcpy(buf, coins, KYBER_SYMBYTES);
#if defined(MLKEM_PUBLIC_CACHE_HPK)
    if (public_cache != 0) {
        public_hpk_from_cache(buf + KYBER_SYMBYTES, public_cache);
    } else
#endif
    {
        hash_h(buf + KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);
    }
    hash_g(kr, buf, 2 * KYBER_SYMBYTES);

    indcpa_enc_cached_a_internal(ct, buf, pk, a_cache, public_cache,
                                 kr + KYBER_SYMBYTES);

    memcpy(ss, kr, KYBER_SYMBYTES);
    return 0;
}

static int crypto_kem_enc_derand_cached_a(unsigned char *ct,
                                          unsigned char *ss,
                                          const unsigned char *pk,
                                          const unsigned char *a_cache,
                                          const unsigned char *coins) {
    return crypto_kem_enc_derand_cached_a_internal(ct, ss, pk, a_cache, 0,
                                                   coins);
}

int crypto_kem_enc_cached_a(unsigned char *ct,
                            unsigned char *ss,
                            const unsigned char *pk,
                            const unsigned char *a_cache) {
    unsigned char coins[KYBER_SYMBYTES];

    randombytes(coins, KYBER_SYMBYTES);
    crypto_kem_enc_derand_cached_a(ct, ss, pk, a_cache, coins);
    return 0;
}

int crypto_kem_enc_cached_a_public(unsigned char *ct,
                                   unsigned char *ss,
                                   const unsigned char *pk,
                                   const unsigned char *a_cache,
                                   const unsigned char *public_cache) {
    unsigned char coins[KYBER_SYMBYTES];

    randombytes(coins, KYBER_SYMBYTES);
    crypto_kem_enc_derand_cached_a_internal(ct, ss, pk, a_cache,
                                            public_state_const(public_cache),
                                            coins);
    return 0;
}

int crypto_kem_dec_cached_a(unsigned char *ss,
                            const unsigned char *ct,
                            const unsigned char *sk,
                            const unsigned char *a_cache) {
    int fail;
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char kr[2 * KYBER_SYMBYTES];
    const unsigned char *pk = sk + KYBER_INDCPA_SECRETKEYBYTES;

    indcpa_dec(buf, ct, sk);

    memcpy(buf + KYBER_SYMBYTES,
           sk + KYBER_SECRETKEYBYTES - 2 * KYBER_SYMBYTES,
           KYBER_SYMBYTES);
    hash_g(kr, buf, 2 * KYBER_SYMBYTES);

    fail = indcpa_enc_cmp_cached_a(ct, buf, pk, a_cache,
                                   kr + KYBER_SYMBYTES);

    rkprf(ss, sk + KYBER_SECRETKEYBYTES - KYBER_SYMBYTES, ct);
    cmov(ss, kr, KYBER_SYMBYTES, (uint8_t)(1 - fail));

    return 0;
}

int crypto_kem_dec_cached_a_public(unsigned char *ss,
                                   const unsigned char *ct,
                                   const unsigned char *sk,
                                   const unsigned char *a_cache,
                                   const unsigned char *public_cache) {
    int fail;
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char kr[2 * KYBER_SYMBYTES];
    const unsigned char *pk = sk + KYBER_INDCPA_SECRETKEYBYTES;
    const cached_public_state *cache = public_state_const(public_cache);

    indcpa_dec(buf, ct, sk);

    memcpy(buf + KYBER_SYMBYTES,
           sk + KYBER_SECRETKEYBYTES - 2 * KYBER_SYMBYTES,
           KYBER_SYMBYTES);
    hash_g(kr, buf, 2 * KYBER_SYMBYTES);

    fail = indcpa_enc_cmp_cached_a_internal(ct, buf, pk, a_cache, cache,
                                            kr + KYBER_SYMBYTES);

    rkprf(ss, sk + KYBER_SECRETKEYBYTES - KYBER_SYMBYTES, ct);
    cmov(ss, kr, KYBER_SYMBYTES, (uint8_t)(1 - fail));

    return 0;
}
