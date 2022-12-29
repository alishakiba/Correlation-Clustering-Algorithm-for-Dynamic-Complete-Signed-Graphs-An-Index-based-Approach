cc_test(
    name = "graph_test",
    size = "small",
    srcs = ["graph_test.cpp"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "//lib:Graph",
    ],
)

cc_test(
    name = "nao_test",
    size = "small",
    srcs = ["nao_test.cpp"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "//lib:Graph",
        "//lib:NAO",
    ],
)

# cc_test(
#     name = "naive_cc_test",
#     size = "small",
#     srcs = ["naive_cc_test.cpp"],
#     deps = [
#         "@com_google_googletest//:gtest_main",
#         "//lib:Graph",
#         "//lib:NAO",
#         "//lib:NaiveCorrelationClustering",
#     ],
# )

cc_test(
    name = "index_cc_test",
    size = "small",
    srcs = ["index_cc_test.cpp"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "//lib:Graph",
        "//lib:NAO",
        "//lib:NaiveCorrelationClustering",
        "//lib:IndexBasedCorrelationClustering",
    ],
)
