#include "meridian.hpp"

int main() {
    Meridian_init();

    Meridian_builtin();
    Meridian_run_file("test/test.mr");

    Meridian_free();
}
