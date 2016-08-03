// Test the handle_sigill option.
//
// RUN: %clangxx_asan %s -o %t && %env_asan_opts=handle_sigill=0 not --crash %run %t 2>&1 | FileCheck %s --check-prefix=CHECK0
// RUN: %clangxx_asan %s -o %t && %env_asan_opts=handle_sigill=1 not %run %t 2>&1 | FileCheck %s --check-prefix=CHECK1

int main() {
  __builtin_trap();
}
// CHECK0-NOT: ERROR: AddressSanitizer
// CHECK1: ERROR: AddressSanitizer: {{ILL|illegal-instruction}} on unknown address {{0x0*}}
