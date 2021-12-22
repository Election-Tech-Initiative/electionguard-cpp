#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test_ballot_code(void);
bool test_ballot(void);
bool test_chaum_pedersen_proof(void);
bool test_collections(void);
bool test_election(void);
bool test_elgamal(void);
bool test_encrypt_compact(void);
bool test_encrypt(void);
bool test_hash(void);
bool test_manifest(void);

int main(void)
{
    printf("\n ---------- RUNNING C TESTS ------------ \n");

    bool ballot_code = test_ballot_code();
    bool ballot = test_ballot();
    bool proofs = test_chaum_pedersen_proof();
    bool collections = test_collections();
    bool election = test_election();
    bool elgamal = test_elgamal();
    bool encrypt_compact = test_encrypt_compact();
    bool encrypt = test_encrypt();
    bool hash = test_hash();
    bool manifest = test_manifest();

    bool success = ballot_code && ballot && proofs && collections && election && elgamal &&
                   encrypt_compact && encrypt && hash && manifest;

    if (success == true) {
        printf("\n ---------- C TEST STATUS SUCCESS! ---------- \n");
        return 0;
    } else {
        printf("\n ---------- C TEST FAILED! ---------- \n");
        return 1;
    }
}
