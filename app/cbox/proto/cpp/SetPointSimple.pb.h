/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.8 at Mon Sep 18 12:21:14 2017. */

#ifndef PB_BLOX_SETPOINTSIMPLE_PB_H_INCLUDED
#define PB_BLOX_SETPOINTSIMPLE_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _blox_SetPointSimple {
    int16_t value;
/* @@protoc_insertion_point(struct:blox_SetPointSimple) */
} blox_SetPointSimple;

/* Default values for struct fields */

/* Initializer values for message structs */
#define blox_SetPointSimple_init_default         {0}
#define blox_SetPointSimple_init_zero            {0}

/* Field tags (for use in manual encoding/decoding) */
#define blox_SetPointSimple_value_tag            1

/* Struct field encoding specification for nanopb */
extern const pb_field_t blox_SetPointSimple_fields[2];

/* Maximum encoded size of messages (where known) */
#define blox_SetPointSimple_size                 6

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define SETPOINTSIMPLE_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif