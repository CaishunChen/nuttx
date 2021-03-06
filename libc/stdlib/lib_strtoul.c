/****************************************************************************
 * /libc/stdlib/lib_strtoul.c
 *
 *   Copyright (C) 2007, 2009, 2011, 2016 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdlib.h>
#include <errno.h>

#include "libc.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: strtoul
 *
 * Description:
 *   The  strtol() function  converts  the initial part of the string in
 *   nptr to a long unsigned integer value according to the given base, which
 *   must be between 2 and 36 inclusive, or be the special value 0.
 *
 * Returns:
 *   - The converted value, if the base and number are valid
 *   - 0 if an error occurs, and seterrno to:
 *     * EINVAL if base < 2 or base > 36
 *     * ERANGE if the number cannot be represented using unsigned long
 *
 ****************************************************************************/

unsigned long strtoul(FAR const char *nptr, FAR char **endptr, int base)
{
  unsigned long accum = 0;
  unsigned long prev;
  int value;

  if (nptr)
    {
      /* Skip leading spaces */

      lib_skipspace(&nptr);

      /* Check for unspecified or incorrect base */

      base = lib_checkbase(base, &nptr);

      if (base < 0)
        {
          set_errno(EINVAL);
          return 0;
        }

      /* Accumulate each "digit" */

      while (lib_isbasedigit(*nptr, base, &value))
        {
          prev  = accum;
          accum = accum*base + value;
          nptr++;

          /* Check for overflow */

          if (accum < prev)
            {
              set_errno(ERANGE);
              accum = 0;
              break;
            }
        }

      /* Return the final pointer to the unused value */

      if (endptr)
        {
          *endptr = (char *)nptr;
        }
    }

  return accum;
}
