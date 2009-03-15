
#include "config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <orc/orcprogram.h>
#include <orc/orcdebug.h>


OrcExecutor *
orc_executor_new (OrcProgram *program)
{
  OrcExecutor *ex;

  ex = malloc(sizeof(OrcExecutor));
  memset(ex,0,sizeof(OrcExecutor));

  ex->program = program;

  return ex;
}

void
orc_executor_free (OrcExecutor *ex)
{
  free (ex);
}

void
orc_executor_run (OrcExecutor *ex)
{
  void (*func) (OrcExecutor *);

  func = ex->program->code_exec;
  if (func) {
    func (ex);
  } else {
    orc_executor_emulate (ex);
  }
}

void
orc_executor_set_array (OrcExecutor *ex, int var, void *ptr)
{
  ex->arrays[var] = ptr;
}

void
orc_executor_set_array_str (OrcExecutor *ex, const char *name, void *ptr)
{
  int var;
  var = orc_program_find_var_by_name (ex->program, name);
  ex->arrays[var] = ptr;
}

void
orc_executor_set_parameter (OrcExecutor *ex, int var, int value)
{
  ex->params[var] = value;
}

void
orc_executor_set_param_str (OrcExecutor *ex, const char *name, int value)
{
  int var;
  var = orc_program_find_var_by_name (ex->program, name);
  ex->params[var] = value;
}

void
orc_executor_set_n (OrcExecutor *ex, int n)
{
  ex->n = n;
}

void
orc_executor_emulate (OrcExecutor *ex)
{
  int i;
  int j;
  int k;
  OrcProgram *program = ex->program;
  OrcInstruction *insn;
  OrcOpcode *opcode;
  OrcOpcodeExecutor opcode_ex;

  for(i=0;i<ex->n;i++){
    for(j=0;j<program->n_insns;j++){
      insn = program->insns + j;
      opcode = insn->opcode;

      /* set up args */
      for(k=0;k<opcode->n_src + opcode->n_dest;k++){

        if (program->vars[insn->args[k]].vartype == ORC_VAR_TYPE_SRC) {
          void *ptr = ex->arrays[insn->args[k]] +
            program->vars[insn->args[k]].size*i;

          switch (program->vars[insn->args[k]].size) {
            case 1:
              opcode_ex.values[k] = *(int8_t *)ptr;
              break;
            case 2:
              opcode_ex.values[k] = *(int16_t *)ptr;
              break;
            case 4:
              opcode_ex.values[k] = *(int32_t *)ptr;
              break;
            default:
              ORC_ERROR("ack");
          }
        }
      }

      opcode->emulate (&opcode_ex, opcode->emulate_user);

      for(k=0;k<opcode->n_src + opcode->n_dest;k++){
        if (program->vars[insn->args[k]].vartype == ORC_VAR_TYPE_DEST) {
          void *ptr = ex->arrays[insn->args[k]] +
            program->vars[insn->args[k]].size*i;

          switch (program->vars[insn->args[k]].size) {
            case 1:
              *(int8_t *)ptr = opcode_ex.values[k];
              break;
            case 2:
              *(int16_t *)ptr = opcode_ex.values[k];
              break;
            case 4:
              *(int32_t *)ptr = opcode_ex.values[k];
              break;
            default:
              ORC_ERROR("ack");
          }
        }
      }
    }
  }
}

