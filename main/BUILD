cc_library(
    name = "singleton",
    srcs = ["singleton.cpp"],
    hdrs = ["singleton.h"],
    deps = [":yangkvMain"]
)

cc_library(
    name = "versionedit",
    srcs = ["versionedit.cpp"],
    hdrs = ["versionedit.h"],
)
cc_library(
    name = "versionset",
    srcs = ["versionset.cpp"],
    hdrs = ["versionset.h"],
    deps = [":versionedit",
            "//memory:writer",
            "//memory:compacter",
            ],
)
cc_library(
    name = "yangkvMain",
    srcs = ["yangkvMain.cpp"],
    hdrs = ["yangkvMain.h"],
    deps = ["//memory:writer", 
            "//memory:compacter",
            "//utils:env",
            "//utils:hashhelper",
            ":versionset",
            "//utils:status",
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
