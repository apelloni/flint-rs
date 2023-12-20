/*
    Copyright (C) 2012 Fredrik Johansson

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "arb_poly.h"

void
arb_poly_fit_length(arb_poly_t poly, slong len)
{
    slong i;

    if (len > poly->alloc)
    {
        if (len < 2 * poly->alloc)
            len = 2 * poly->alloc;

        poly->coeffs = flint_realloc(poly->coeffs,
            len * sizeof(arb_struct));

        for (i = poly->alloc; i < len; i++)
            arb_init(poly->coeffs + i);

        poly->alloc = len;
    }
}
