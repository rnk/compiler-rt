//===-- sanitizer_dbghelp.h ------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Wrappers for lazy loaded dbghelp.dll.
//
//===----------------------------------------------------------------------===//

#ifndef SANITIZER_SYMBOLIZER_WIN_H
#define SANITIZER_SYMBOLIZER_WIN_H

#if !SANITIZER_WINDOWS
#error "sanitizer_symbolizer_win.h is a Windows-only header"
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dbghelp.h>

namespace __sanitizer {

#if SANITIZER_WINDOWS
StackWalk64
SymCleanup
SymFromAddr
SymFunctionTableAccess64
SymGetLineFromAddr64
SymGetModuleBase64
SymGetSearchPathW
SymInitialize
SymSetOptions
SymSetSearchPathW
UnDecorateSymbolName
#endif
}  // namespace __sanitizer

#endif  // SANITIZER_SYMBOLIZER_WIN_H
