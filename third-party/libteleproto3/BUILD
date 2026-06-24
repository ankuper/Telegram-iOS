# libteleproto3 — Type3 proxy transport (prebuilt).
#
# Consumed ONLY as a prebuilt static library from the ankuper/teleproto3 GitHub
# release — never built from source here (project rule: one source of truth).
# The release ships two iOS slices with identical public headers:
#   * device     — arm64            (libteleproto3-ios-arm64.tar.gz)
#   * simulator  — arm64 + x86_64   (libteleproto3-ios-simulator.tar.gz)
# Both tarballs have the layout: ./lib/libteleproto3.a + ./lib/include/*.h
#
# Public API: t3_client_create / t3_client_get_fd / t3_client_pump /
# t3_client_read / t3_client_write / t3_client_destroy (see t3_client.h).

package(default_visibility = ["//visibility:public"])

_HEADERS = [
    "t3.h",
    "t3_client.h",
    "t3_features.h",
    "t3_shim_socks5.h",
]

# Extract the device tarball: static lib + the shared public headers.
genrule(
    name = "extract_device",
    srcs = ["@libteleproto3_ios_device//file"],
    outs = ["device/libteleproto3.a"] + ["include/" + h for h in _HEADERS],
    cmd = """
set -e
WORK=$(RULEDIR)/_work_device
rm -rf "$$WORK" && mkdir -p "$$WORK"
tar -xzf "$(location @libteleproto3_ios_device//file)" -C "$$WORK"
cp "$$WORK/lib/libteleproto3.a" "$(location device/libteleproto3.a)"
""" + "".join([
        'cp "$$WORK/lib/include/{h}" "$(location include/{h})"\n'.format(h = h)
        for h in _HEADERS
    ]),
)

# Extract the simulator tarball: static lib only (headers shared from device).
genrule(
    name = "extract_simulator",
    srcs = ["@libteleproto3_ios_simulator//file"],
    outs = ["simulator/libteleproto3.a"],
    cmd = """
set -e
WORK=$(RULEDIR)/_work_simulator
rm -rf "$$WORK" && mkdir -p "$$WORK"
tar -xzf "$(location @libteleproto3_ios_simulator//file)" -C "$$WORK"
cp "$$WORK/lib/libteleproto3.a" "$(location simulator/libteleproto3.a)"
""",
)

cc_library(
    name = "teleproto3_device_archive",
    srcs = ["device/libteleproto3.a"],
    # Force-load the whole archive. The prebuilt .a's t3_client_* objects
    # reference internal t3c_*/ssl_* symbols defined in sibling objects of the
    # same archive; without alwayslink the linker does not pull those siblings
    # in and they remain "Undefined symbols for architecture arm64".
    alwayslink = True,
)

cc_library(
    name = "teleproto3_simulator_archive",
    srcs = ["simulator/libteleproto3.a"],
    alwayslink = True,
)

# Consumer-facing target. Selects the correct slice for the build platform;
# headers are identical across slices so they are exposed unconditionally.
# Type3 uses SecRandomCopyBytes (Security.framework) for its CSPRNG.
objc_library(
    name = "teleproto3",
    module_name = "teleproto3",
    enable_modules = True,
    hdrs = ["include/" + h for h in _HEADERS],
    includes = ["include"],
    sdk_frameworks = ["Security"],
    deps = select({
        "@build_bazel_apple_support//constraints:simulator": [":teleproto3_simulator_archive"],
        "//conditions:default": [":teleproto3_device_archive"],
    }),
    visibility = ["//visibility:public"],
)
