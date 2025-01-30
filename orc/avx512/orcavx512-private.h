#ifndef ORC_AVX512_PRIVATE
#define ORC_AVX512_PRIVATE

#include <orc/orcavx-internal.h>

ORC_BEGIN_DECLS

/* orcavx512.c */
ORC_INTERNAL void orc_avx512_init (void);
ORC_INTERNAL const char * orc_avx512_get_flag_name (OrcTargetAVX512Flags flag);

/* orcavx512target.c */
ORC_INTERNAL OrcTarget * orc_avx512_target_init (void);


ORC_END_DECLS

#endif
