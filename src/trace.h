#ifndef BU_TRACE_H
#define BU_TRACE_H

#ifdef BU_TRACE
# define TRACE() printf("trace: %s\n", __PRETTY_FUNCTION__ )
#else
# define TRACE() {}
#endif

#endif
