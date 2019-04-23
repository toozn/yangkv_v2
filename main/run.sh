rm ./yang_log*
rm ./yang_sst*
bazel build :test
../bazel-bin/main/test
