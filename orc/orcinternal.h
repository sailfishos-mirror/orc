
#ifndef _ORC_INTERNAL_H_
#define _ORC_INTERNAL_H_

#include <orc/orcutils.h>
#include <orc/orclimits.h>

ORC_BEGIN_DECLS

#ifdef ORC_ENABLE_UNSTABLE_API

/* The function prototypes need to be visible to orc.c */
ORC_INTERNAL void orc_mmx_init (void);
ORC_INTERNAL void orc_sse_init (void);
ORC_INTERNAL void orc_avx_init (void);
ORC_INTERNAL void orc_arm_init (void);
ORC_INTERNAL void orc_powerpc_init (void);
ORC_INTERNAL void orc_c_init (void);
ORC_INTERNAL void orc_neon_init (void);
ORC_INTERNAL void orc_c64x_init (void);
ORC_INTERNAL void orc_c64x_c_init (void);
ORC_INTERNAL void orc_mips_init (void);

typedef struct _OrcCodeRegion OrcCodeRegion;
typedef struct _OrcCodeChunk OrcCodeChunk;

/* This is internal API, nothing in the public headers returns an OrcCodeChunk
 */
OrcCodeRegion * orc_code_region_alloc (void);
void orc_code_chunk_free (OrcCodeChunk *chunk);

ORC_INTERNAL orc_bool orc_compiler_is_debug ();

ORC_INTERNAL extern int _orc_data_cache_size_level1;
ORC_INTERNAL extern int _orc_data_cache_size_level2;
ORC_INTERNAL extern int _orc_data_cache_size_level3;
ORC_INTERNAL extern int _orc_cpu_family;
ORC_INTERNAL extern int _orc_cpu_model;
ORC_INTERNAL extern int _orc_cpu_stepping;
ORC_INTERNAL extern const char *_orc_cpu_name;

ORC_INTERNAL void orc_compiler_emit_invariants (OrcCompiler *compiler);
ORC_INTERNAL int orc_compiler_has_float (OrcCompiler *compiler);

ORC_INTERNAL char* _orc_getenv (const char *var);
ORC_INTERNAL void orc_opcode_sys_init (void);

#endif

ORC_END_DECLS

#endif

