
cc_library(
    name = "message_queue",
    srcs = ["message_queue.cpp"],
    hdrs = ["message_queue.h"],
    deps = ["//utils:dbformat",
            "//utils:env",
            "//utils:logwriter",
            "//utils:logreader",
            "//utils:filename"],
    visibility = ["//main:__pkg__"],
)

cc_library(
	name = "skiplist",
    srcs = ["skiplist.h"],
    deps = ["//utils:dbformat",
    		"//utils:status",
    		],
    visibility = ["//main:__pkg__"],
)
cc_library(
	name = "writer",
    srcs = ["writer.cpp"],
    hdrs = ["writer.h"],
    deps = [ "//utils:config", 
             "//utils:dbformat",
             ":skiplist",
             ":message_queue",
             "//main:versionset",
            ],
    visibility = ["//main:__pkg__"],
)

cc_library(
	name = "compacter",
    srcs = ["compacter.cpp"],
    hdrs = ["compacter.h"],
    deps = [":message_queue", 
            "//utils:config",
            ":skiplist",
            "//utils:condlock",
            "//main:builder",
            "//main:versionset",
            "//utils:env",
            ],
    visibility = ["//main:__pkg__"],
)
