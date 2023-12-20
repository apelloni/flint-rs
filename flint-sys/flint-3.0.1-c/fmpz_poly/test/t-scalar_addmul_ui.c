/*
    Copyright (C) 2009 William Hart
    Copyright (C) 2010 Sebastian Pancratz
    Copyright (C) 2021 Albin Ahlbäck

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "flint.h"
#include "fmpz.h"
#include "fmpz_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, result;
    FLINT_TEST_INIT(state);

    flint_printf("scalar_addmul_ui....");
    fflush(stdout);

    /* check dirty entries */
    {
        fmpz_poly_t a, b;

        fmpz_poly_init(a);
        fmpz_poly_init(b);

        fmpz_poly_one(a);
        fmpz_poly_zero(a);
        fmpz_poly_one(b);
        fmpz_poly_scalar_addmul_ui(a, b, 2);
        fmpz_poly_set_ui(b, 2);

        if (!fmpz_poly_equal(a, b))
        {
            flint_printf("FAIL: dirty\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_poly_clear(a);
        fmpz_poly_clear(b);
    }

    /* Check aliasing of a and b */
    for (i = 0; i < 1000 * flint_test_multiplier(); i++)
    {
        fmpz_poly_t a, b, c;
        ulong x;

        fmpz_poly_init(a);
        fmpz_poly_init(b);
        fmpz_poly_init(c);
        x = n_randtest_bits(state, FLINT_BITS);
        fmpz_poly_randtest(a, state, n_randint(state, 100), 200);
        fmpz_poly_set(b, a);
        fmpz_poly_set(c, a);

        fmpz_poly_scalar_addmul_ui(b, a, x);
        fmpz_poly_scalar_addmul_ui(a, a, x);

        result = (fmpz_poly_equal(a, b));
        if (!result)
        {
            flint_printf("FAIL (1):\n");
            flint_printf("a = "), fmpz_poly_print(a), flint_printf("\n\n");
            flint_printf("b = "), fmpz_poly_print(b), flint_printf("\n\n");
            flint_printf("c = "), fmpz_poly_print(c), flint_printf("\n\n");
            flint_printf("x = %u", x), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_poly_clear(a);
        fmpz_poly_clear(b);
        fmpz_poly_clear(c);
    }

    /* Check that b += x*a is the same as c = b + x*a */
    for (i = 0; i < 1000 * flint_test_multiplier(); i++)
    {
        fmpz_poly_t a, b, c;
        ulong x;

        fmpz_poly_init(a);
        fmpz_poly_init(b);
        fmpz_poly_init(c);
        x = n_randtest_bits(state, FLINT_BITS);
        fmpz_poly_randtest(a, state, n_randint(state, 100), 200);
        fmpz_poly_randtest(b, state, n_randint(state, 100), 200);

        fmpz_poly_scalar_mul_ui(c, a, x);
        fmpz_poly_add(c, b, c);

        fmpz_poly_scalar_addmul_ui(b, a, x);

        result = (fmpz_poly_equal(b, c));
        if (!result)
        {
            flint_printf("FAIL (2):\n");
            flint_printf("a = "), fmpz_poly_print(a), flint_printf("\n\n");
            flint_printf("b = "), fmpz_poly_print(b), flint_printf("\n\n");
            flint_printf("c = "), fmpz_poly_print(c), flint_printf("\n\n");
            flint_printf("x = %u", x), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_poly_clear(a);
        fmpz_poly_clear(b);
        fmpz_poly_clear(c);
    }

    FLINT_TEST_CLEANUP(state);

    flint_printf("PASS\n");
    return 0;
}
