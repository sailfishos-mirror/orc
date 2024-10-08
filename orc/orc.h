#ifndef _ORC_ORC_H_
#define _ORC_ORC_H_

#include <orc/orcutils.h>
#include <orc/orcprogram.h>
#include <orc/orcdebug.h>
#include <orc/orcfunctions.h>
#include <orc/orconce.h>
#include <orc/orcparse.h>
#include <orc/orccpu.h>

ORC_BEGIN_DECLS

ORC_API void orc_init (void);
ORC_API const char * orc_version_string (void);

ORC_END_DECLS

#endif
