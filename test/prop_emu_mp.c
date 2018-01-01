#include "fma.h"
#include "theft.h"


struct repeat_once_env {
    uint8_t local_minimum_runs;
    bool fail;
};

enum theft_trial_res
prop_emu_matches_asm(struct theft *t, void *arg1, void *arg2, void *arg3) {
    double a = *((double *) arg1);
    double b = *((double *) arg2);
    double c = *((double *) arg3);

    double emu_out = fma_emu_mp(a, b, c);
    double asm_out = fma_asm(a, b, c);

    uint64_t asm_out_raw = to_raw(asm_out);
    uint64_t emu_out_raw = to_raw(emu_out);

    if (emu_out_raw == asm_out_raw) {
        return THEFT_TRIAL_PASS;
    }

    printf("failure at:\n");
    printf("    asm = %lx (%a):\n", asm_out_raw, asm_out);
    printf("    emu = %lx (%a):\n", emu_out_raw, emu_out);

    return THEFT_TRIAL_FAIL;
}

enum theft_alloc_res
random_f64_alloc_cb(struct theft *t, void *env, void **instance) {
    double *x = malloc(sizeof(double));
    if (x == NULL) return THEFT_ALLOC_ERROR;

    *x = from_raw(theft_random_choice(t, UINT64_MAX));
    *instance = x;

    return THEFT_ALLOC_OK;
}

void print_f64_cb(FILE *f, const void *instance, void *env) {
    double x = *((double *) instance);
    fprintf(f, "%a == %lx == %f", x, to_raw(x), x);
}

struct theft_type_info random_f64_info = {
    .alloc = random_f64_alloc_cb,
    .free = theft_generic_free_cb,
    .print = print_f64_cb,
};

bool test_emu_matches_asm(uint64_t trials) {
    struct repeat_once_env env = { .fail = false };

    theft_seed seed = theft_seed_of_time();

    struct theft_run_config config = {
        .name = __func__,
        .prop3 = prop_emu_matches_asm,
        .type_info = {
            &random_f64_info,
            &random_f64_info,
            &random_f64_info,
        },
        .seed = seed,
        .trials = trials,
    };

    enum theft_run_res res = theft_run(&config);
    return res == THEFT_RUN_PASS;
}


int main() {
    uint64_t trials = (uint64_t) 1e6;
    test_emu_matches_asm(trials);
}
