This is a folder meant for integration unit testing for the external libraries.

This makes sure that the integration of the librarbies was successful, and they continue to execute properly.

This will also be useful for library upgrades.

For tests that need files to be loaded, on windows, the test must be run with the `--enable_runfiles` flag and in DeveloperMode
This is a poorly documented bazel issue
https://github.com/bazelbuild/bazel/issues/6402
https://github.com/bazelbuild/bazel/issues/8843#issuecomment-510503935


