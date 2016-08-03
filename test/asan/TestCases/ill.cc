// RUN: %clangxx_asan -O0 %s -o %t && not %run %t 2>&1 | FileCheck %s
// RUN: %clangxx_asan -O2 %s -o %t && not %run %t 2>&1 | FileCheck %s
// Test the handle_sigill option.

int main() {
  __builtin_trap();
}
// CHECK: ERROR: AddressSanitizer: {{ILL|illegal-instruction}} on unknown address {{0x0*}}
