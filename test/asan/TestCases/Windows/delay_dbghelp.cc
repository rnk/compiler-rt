// Make a directory, copy the problematic copies of dbg*.dll into that
// directory, build an exe there, and force the loader to find dbghelp.dll there
// instead of searching in C:/Windows/system32 first. Touching t.exe.local does
// this, according to MSDN:
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms682600(v=vs.85).aspx
//
// RUN: rm -rf %t.dir && mkdir %t.dir && cd %t.dir
// RUN: find %/S/Inputs/delay_dbghelp -iname '*.dll' | xargs cp -t .
// RUN: touch t.exe.local
// RUN: %clang_cl_asan %s -Fet.exe
// RUN: %run %t.dir/t.exe

// REQUIRES: asan-64-bits

extern "C" int puts(const char *);

int main() {
  puts("main");
}
