
#ifndef _ORC_RULE_H_
#define _ORC_RULE_H_

#include <orc/orclimits.h>
#include <orc/orcopcode.h>

ORC_BEGIN_DECLS

typedef struct _OrcRule OrcRule;
typedef struct _OrcRuleSet OrcRuleSet;


typedef void (*OrcRuleEmitFunc)(OrcCompiler *p, void *user, OrcInstruction *insn);

/**
 * OrcRule:
 *
 * The OrcRule structure has no public members
 */
struct _OrcRule {
  /*< private >*/
  OrcRuleEmitFunc emit;
  void *emit_user;
};

/**
 * OrcRuleSet:
 *
 * The OrcRuleSet structure has no public members
 */
struct _OrcRuleSet {
  /*< private >*/
  int opcode_major;
  int required_target_flags;

  OrcRule *rules;
  int n_rules;
};

ORC_END_DECLS

/* FIXME we need to include orctarget.h here as it has the typedef. There is a
 * circular dependency between OrcTarget and OrcRule. Check orc_rule_set_new
 * and orc_target_add_rule_set comments
 */

#include <orc/orctarget.h>

ORC_BEGIN_DECLS

ORC_API OrcRuleSet * orc_rule_set_new (OrcOpcodeSet *opcode_set, OrcTarget *target,
    unsigned int required_flags);

ORC_API void orc_rule_register (OrcRuleSet *rule_set, const char *opcode_name,
    OrcRuleEmitFunc emit, void *emit_user);


ORC_END_DECLS

#endif

