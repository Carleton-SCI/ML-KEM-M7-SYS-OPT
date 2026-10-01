.syntax unified
.cpu cortex-m4
.thumb

// void mlkem_public_cache_make_prime_asm(int16_t *out,
//                                        const int16_t *in,
//                                        const int32_t zetas[64])
//
// Builds the "prime" representation consumed by the optimized basemul
// routines. This is the same per-coefficient transform that the original
// matacc cache assembly performs as a side effect, isolated here so hpk_pk
// key generation does not need a dummy matrix-vector multiplication.
.global mlkem_public_cache_make_prime_asm
.type mlkem_public_cache_make_prime_asm, %function
.align 2
mlkem_public_cache_make_prime_asm:
  push {r4-r8, lr}

  outptr   .req r0
  inptr    .req r1
  zetaptr  .req r2
  loop     .req r3
  poly     .req r4
  zeta     .req r5
  tmp      .req r6
  q        .req r7
  qa       .req r8

  movw qa, #26632
  movw q, #3329
  movw loop, #64
1:
  ldr poly, [inptr], #4
  ldr zeta, [zetaptr], #4
  smulwt tmp, zeta, poly
  smlabb tmp, tmp, q, qa
  pkhbt tmp, poly, tmp
  str tmp, [outptr], #4

  neg zeta, zeta
  ldr poly, [inptr], #4
  smulwt tmp, zeta, poly
  smlabb tmp, tmp, q, qa
  pkhbt tmp, poly, tmp
  str tmp, [outptr], #4

  subs.w loop, #1
  bne.w 1b

  .unreq outptr
  .unreq inptr
  .unreq zetaptr
  .unreq loop
  .unreq poly
  .unreq zeta
  .unreq tmp
  .unreq q
  .unreq qa

  pop {r4-r8, pc}

.size mlkem_public_cache_make_prime_asm, .-mlkem_public_cache_make_prime_asm
