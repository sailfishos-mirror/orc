#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <orc/orcx86.h>
#include <orc/orcx86-private.h>
#include <orc/avx512/orcavx512.h>
#include <orc/avx512/orcavx512-private.h>
#include <orc/orcinternal.h>

#define ORC_AVX512_REG_SIZE 64
#define ORC_AVX512_REG_AMOUNT 32
/* TODO Foundation (F) instruction set should be the minimum set to make
 * the target runnable
 */
#define ORC_AVX512_MIN_FLAGS (\
  ORC_TARGET_AVX512_F |       \
  ORC_TARGET_AVX512_VL |      \
  ORC_TARGET_AVX512_BW |      \
  ORC_TARGET_AVX512_VBMI      \
)

static unsigned int
orc_avx512_target_get_default_flags (void)
{
  unsigned int flags = 0;

#if defined(HAVE_AMD64)
  flags |= ORC_TARGET_AVX512_64BIT;
#endif
  if (orc_compiler_is_debug ()) {
    flags |= ORC_TARGET_AVX512_FRAME_POINTER;
  }

#if defined(HAVE_I386) || defined(HAVE_AMD64)
  flags |= orc_avx512_get_cpu_flags ();
#else
  flags |= ORC_AVX512_MIN_FLAGS;
#endif

  return flags;
}

static const char *
orc_avx512_target_get_flag_name (const int shift)
{
  return orc_avx512_get_flag_name (1 << shift);
}

static int
orc_avx512_target_is_executable (void)
{
#if defined(HAVE_I386) || defined(HAVE_AMD64)
  /* initializes cache information */
  const int flags = orc_avx512_get_cpu_flags ();

  if ((flags & ORC_AVX512_MIN_FLAGS) == ORC_AVX512_MIN_FLAGS) {
    return TRUE;
  }
#endif
  return FALSE;
}

static void
orc_avx512_target_validate_registers (int *regs, int is_64bit)
{
  int i;

  if (is_64bit) {
    for (i = 0; i < ORC_AVX512_REG_AMOUNT; i++) {
      regs[ORC_AVX512_ZMM0 + i] = 1;
    }
  } else {
    for (i = 0; i < 8; i++) {
      regs[ORC_AVX512_ZMM0 + i] = 1;
    }
  }
}

static void
orc_avx512_target_saveable_registers (int *regs, int is_64bit)
{
#if defined(_WIN32) || defined(__CYGWIN__)
  if (is_64bit) {
    int i;
    for (i = 6; i < ORC_AVX_REG_AMOUNT; i++) {
      regs[ORC_AVX512_ZMM0 + i] = 1;
    }
  }
#endif
}

static int
orc_avx512_target_is_64bit (int flags)
{
  if (flags & ORC_TARGET_AVX512_64BIT) {
    return TRUE;
  } else {
    return FALSE;
  }
}

static int
orc_avx512_target_use_frame_pointer (int flags)
{
  if (flags & ORC_TARGET_AVX512_FRAME_POINTER) {
    return TRUE;
  } else {
    return FALSE;
  }
}

