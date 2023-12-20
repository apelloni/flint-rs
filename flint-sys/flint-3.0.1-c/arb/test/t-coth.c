/*
    Copyright (C) 2013 Fredrik Johansson

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "arb.h"

int main(void)
{
    slong iter;
    flint_rand_t state;

    flint_printf("coth....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 10000 * 0.1 * flint_test_multiplier(); iter++)
    {
        arb_t x, y, a, b, c, d;
        slong prec1, prec2;

        prec1 = 2 + n_randint(state, 1000);
        prec2 = prec1 + 30;

        arb_init(x);
        arb_init(y);
        arb_init(a);
        arb_init(b);
        arb_init(c);
        arb_init(d);

        arb_randtest_precise(x, state, 1 + n_randint(state, 1000), 100);
        arb_randtest_precise(y, state, 1 + n_randint(state, 1000), 100);

        arb_coth(a, x, prec1);
        arb_coth(b, x, prec2);

        /* check consistency */
        if (!arb_overlaps(a, b))
        {
            flint_printf("FAIL: overlap\n\n");
            flint_printf("x = "); arb_print(x); flint_printf("\n\n");
            flint_printf("a = "); arb_print(a); flint_printf("\n\n");
            flint_printf("b = "); arb_print(b); flint_printf("\n\n");
            flint_abort();
        }

        /* check coth(x+y) = (1 + coth(x) coth(y)) / (coth(x) + coth(y)) */
        arb_add(b, x, y, prec1);
        arb_coth(b, b, prec1);

        arb_coth(c, y, prec1);
        arb_add(d, a, c, prec1);
        arb_mul(c, a, c, prec1);
        arb_add_ui(c, c, 1, prec1);
        arb_div(d, c, d, prec1);

        if (!arb_overlaps(b, d))
        {
            flint_printf("FAIL: functional equation\n\n");
            flint_printf("x = "); arb_print(x); flint_printf("\n\n");
            flint_printf("y = "); arb_print(y); flint_printf("\n\n");
            flint_printf("b = "); arb_print(b); flint_printf("\n\n");
            flint_printf("d = "); arb_print(d); flint_printf("\n\n");
            flint_abort();
        }

        arb_coth(x, x, prec1);

        if (!arb_overlaps(a, x))
        {
            flint_printf("FAIL: aliasing\n\n");
            flint_printf("a = "); arb_print(a); flint_printf("\n\n");
            flint_printf("x = "); arb_print(x); flint_printf("\n\n");
            flint_abort();
        }

        arb_clear(x);
        arb_clear(y);
        arb_clear(a);
        arb_clear(b);
        arb_clear(c);
        arb_clear(d);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return 0;
}

