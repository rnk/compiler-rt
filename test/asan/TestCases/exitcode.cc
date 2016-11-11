// RUN: %clangxx_asan -Wno-deprecated-declarations %s -o %t
// RUN: %env_asan_opts=exitcode=123 %t | FileCheck %s

// CHECK: got expected 123 exit code

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

int spawn_child(const char **argv) {
  // Set an environment variable to tell the child process to interrupt
  // itself.
  if (!SetEnvironmentVariableW(L"CRASH_FOR_TEST", L"1")) {
    printf("SetEnvironmentVariableW failed (0x%8lx).\n", GetLastError());
    fflush(stdout);
    exit(1);
  }

  STARTUPINFOW si;
  memset(&si, 0, sizeof(si));
  si.cb = sizeof(si);

  PROCESS_INFORMATION pi;
  memset(&pi, 0, sizeof(pi));

  if (!CreateProcessW(nullptr,           // No module name (use command line)
                      GetCommandLineW(), // Command line
                      nullptr,           // Process handle not inheritable
                      nullptr,           // Thread handle not inheritable
                      TRUE,              // Set handle inheritance to TRUE
                      0,                 // No flags
                      nullptr,           // Use parent's environment block
                      nullptr,           // Use parent's starting directory
                      &si, &pi)) {
    printf("CreateProcess failed (0x%08lx).\n", GetLastError());
    fflush(stdout);
    exit(1);
  }

  WaitForSingleObject(pi.hProcess, INFINITE);

  DWORD exit_code;
  if (!GetExitCodeProcess(pi.hProcess, &exit_code)) {
    printf("GetExitCodeProcess failed (0x%08lx).\n", GetLastError());
    fflush(stdout);
    exit(1);
  }

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  return exit_code;
}
#else
#include <spawn.h>

#if defined(__APPLE__) && !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE)
#define USE_NSGETENVIRON 1
#else
#define USE_NSGETENVIRON 0
#endif

#if !USE_NSGETENVIRON
extern char **environ;
#else
#include <crt_externs.h> // _NSGetEnviron
#endif

int spawn_child(const char **argv) {
  setenv("CRASH_FOR_TEST", "1");

#if !USE_NSGETENVIRON
  const char **envp = const_cast<const char **>(environ);
#else
  const char **envp = const_cast<const char **>(*_NSGetEnviron());
#endif

  pid_t pid;
  int err = posix_spawn(&pid, argv[0], nullptr, nullptr, argv, envp);
  if (err) {
    printf("posix_spawn failed: %d\n", err);
    exit(1);
  }

  // Wait until the child exits.
  int status;
  pid_t wait_result_pid;
  do {
    wait_result_pid = waitpid(pid, &status, 0);
  } while (WaitResult.Pid == -1 && errno == EINTR);

  if (wait_result_pid != pid) {
    printf("error in waitpid\n");
    exit(1);
  }

  // Return the exit status.
  return status;
}
#endif

int main(int argc, char **argv) {
  int r = 0;
  if (getenv("CRASH_FOR_TEST")) {
    // Generate an asan report to test ASAN_OPTIONS=exitcode=123
    int *p = new int;
    delete p;
    r = *p;
  } else {
    int exit_code = spawn_child(const_cast<const char **>(argv));
    if (exit_code == 123) {
      printf("got expected 123 exit code\n");
      fflush(stdout);
    }
  }
  return r;
}
