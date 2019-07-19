#ifndef LIBPT_COMMON_H
#define LIBPT_COMMON_H

#include <stdio.h> // FILE *
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
  
//---------------------------------------------------------------------------
// Callback types.
//---------------------------------------------------------------------------

/**
 * \brief Type related to a *_free() functions.
 */

#define ELEMENT_FREE void   (*)(void *)

/**
 * \brief Type related to a *_dup() functions.
 */

#define ELEMENT_DUP  void * (*)(const void *)

/**
 * \brief Type related to a *_dump() functions.
 * \warning Your object should rather on *_fprintf functions.
 */

#define ELEMENT_DUMP void   (*)(const void *)

/**
 * \brief Type related to a *_dump() functions.
 */

#define ELEMENT_FPRINTF void   (*)(FILE *, const void *)


/**
 * \brief Type related to a *_compare() function
 */

#define ELEMENT_COMPARE int (*)(const void *, const void *)

//---------------------------------------------------------------------------
// Misc
//---------------------------------------------------------------------------

/**
 * \brief Macro returning the minimal value of two elements
 * \param x The left operand
 * \param x The right operand
 * \return The min of x and y
 */

#define MIN(x, y) ((x) < (y) ? (x) : (y))

/**
 * \brief Macro returning the maximal value of two elements
 * \param x The left operand
 * \param x The right operand
 * \return The max of x and y
 */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/**
 * \return The current timestamp (in seconds)
 */

double get_timestamp();

/**
 * \brief Print some space characters
 * \param indent The number of space characters to print
 *   in the standard output
 */

void print_indent(unsigned int indent);

/*
 * For a compatibility purpose only, since the lib shouldn't print anything but store data to be printed
 * by its client.
 * \param iDoPrint if true, traceroute_handler() acts as default so far, and prints everything(ttl, ip, resolved host name)
 */
void setPrintMode(bool iDoPrint);

#ifdef __cplusplus
}
#endif

#endif // LIBPT_COMMON_H
