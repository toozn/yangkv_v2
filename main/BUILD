cc_library(
    name = "singleton",
    srcs = ["singleton.cpp"],
    hdrs = ["singleton.h"],
    deps = [":yangkvMain"],
)

cc_library(
    name = "builder",
    srcs = ["builder.cpp"],
    hdrs = ["builder.h"],
    deps = [
            "//memory:skiplist",
            ":versionedit",
            "//table:table_builder",
            "//utils:filename",],
    visibility = ["//memory:__pkg__"],
)

cc_library(
    name = "versionedit",
    srcs = ["versionedit.cpp"],
    hdrs = ["versionedit.h"],
    deps = ["//memory:skiplist",
            "//utils:env",],
)
cc_library(
    name = "versionset",
    srcs = ["versionset.cpp"],
    hdrs = ["versionset.h"],
    deps = [":versionedit",
            "//utils:hashhelper",
            "//utils:condlock",
            ],
    visibility = ["//memory:__pkg__"],
)
cc_library(
    name = "yangkvMain",
    srcs = ["yangkvMain.cpp"],
    hdrs = ["yangkvMain.h"],
    deps = ["//memory:compacter",
            "//utils:env",
            "//memory:writer",
            "//utils:status",
            "//utils:condlock",
            "//utils:hashhelper",
           ],
)

cc_binary(
	name = "test",
	srcs = ["test.cpp"],
	deps = [":singleton"],
	linkopts = [
        "-pthread",
    ],
)
