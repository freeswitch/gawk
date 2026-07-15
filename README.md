# gawk.exe builds for Windows

GNU awk 3.1.8 is only distributed as source from
[ftp.gnu.org/gnu/gawk](https://ftp.gnu.org/gnu/gawk/), and the old GnuWin32
binary builds are unmaintained. This repository builds a native Windows
`gawk.exe` from the unmodified GNU tarball with GitHub Actions and publishes
it as a release here.

The build uses the MinGW-w64 toolchain (via MSYS2) and the DOS/Windows port
that ships inside the tarball's `pc/` directory (`make mingw32`), the same
approach the historical GnuWin32 packages used. The result is a standalone
native win32 executable that links against Microsoft's C runtime
(`msvcrt.dll`, `msvcp60.dll`) — no Cygwin or MSYS emulation layer.

The one modern patch needed is [`shim/langinfo.h`](shim/langinfo.h): MinGW
has no `<langinfo.h>`, which the GnuWin32 builds got from LibGw32C. The shim
provides the single `nl_langinfo(CODESET)` call gawk makes.

## Cutting a release

1. Put the gawk version into [`GAWK_VERSION`](GAWK_VERSION), e.g. `3.1.8`.
2. Commit, tag the commit, and push both:

   ```sh
   git commit -am "gawk 3.1.8"
   git tag v3.1.8
   git push origin HEAD v3.1.8
   ```

The [`build`](.github/workflows/build.yml) workflow then downloads
`gawk-<version>.tar.gz` from ftp.gnu.org, builds it with `make mingw32`,
smoke-tests the result (runs it and verifies it depends only on Windows
system DLLs), and creates a release for the pushed tag with these assets:

- `gawk.exe` — GNU awk (x86-64, stripped)
- `pgawk.exe` — profiling GNU awk
- `COPYING` — license (GPL), from the source tarball
- `sha256sums.txt`

The workflow fails if the pushed tag does not match the contents of
`GAWK_VERSION`, so a stale file can't produce a mislabeled release.

## Building without a tag

Run the workflow manually (Actions → build → Run workflow) and enter the
version, or:

```sh
gh workflow run build -f gawk_version=3.1.8
```

Note: the `pc/`-port build recipe is verified for gawk 3.1.8. Other 3.1.x
versions will likely build the same way; 4.x+ changed its Windows build and
may need workflow changes.