static int
orc_avx512_target_use_long_jumps (int flags)
{
  if (!(flags & ORC_TARGET_AVX512_SHORT_JUMPS)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/* How many elements of max_var_size can fit in a register
 * It is in the form of (2^ret)*(max_var_size*8) = reg_size */
static int
orc_avx512_target_loop_shift (int max_var_size)
{
  switch (max_var_size) {
    case 1:
      return 6;
    case 2:
      return 5;
    case 4:
      return 4;
    case 8:
      return 3;
    default:
      ORC_ERROR ("unhandled max var size %d", max_var_size);
      break;
  }

  return -1;
}

static void
orc_avx512_target_init_accumulator (OrcCompiler *compiler, OrcVariable *var)
{
  ORC_ERROR ("Missing implementation");
#if 0
  orc_avx512_emit_pxor (compiler, var->alloc, var->alloc, var->alloc);
#endif
}

static void
orc_avx512_target_reduce_accumulator (OrcCompiler *compiler, int i, OrcVariable *var)
{
  ORC_ERROR ("Missing implementation");
}

void
orc_avx512_target_load_constant (OrcCompiler *compiler, int reg, int size,
    orc_uint64 value)
{
  ORC_ERROR ("Missing implementation");
}

static void
orc_avx512_target_load_constant_long (OrcCompiler *compiler, int reg, OrcConstant *constant)
{
  ORC_ERROR ("Missing implementation");
}

static void
orc_avx512_target_move_register_to_memoffset (OrcCompiler *compiler, int size, int reg1, int offset, int reg2, int aligned, int uncached)
{
  ORC_ERROR ("Missing implementation");
#if 0
  orc_x86_emit_mov_avx_memoffset (compiler, size, reg1, offset, reg2, aligned, uncached);
#endif
}

static void
orc_avx512_target_move_memoffset_to_register (OrcCompiler *compiler, int size, int offset, int reg1, int reg2, int is_aligned)
{
  ORC_ERROR ("Missing implementation");
#if 0
  orc_x86_emit_mov_memoffset_avx (compiler, size, offset, reg1, reg2, is_aligned);
#endif
}

static int
orc_avx512_target_get_shift (int size)
{
  switch (size) {
    case 1:
      return 0;
    case 2:
      return 1;
    case 4:
      return 2;
    case 8:
      return 3;
    default:
      ORC_ERROR ("bad size %d", size);
  }
  return -1;
}

static void
orc_avx512_target_set_mxcsr (OrcCompiler *c)
{
  orc_avx512_insn_emit_stmxcsr (c,
      ORC_STRUCT_OFFSET (OrcExecutor, params[ORC_VAR_A4]),
      c->exec_reg);

  orc_x86_emit_mov_memoffset_reg (c, 4,
      ORC_STRUCT_OFFSET (OrcExecutor, params[ORC_VAR_A4]),
      c->exec_reg, c->gp_tmpreg);

  orc_x86_emit_mov_reg_memoffset (c, 4, c->gp_tmpreg,
      ORC_STRUCT_OFFSET (OrcExecutor, params[ORC_VAR_C1]),
      c->exec_reg);

  orc_x86_emit_cpuinsn_imm_reg (c, ORC_X86_or_imm32_rm, 4, 0x8040,
      c->gp_tmpreg);

  orc_x86_emit_mov_reg_memoffset (c, 4, c->gp_tmpreg,
      ORC_STRUCT_OFFSET (OrcExecutor, params[ORC_VAR_A4]),
      c->exec_reg);

  orc_avx512_insn_emit_ldmxcsr (c,
      ORC_STRUCT_OFFSET (OrcExecutor, params[ORC_VAR_A4]),
      c->exec_reg);
}

static void
orc_avx512_target_restore_mxcsr (OrcCompiler *c)
{
  orc_avx512_insn_emit_ldmxcsr (c,
      ORC_STRUCT_OFFSET (OrcExecutor, params[ORC_VAR_A4]),
      c->exec_reg);
}

OrcTarget *
orc_avx512_target_init (void)
{
  /* clang-format off */
  static OrcX86Target target = {
    "avx512",
    orc_avx512_target_get_default_flags,
    orc_avx512_target_get_flag_name,
    orc_avx512_target_is_executable,
    orc_avx512_target_validate_registers,
    orc_avx512_target_saveable_registers,
    orc_avx512_target_is_64bit,
    orc_avx512_target_use_frame_pointer,
    orc_avx512_target_use_long_jumps,
    orc_avx512_target_loop_shift,
    orc_avx512_target_init_accumulator,
    orc_avx512_target_reduce_accumulator,
    orc_avx512_target_load_constant,
    orc_avx512_target_load_constant_long,
    orc_avx512_target_move_register_to_memoffset,
    orc_avx512_target_move_memoffset_to_register,
    orc_avx512_target_get_shift,
    orc_avx512_target_set_mxcsr,
    orc_avx512_target_restore_mxcsr,
    NULL,
    NULL,
    ORC_AVX512_REG_SIZE,
    ORC_AVX512_ZMM0,
    ORC_AVX512_REG_AMOUNT,
    16,
  };
  /* clang-format on */
  static OrcTarget t;
  orc_x86_register_extension (&t, &target);
  return &t;
}
