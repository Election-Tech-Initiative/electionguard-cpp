

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test_chaum_pedersen_proof(void);
bool test_elgamal(void);
bool test_hash(void);

int main(void)
{
    assert(test_chaum_pedersen_proof() == true);
    assert(test_elgamal() == true);
    assert(test_hash() == true);

    printf("\n C TEST STATUS SUCCESS! \n");
}
