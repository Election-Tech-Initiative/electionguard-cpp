#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test_ballot_code(void);
bool test_ballot(void);
bool test_chaum_pedersen_proof(void);
bool test_election(void);
bool test_elgamal(void);
bool test_encrypt_compact(void);
bool test_encrypt(void);
bool test_hash(void);

int main(void)
{
    assert(test_ballot_code() == true);
    assert(test_ballot() == true);
    assert(test_chaum_pedersen_proof() == true);
    assert(test_election() == true);
    assert(test_elgamal() == true);
    assert(test_encrypt_compact() == true);
    assert(test_encrypt() == true);
    assert(test_hash() == true);

    printf("\n C TEST STATUS SUCCESS! \n");
}
