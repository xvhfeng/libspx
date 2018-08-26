#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "spx_allocator.h"

START_TEST(spx_allocator_default_alloc_entry_nonpooling_test) {
    spx_ret_t ret = SpxRet_OK;
    sint_t size = 15;
    void_t obj = spx_allocator_default_alloc_entry(NULL, size, &ret);
    ck_assert_int_eq(ret, SpxRet_OK);
    spx_entry_t *entry = spx_container_of(obj, spx_entry_t, buf);
    ck_assert_int_eq(entry->len, sizeof(spx_entry_t) + spx_sys_aligned(size));
    ck_assert_int_eq(entry_flag, SpxEntryNormal);
}
END_TEST

Suite *make_spx_allocator_test_suite(void) {
    Suite *s = suite_create("spx_allocator");
    TCase *tc = tcase_create("spx_allocator");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, spx_allocator_test_allocator);
    return s;
}
