
cc_library(
    name = "message_queue",
    srcs = ["message_queue.cpp"],
    hdrs = ["message_queue.h"],
    deps = ["//utils:dbformat",],
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
            "compacter",
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
            ],
    visibility = ["//main:__pkg__"],
)
