ifeq ($(subdir),math)
libm-sysdep_routines += s_floor-c s_ceil-c s_floorf-c s_ceilf-c \
			s_rint-c s_rintf-c s_nearbyint-c s_nearbyintf-c \
			s_trunc-c s_truncf-c

libm-sysdep_routines += s_ceil-sse4_1 s_ceilf-sse4_1 s_floor-sse4_1 \
			s_floorf-sse4_1 s_nearbyint-sse4_1 \
			s_nearbyintf-sse4_1 s_rint-sse4_1 s_rintf-sse4_1 \
			s_trunc-sse4_1 s_truncf-sse4_1

libm-sysdep_routines += e_exp-fma e_log-fma e_pow-fma s_atan-fma \
			e_asin-fma e_atan2-fma s_sin-fma s_tan-fma \
			mplog-fma mpa-fma slowexp-fma slowpow-fma \
			sincos32-fma doasin-fma dosincos-fma \
			halfulp-fma mpexp-fma \
			mpatan2-fma mpatan-fma mpsqrt-fma mptan-fma

CFLAGS-doasin-fma.c = -mfma -mavx2
CFLAGS-dosincos-fma.c = -mfma -mavx2
CFLAGS-e_asin-fma.c = -mfma -mavx2
CFLAGS-e_atan2-fma.c = -mfma -mavx2
CFLAGS-e_exp-fma.c = -mfma -mavx2
CFLAGS-e_log-fma.c = -mfma -mavx2
CFLAGS-e_pow-fma.c = -mfma -mavx2 $(config-cflags-nofma)
CFLAGS-halfulp-fma.c = -mfma -mavx2
CFLAGS-mpa-fma.c = -mfma -mavx2
CFLAGS-mpatan-fma.c = -mfma -mavx2
CFLAGS-mpatan2-fma.c = -mfma -mavx2
CFLAGS-mpexp-fma.c = -mfma -mavx2
CFLAGS-mplog-fma.c = -mfma -mavx2
CFLAGS-mpsqrt-fma.c = -mfma -mavx2
CFLAGS-mptan-fma.c = -mfma -mavx2
CFLAGS-s_atan-fma.c = -mfma -mavx2
CFLAGS-sincos32-fma.c = -mfma -mavx2
CFLAGS-slowexp-fma.c = -mfma -mavx2
CFLAGS-slowpow-fma.c = -mfma -mavx2
CFLAGS-s_sin-fma.c = -mfma -mavx2
CFLAGS-s_tan-fma.c = -mfma -mavx2

libm-sysdep_routines += s_sinf-sse2 s_cosf-sse2 s_sincosf-sse2

libm-sysdep_routines += e_exp2f-fma e_expf-fma e_log2f-fma e_logf-fma \
			e_powf-fma s_sinf-fma s_cosf-fma s_sincosf-fma

CFLAGS-e_exp2f-fma.c = -mfma -mavx2
CFLAGS-e_expf-fma.c = -mfma -mavx2
CFLAGS-e_log2f-fma.c = -mfma -mavx2
CFLAGS-e_logf-fma.c = -mfma -mavx2
CFLAGS-e_powf-fma.c = -mfma -mavx2
CFLAGS-s_sinf-fma.c = -mfma -mavx2
CFLAGS-s_cosf-fma.c = -mfma -mavx2
CFLAGS-s_sincosf-fma.c = -mfma -mavx2

libm-sysdep_routines += e_exp-fma4 e_log-fma4 e_pow-fma4 s_atan-fma4 \
			e_asin-fma4 e_atan2-fma4 s_sin-fma4 s_tan-fma4 \
			mplog-fma4 mpa-fma4 slowexp-fma4 slowpow-fma4 \
			sincos32-fma4 doasin-fma4 dosincos-fma4 \
			halfulp-fma4 mpexp-fma4 \
			mpatan2-fma4 mpatan-fma4 mpsqrt-fma4 mptan-fma4

CFLAGS-doasin-fma4.c = -mfma4
CFLAGS-dosincos-fma4.c = -mfma4
CFLAGS-e_asin-fma4.c = -mfma4
CFLAGS-e_atan2-fma4.c = -mfma4
CFLAGS-e_exp-fma4.c = -mfma4
CFLAGS-e_log-fma4.c = -mfma4
CFLAGS-e_pow-fma4.c = -mfma4 $(config-cflags-nofma)
CFLAGS-halfulp-fma4.c = -mfma4
CFLAGS-mpa-fma4.c = -mfma4
CFLAGS-mpatan-fma4.c = -mfma4
CFLAGS-mpatan2-fma4.c = -mfma4
CFLAGS-mpexp-fma4.c = -mfma4
CFLAGS-mplog-fma4.c = -mfma4
CFLAGS-mpsqrt-fma4.c = -mfma4
CFLAGS-mptan-fma4.c = -mfma4
CFLAGS-s_atan-fma4.c = -mfma4
CFLAGS-sincos32-fma4.c = -mfma4
CFLAGS-slowexp-fma4.c = -mfma4
CFLAGS-slowpow-fma4.c = -mfma4
CFLAGS-s_sin-fma4.c = -mfma4
CFLAGS-s_tan-fma4.c = -mfma4

