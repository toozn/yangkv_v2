cc_library(
    name = "slice",
    hdrs = ["slice.h"],
    deps = [":status",],
    visibility = ["//main:__pkg__", "//memory:__pkg__", "//table:__pkg__"],
)

cc_library(
    name = "comparator",
    hdrs = ["comparator.h"],
    deps = [":slice",],
    visibility = ["//main:__pkg__", "//memory:__pkg__", "//table:__pkg__"],
)

cc_library(
    name = "option",
    hdrs = ["option.h"],
    deps = [":slice",
            ":env",
            ":comparator",],
    visibility = ["//main:__pkg__", "//memory:__pkg__", "//table:__pkg__"],
)

cc_library(
    name = "filename",
    hdrs = ["filename.h"],
    visibility = ["//main:__pkg__", "//memory:__pkg__", "//table:__pkg__"],
)

cc_library(
    name = "condlock",
    hdrs = ["condlock.h"],
    visibility = ["//main:__pkg__", "//memory:__pkg__", "//table:__pkg__"],
)


cc_library(
    name = "dbformat",
    srcs = ["dbformat.cpp"],
    hdrs = ["dbformat.h"],
    deps = [":env",],
    visibility = ["//main:__pkg__", "//memory:__pkg__", "//table:__pkg__"],
)


cc_library(
    name = "bloomfilter",
    srcs = ["bloomfilter.cpp"],
    hdrs = ["bloomfilter.h"],
    visibility = ["//main:__pkg__"],
)

cc_library(
    name = "hashhelper",
    srcs = ["hash_helper.cpp"],
    hdrs = ["hash_helper.h"],
    visibility = ["//main:__pkg__"],
)

cc_library(
    name = "pointcache",
    srcs = ["pointcache.cpp"],
    hdrs = ["pointcache.h"],
    visibility = ["//main:__pkg__"],
)

cc_library(
    name = "logwriter",
    srcs = ["logwriter.cpp"],
    hdrs = ["logwriter.h"],
    deps = [":status",
            ":slice",
            ":dbformat",
            "//main:versionedit",],
    visibility = ["//main:__pkg__", "//memory:__pkg__", "//table:__pkg__"],
)

cc_library(
    name = "logreader",
    srcs = ["logreader.cpp"],
    hdrs = ["logreader.h"],
    deps = [":status",
            ":slice",
            ":env",
            ":dbformat",],
    visibility = ["//main:__pkg__", "//memory:__pkg__", "//table:__pkg__"],
)


cc_library(
    name = "env",
    srcs = ["env.cpp"],
    hdrs = ["env.h"],
    deps = [":status",
            ":slice",
            ],
    visibility = ["//main:__pkg__", "//memory:__pkg__", "//table:__pkg__"],
)

cc_library(
    name = "config",
    hdrs = ["config.h"],
    visibility = ["//main:__pkg__", "//memory:__pkg__", "//table:__pkg__"],
)

cc_library(
    name = "status",
    hdrs = ["status.h"],
    visibility = ["//main:__pkg__", "//memory:__pkg__", "//table:__pkg__"],
)
