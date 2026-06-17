# libteleproto3 (prebuilt)

Type3 proxy transport, consumed as a **prebuilt static library** from the
[ankuper/teleproto3 release](https://github.com/ankuper/teleproto3/releases)
— never built from source in this repo (single source of truth).

## Bazel target

```starlark
deps = ["//third-party/libteleproto3:teleproto3"]
```

Exposes the `t3_client_*` API (`t3.h`, `t3_client.h`). The build downloads the
device (`arm64`) and simulator (`arm64`+`x86_64`) slices via `http_file` in
`MODULE.bazel` and selects the correct one for the target platform.

## Bumping the version

1. Update the two `http_file` URLs + `sha256` in `MODULE.bazel`
   (`libteleproto3_ios_device`, `libteleproto3_ios_simulator`).
2. Get the checksums:
   ```bash
   curl -sL <release-url>/libteleproto3-ios-arm64.tar.gz | shasum -a 256
   curl -sL <release-url>/libteleproto3-ios-simulator.tar.gz | shasum -a 256
   ```
3. If the public header set changes, update `_HEADERS` in this directory's `BUILD`.

Current version: **v0.7.0**.
