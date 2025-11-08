# Compiler flags for a dependency-free kernel (brief)

Flags to compile a bare‑metal kernel without depending on the host runtime (it relies on the host's startup/runtime libraries):

    gcc -m32 -fno-builtin -fno-stack-protector -ffreestanding -nostdlib -nodefaultlibs -c kernel.c -o kernel.o

- `-m32`  
  Generate 32‑bit x86 code (useful if your bootloader/kernel expect i386 ABI).

- `-fno-builtin`  
  Disable compiler built‑ins so calls like `memcpy`, `strlen` are not replaced by compiler intrinsics — you provide your own implementations.

- `-fno-stack-protector`  
  Disable stack‑canary instrumentation (`__stack_chk_fail`) to avoid runtime dependencies.

- `-ffreestanding`  
  Tell the compiler this is a freestanding (bare‑metal) environment; do not assume a hosted C library or usual startup code.

- `-nostdlib`  
  Do not link the standard C runtime startup files or libc. You must provide the entry point and any required functions.

- `-nodefaultlibs`  
  Do not link the default system libraries (libc, libm, libgcc, ...). Often used together with `-nostdlib` for full control.

- `-c` and `-o kernel.o`  
  `-c` compiles only (no link); `-o` sets the output object file name.

C++ specific:
- `-fno-exceptions` — disable exceptions (try/catch).  
- `-fno-rtti` — disable RTTI (`typeid`, `dynamic_cast`).  
(Only relevant when compiling C++ sources to avoid pulling C++ runtime support.)

Notes:
- With `-nostdlib`/`-nodefaultlibs` own implemention of any used library functions (e.g. `memcpy`, `memset`, `strlen`) are needed.
- Linker script (e.g. `linker.ld`) or `ld` directly to set the load address and entry point.