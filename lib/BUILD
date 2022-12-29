cc_library (
    name = "Graph",
    srcs = ["Graph.cpp"],
    hdrs = ["Graph.h"],
    visibility = [
        "//main:__pkg__",
        "//tests:__pkg__",
    ],
    linkopts = [
        '-lboost_log',
    ]
)

cc_library (
    name = "NAO",
    srcs = ["NAO.cpp"],
    hdrs = ["NAO.h"],
    visibility = [
        "//main:__pkg__",
        "//tests:__pkg__",
    ],
    deps = [
        "//lib:Graph",
    ],
)

cc_library (
    name = "NaiveCorrelationClustering",
    srcs = ["NaiveCorrelationClustering.cpp"],
    hdrs = ["NaiveCorrelationClustering.h"],
    visibility = [
        "//main:__pkg__",
        "//tests:__pkg__",
    ],
    deps = [
        "//lib:Graph",
    ],
)

cc_library (
    name = "IndexBasedCorrelationClustering",
    srcs = ["IndexBasedCorrelationClustering.cpp"],
    hdrs = ["IndexBasedCorrelationClustering.h"],
    visibility = [
        "//main:__pkg__",
        "//tests:__pkg__",
    ],
    deps = [
        "//lib:Graph",
        "//lib:NAO",
        "//lib:NaiveCorrelationClustering",
    ],
)
