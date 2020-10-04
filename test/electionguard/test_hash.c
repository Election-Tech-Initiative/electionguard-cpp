#include <assert.h>
#include <electionguard/hash.h>

static bool test_hash_elems(void);

bool test_hash(void) { return test_hash_elems(); }

bool test_hash_elems(void)
{
    eg_element_mod_q_t *string_hash = NULL;
    if (eg_hash_elems_string("test", &string_hash)) {
        assert(false);
    }
    uint64_t *string_hash_data = NULL;
    size_t string_hash_data_size;
    if (eg_element_mod_q_get_data(string_hash, &string_hash_data, &string_hash_data_size)) {
        assert(false);
    }

    printf("string hash %llu", string_hash_data[0]);

    // TODO: check the actual hash string
    assert(string_hash != NULL);
    assert(string_hash != 0);
    assert(string_hash_data_size == 4);
    assert(string_hash_data[0] > 0);

    const char *strings[] = {"test", "strings"};
    eg_element_mod_q_t *strings_hash = NULL;
    if (eg_hash_elems_strings(strings, 2, &strings_hash)) {
        assert(false);
    }
    uint64_t *strings_hash_data = NULL;
    size_t strings_hash_data_size;
    if (eg_element_mod_q_get_data(string_hash, &strings_hash_data, &strings_hash_data_size)) {
        assert(false);
    }

    // TODO: check the actual hash string
    assert(strings_hash != NULL);
    assert(strings_hash != 0);
    assert(strings_hash_data_size == 4);
    assert(strings_hash_data[0] > 0);

    eg_element_mod_q_t *int_hash = NULL;
    if (eg_hash_elems_int(1234, &int_hash)) {
        assert(false);
    }
    uint64_t *int_hash_data = NULL;
    size_t int_hash_data_size;
    if (eg_element_mod_q_get_data(string_hash, &int_hash_data, &int_hash_data_size)) {
        assert(false);
    }

    // TODO: check the actual hash string
    assert(int_hash != NULL);
    assert(int_hash != 0);
    assert(int_hash_data_size == 4);
    assert(int_hash_data[0] > 0);

    if (eg_element_mod_q_free(string_hash)) {
        assert(false);
    }
    if (eg_element_mod_q_free(strings_hash)) {
        assert(false);
    }
    if (eg_element_mod_q_free(int_hash)) {
        assert(false);
    }

    return true;
}
