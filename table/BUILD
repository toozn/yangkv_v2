cc_library(
    name = "block",
    srcs = ["block.cpp"],
    hdrs = ["block.h"],
    deps = [":format",],
    visibility = ["//main:__pkg__", "//memory:__pkg__"],
)

cc_library(
    name = "format",
    srcs = ["format.cpp"],
    hdrs = ["format.h"],
    deps = ["//utils:slice",
            "//utils:status",
            "//utils:dbformat",
            "//utils:option",
            "//utils:env"],
    visibility = ["//main:__pkg__", "//memory:__pkg__"],
)

cc_library(
    name = "table_builder",
    srcs = ["table_builder.cpp"],
    hdrs = ["table_builder.h"],
    deps = [":format",
            ":block",
            "//utils:env",
            ":block_builder",],
    visibility = ["//main:__pkg__", "//memory:__pkg__"],
)

cc_library(
    name = "block_builder",
    srcs = ["block_builder.cpp"],
    hdrs = ["block_builder.h"],
    deps = [":format",
            ":block",

            "//utils:comparator",
            "//utils:env"],
    visibility = ["//main:__pkg__", "//memory:__pkg__"],
)



