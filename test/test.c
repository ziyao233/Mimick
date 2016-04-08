#include <mimick.h>
#include <stdlib.h>

#include "libfoo.h"
#include "assert.h"

mmk_mock_define (fn_ii_mock, int, int);
mmk_mock_define (fn_vv_mock, void);

mmk_mock_define (fn_ii_va_mock, int, int, mmk_va_args);

static int valid;

void set_valid(void) {
    valid = 1;
}

int main(void)
{
    mmk_mock("fn_vv", fn_vv_mock);
    mmk_when(fn_vv(), .then_call = (mmk_fn) set_valid);

    mmk_assert(mmk_verify(fn_vv(), .never = 1));
    mmk_assert(mmk_verify(fn_vv(), .at_most = 1));

    mmk_assert(!mmk_verify(fn_vv(), .times = 1));
    mmk_assert(!mmk_verify(fn_vv(), .at_least = 1));
    mmk_assert(!mmk_verify(fn_vv(), .at_least = 1, .at_most = 1));

    fn_vv();
    mmk_assert(valid);
    mmk_assert(mmk_verify(fn_vv(), .times = 1));
    mmk_assert(mmk_verify(fn_vv(), .at_least = 1));
    mmk_assert(mmk_verify(fn_vv(), .at_most = 1));
    mmk_assert(mmk_verify(fn_vv(), .at_least = 1, .at_most = 1));
    mmk_assert(!mmk_verify(fn_vv(), .never = 1));
    mmk_assert(!mmk_verify(fn_vv(), .times = 2));
    mmk_assert(!mmk_verify(fn_vv(), .at_least = 2));
    mmk_reset(fn_vv);

    int err = 0xfefefefe;
    mmk_mock("fn_ii", fn_ii_mock);
    mmk_when(fn_ii(1), .then_return = mmk_val(int, 1), .then_errno = err);

    mmk_assert(fn_ii(1)  == 1 && errno == err);
    mmk_assert(fn_ii(42) == 0);

    mmk_assert(mmk_verify(fn_ii(mmk_any(int)), .times = 2));
    mmk_assert(mmk_verify(fn_ii(1), .times = 1));
    mmk_assert(mmk_verify(fn_ii(42), .times = 1));
    mmk_assert(mmk_verify(fn_ii(12), .never = 1));
    mmk_reset(fn_ii);

    return 0;
}