libm-sysdep_routines += e_exp-avx e_log-avx s_atan-avx \
			e_atan2-avx s_sin-avx s_tan-avx \
			mplog-avx mpa-avx slowexp-avx \
			mpexp-avx

sse2avx-flags = -msse2avx -DSSE2AVX
ifeq ($(with-clang),yes)
sse2avx-flags = -DSSE2AVX
endif

CFLAGS-e_atan2-avx.c = $(sse2avx-flags)
CFLAGS-e_exp-avx.c = $(sse2avx-flags)
CFLAGS-e_log-avx.c = $(sse2avx-flags)
CFLAGS-mpa-avx.c = $(sse2avx-flags)
CFLAGS-mpexp-avx.c = $(sse2avx-flags)
CFLAGS-mplog-avx.c = $(sse2avx-flags)
CFLAGS-s_atan-avx.c = $(sse2avx-flags)
CFLAGS-s_sin-avx.c = $(sse2avx-flags)
CFLAGS-slowexp-avx.c = $(sse2avx-flags)
CFLAGS-s_tan-avx.c = $(sse2avx-flags)
endif

ifeq ($(subdir),mathvec)
libmvec-sysdep_routines += svml_d_cos2_core_sse4 svml_d_cos4_core_avx2 \
			   svml_d_cos8_core_avx512 svml_d_sin2_core_sse4 \
			   svml_d_sin4_core_avx2 svml_d_sin8_core_avx512 \
			   svml_d_log2_core_sse4 svml_d_log4_core_avx2 \
			   svml_d_log8_core_avx512 svml_d_sincos2_core_sse4 \
			   svml_d_sincos4_core_avx2 svml_d_sincos8_core_avx512 \
			   svml_s_cosf4_core_sse4 svml_s_cosf8_core_avx2 \
			   svml_s_cosf16_core_avx512 svml_s_sinf4_core_sse4 \
			   svml_s_sinf8_core_avx2 svml_s_sinf16_core_avx512 \
			   svml_s_logf4_core_sse4 svml_s_logf8_core_avx2 \
			   svml_s_logf16_core_avx512 svml_d_exp2_core_sse4 \
			   svml_d_exp4_core_avx2 svml_d_exp8_core_avx512 \
			   svml_s_expf4_core_sse4 svml_s_expf8_core_avx2 \
			   svml_s_expf16_core_avx512 svml_d_pow2_core_sse4 \
			   svml_d_pow4_core_avx2 svml_d_pow8_core_avx512 \
			   svml_s_powf4_core_sse4 svml_s_powf8_core_avx2 \
			   svml_s_powf16_core_avx512 svml_s_sincosf4_core_sse4 \
			   svml_s_sincosf8_core_avx2 \
			   svml_s_sincosf16_core_avx512 \
			   svml_d_cos2_core-sse2 svml_d_cos4_core-sse \
			   svml_d_cos8_core-avx2 svml_d_exp2_core-sse2 \
			   svml_d_exp4_core-sse svml_d_exp8_core-avx2 \
			   svml_d_log2_core-sse2 svml_d_log4_core-sse \
			   svml_d_log8_core-avx2 svml_d_pow2_core-sse2 \
			   svml_d_pow4_core-sse svml_d_pow8_core-avx2 \
			   svml_d_sin2_core-sse2 svml_d_sin4_core-sse \
			   svml_d_sin8_core-avx2 \
			   svml_d_sincos2_core-sse2 \
			   svml_d_sincos4_core-sse \
			   svml_d_sincos8_core-avx2 \
			   svml_s_cosf16_core-avx2 \
			   svml_s_cosf4_core-sse2 \
			   svml_s_cosf8_core-sse \
			   svml_s_expf16_core-avx2 \
			   svml_s_expf4_core-sse2 \
			   svml_s_expf8_core-sse \
			   svml_s_logf16_core-avx2 \
			   svml_s_logf4_core-sse2 \
			   svml_s_logf8_core-sse \
			   svml_s_powf16_core-avx2 \
			   svml_s_powf4_core-sse2 \
			   svml_s_powf8_core-sse \
			   svml_s_sincosf16_core-avx2 \
			   svml_s_sincosf4_core-sse2 \
			   svml_s_sincosf8_core-sse \
			   svml_s_sinf16_core-avx2 \
			   svml_s_sinf4_core-sse2 \
			   svml_s_sinf8_core-sse
endif
