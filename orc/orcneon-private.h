#ifndef _ORC_NEON_PRIVATE_H_
#define _ORC_NEON_PRIVATE_H_

#include <orc/orcutils.h>

ORC_INTERNAL void orc_compiler_neon_register_rules (OrcTarget *target);

ORC_INTERNAL void orc_neon32_compiler_init (OrcCompiler *compiler);
ORC_INTERNAL void orc_neon32_compile (OrcCompiler *compiler);
ORC_INTERNAL void orc_neon64_compiler_init (OrcCompiler *compiler);
ORC_INTERNAL void orc_neon64_compile (OrcCompiler *compiler);

enum {
  LABEL_ONE_REGION = 1,
  LABEL_ONE_REGION_AFTER,
  LABEL_REGION0_LOOP,
  LABEL_REGION0_SKIP,
  LABEL_REGION1_LOOP,
  LABEL_REGION1_SKIP,
  LABEL_REGION2_LOOP_SMALL,
  LABEL_REGION2_LOOP_MEDIUM,
  LABEL_REGION2_LOOP_LARGE,
  LABEL_REGION2_SMALL,
  LABEL_REGION2_MEDIUM,
  LABEL_REGION2_SKIP,
  LABEL_REGION3_LOOP,
  LABEL_REGION3_SKIP,
  LABEL_OUTER_LOOP,
  LABEL_OUTER_LOOP_SKIP,
  LABEL_L1L2_AFTER,
};

enum RegionFlags {
  FLAG_NUNROLL = (1 << 0),
  FLAG_REGION1 = (1 << 1),
  FLAG_REGION2 = (1 << 2),
  FLAG_REGION3 = (1 << 3),
};

#define ORC_NEON_ALIGNED_DEST_CUTOFF 64

ORC_INTERNAL void orc_neon_emit_loop_common (OrcCompiler *compiler, int unroll_index);
ORC_INTERNAL void orc_neon_compiler_init_common (OrcCompiler *compiler);
ORC_INTERNAL enum RegionFlags orc_compiler_neon_calc_regions (const OrcCompiler *compiler);
ORC_INTERNAL unsigned int orc_neon_get_callee_saved_regs (const OrcCompiler *compiler, int base, int n);
ORC_INTERNAL int orc_neon_get_shift (int size);
ORC_INTERNAL int orc_neon_get_align_var (OrcCompiler *compiler);

#endif
