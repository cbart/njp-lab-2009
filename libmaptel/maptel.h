#ifndef _MAPTEL_H_
#define _MAPTEL_H_

#ifdef __cplusplus
#include <cstddef>
#include <iostream>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** Creates new maptel.
 * Return value:
 *   identificator of created maptel. */
unsigned long maptel_create();

/** Removes maptel of given `id`.
 * In debuglevel > 0: maptel of given `id` must exist.
 * In debuglevel = 0: if maptel of given `id` does not exist ->
 * function does nothing.
 * Return value:
 *   none (void). */
void maptel_delete(unsigned long id);

/** Inserts given transformation (`tel_src` -> `tel_dst`)
 * into maptel of given `id`.
 * In debuglevel > 0: maptel of given `id` must exist.
 * Return value:
 *   none (void). */
void maptel_insert
(unsigned long id, const char *tel_src, const char *tel_dst);

/** Erases single transformation of given `tel_src`
 * in maptel of given `id`.
 * In debuglevel > 0: maptel of given `id` must exist.
 * Return value:
 *   none (void). */
void maptel_erase(unsigned long id, const char *tel_src);

/** Gives single transformation of given `tel_src`
 * in maptel of given `id`, and copies found number
 * to `tel_dst` using maximum of `len` bytes.
 * In debuglevel > 0: maptel of given `id` must exist.
 * `len` must be counted with string's terminal '\0'.
 * Return value:
 *   none (void). */
void maptel_transform
(unsigned long id, const char *tel_src, char *tel_dst, size_t len);

/** Indicates if sequence of transformations,
 * which begins in `tel_src` leads to a cycle.
 * In debuglevel > 0: maptel of given `id` must exist.
 * Return value:
 *   `1` (`true`) if sequence of transformations
 *       which begins in `tel_src` (in maptel of given `id`)
 *       leads to a cycle of transformations.
 *   `0` (`false`) if sequence of transformations
 *       which begins in `tel_src` (in maptel of given `id`)
 *       is a straight sequence (has no cycles).
 *  `-1` (`error`) if any of pointers is NULL, maptel does not exist
 *       or len is too short to write returned string. */
int maptel_is_cyclic(unsigned long id, const char *tel_src);

/** Gives recursive transformation of given `tel_src`
 * in maptel of given `id`, and copies found number
 * to `tel_dst` using maximum of `len` bytes.
 * In debuglevel > 0: maptel of given `id` must exist.
 * `len` must be counted with string's terminal '\0'.
 * In debuglevel = 0, when cycle is found, one of values
 * in the cycle is given.
 * Return value:
 *   none (void). */
void maptel_transform_ex
(unsigned long id, const char *tel_src, char *tel_dst, size_t len);

#ifdef __cplusplus
}
#endif

#endif
