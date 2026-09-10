#include <stdio.h>   // used for printf, fgets.
#include <stdlib.h>  // used for atoi.
#include <ctype.h>   // used for isspace, isdigit.

#define MAX_POTEGA 10    // Maximum degree of the polynomial.
#define MAX_DLUGOSC 1000 // Maximum length of the data on a single line.

// A very useful function that skips spaces. Advances the pointer until there are no more spaces.
static const char *skip_spacje(const char *p)
{
    while (*p && isspace((unsigned char) *p))
    {
        p++;
    }
    return p;
}

// This function reads a polynomial, stores the coefficients in an array, and determines the type of operation.
int wczytaj_wielomian(const char *linia, int wsp[MAX_POTEGA + 1], char *dzialanie)
{
    // We zero out the entire coefficient array.
    for (int i = 0; i <= MAX_POTEGA; i++)
    {
        wsp[i] = 0;
    }

    const char *poz = linia;
    poz = skip_spacje(poz);

    // Recognizing the operation ('+' or '*').
    if (*poz == '+' || *poz == '*')
    {
        *dzialanie = *poz;
        poz++;
    }

    poz = skip_spacje(poz);

    // We find the monomials - sign, coefficient, and exponent.
    while (*poz && *poz != '.')
    {
        int znak = 1; // Sign of the monomial (plus or minus).
        int wspol = 0; // Coefficient value.
        int potega = 0; // Exponent value.
        int ma_wsp = 0; // Indicates whether the coefficient was given explicitly (if not, it is 1 or -1).

        poz = skip_spacje(poz);

        // Determining the sign of the monomial.
        if (*poz == '-')
        {
            znak = -1;
            poz++;
        }
        else if (*poz == '+')
        {
            poz++;
        }

        poz = skip_spacje(poz);

        // Reading the coefficient, if it is a number.
        if (isdigit((unsigned char) *poz))
        {
            wspol = atoi(poz);
            // Moving the pointer to the end of the number.
            while (isdigit((unsigned char) *poz))
            {
                poz++;
            }
            ma_wsp = 1;
        }

        poz = skip_spacje(poz);

        // Reading the variable and its exponent.
        if (*poz == 'x')
        {
            poz++;
            potega = 1;  // If x is present - the exponent defaults to one.
            poz = skip_spacje(poz);

            if (*poz == '^')
            {
                poz++;
                poz = skip_spacje(poz);
                potega = atoi(poz);  // atoi - function that reads a number.
                while (isdigit((unsigned char) *poz))
                {
                    poz++;
                }
            }

            if (!ma_wsp)
            {
                wspol = 1;
            }
        }

        // We add the monomial to the coefficient array.
        wsp[potega] += znak * wspol;

        poz = skip_spacje(poz);
    }

    return 0;
}

// This function adds two polynomials, storing the result in a third array.
int dodaj_wielomiany(int w1[MAX_POTEGA + 1], int w2[MAX_POTEGA + 1], int wynik[MAX_POTEGA + 1])
{
    for (int i = 0; i <= MAX_POTEGA; i++)
    {
        wynik[i] = w1[i] + w2[i];
    }
    return 0;
}

// This function multiplies two polynomials the "school" way (long multiplication).
int mnoz_wielomiany(int w1[MAX_POTEGA + 1], int w2[MAX_POTEGA + 1], int wynik[MAX_POTEGA + 1])
{
    // We zero out the result array, because at each step we add further values.
    for (int i = 0; i <= MAX_POTEGA; i++)
    {
        wynik[i] = 0;
    }

    for (int i = 0; i <= MAX_POTEGA; i++)
    {
        for (int j = 0; j <= MAX_POTEGA; j++)
        {
            if (i + j <= MAX_POTEGA)
            {
                wynik[i + j] += w1[i] * w2[j];
            }
        }
    }

    return 0;
}

// This function prints the polynomial in correct mathematical form.
void wypisz_wielomian(int p[MAX_POTEGA + 1])
{
    int pierwsze = 1; // Indicates whether we are printing the first monomial.
    int puste = 1; // Indicates whether the polynomial is zero.

    for (int j = MAX_POTEGA; j >= 0; j--)
    {
        // We go from the highest exponent.
        if (p[j] != 0)
        {
            // If the coefficient is nonzero, we print the monomial.
            puste = 0;

            // Handling signs between monomials.
            if (!pierwsze)
            {
                if (p[j] < 0)  // If the monomial is not the first one - we print ' - ' or ' + ' with spaces.
                {
                    printf(" - ");
                }
                else
                {
                    printf(" + ");
                }
            }
            else if (p[j] < 0)
            {
                printf("-");  // If the monomial is the first one - we print '-' without spaces.
            }

            pierwsze = 0;

            int wartosc = p[j];
            if (wartosc < 0)
            {
                wartosc *= -1;  // Absolute value of the coefficient.
            }

            // Printing monomials depending on the exponent.
            if (j == 0)
            {
                printf("%d", wartosc); // Exponent zero.
            }
            else if (j == 1)
            {
                if (wartosc != 1)
                {
                    printf("%d", wartosc);
                }
                printf("x"); // Exponent one.
            }
            else
            {
                if (wartosc != 1)
                {
                    printf("%d", wartosc);
                }
                printf("x^%d", j); // Exponent greater than 1.
            }
        }
    }

    if (puste)
    {
        // We print zero if all coefficients were zero.
        printf("0");
    }
}

int main(void)
{
    char linia[MAX_DLUGOSC]; // Buffer for the line being read.
    int akumulator[MAX_POTEGA + 1] = {0}; // Zeroed array storing the result after each operation.
    int wynik[MAX_POTEGA + 1] = {0}; // Zeroed auxiliary array for the calculations.
    int wsp[MAX_POTEGA + 1]; // Array of coefficients of the polynomial being read.
    char dzialanie; // Type of operation: + or *.

    // We read input lines until a dot appears.
    while (fgets(linia, sizeof(linia), stdin) && linia[0] != '.')
    {
        // We skip empty lines.
        const char *p = skip_spacje(linia);
        if (*p == '\0' || *p == '\n')
        {
            continue;
        }

        wczytaj_wielomian(linia, wsp, &dzialanie); // We read the polynomial and the type of operation.

        // Depending on the sign, we perform the operation on the polynomials.
        if (dzialanie == '+')
        {
            dodaj_wielomiany(akumulator, wsp, wynik);
        }
        else if (dzialanie == '*')
        {
            mnoz_wielomiany(akumulator, wsp, wynik);
        }

        wypisz_wielomian(wynik); // We print the result of the operation.
        printf("\n");  // We add a newline character.

        // We copy the result into the accumulator.
        for (int i = 0; i <= MAX_POTEGA; i++)
        {
            akumulator[i] = wynik[i];
        }
    }

    return 0;
}
