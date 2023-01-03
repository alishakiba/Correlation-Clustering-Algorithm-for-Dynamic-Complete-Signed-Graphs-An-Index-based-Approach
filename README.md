# Correlation Clustering Algorithm for Dynamic Complete Signed Graphs: An Index-based Approach

Source code for the following [paper](https://arxiv.org/abs/2301.00384):
```latex
@misc{https://doi.org/10.48550/arxiv.2301.00384,
  doi = {10.48550/ARXIV.2301.00384},
  url = {https://arxiv.org/abs/2301.00384},
  author = {Shakiba, Ali},
  keywords = {Data Structures and Algorithms (cs.DS), Machine Learning (cs.LG), FOS: Computer and information sciences, FOS: Computer and information sciences},
  title = {Correlation Clustering Algorithm for Dynamic Complete Signed Graphs: An Index-based Approach},
  publisher = {arXiv},
  year = {2023}
}
```

## Requirements
* `boost` library: the `boost_log` library for logging, tested with version 1.74 shipped with ubuntu (`sudo apt install sudo apt install libboost-log1.74.0 libboost-log1.74-dev`)
* bazel build system
* Google Test Suit

## Running the experiments
* A sample running workflow:
    1. Run the `preprocess.py` for the datasets downloaded from the [SNAP](https://snap.stanford.edu/data/)
    1. Then, run the following command to do a all-in-one experiment for a dataset: `bazel run //main:all input_filename output_prefix default_eps batch-auto eps-schedule-length`.

* There are two general forms for running this code in `batch` mode:
    1. Using a pre-generated epsilon schedule in a file: `bazel run //main:all [input_filename] [output_prefix] [default_eps] [batch] [eps-schedule-file]`.
    1. Using the distribution of the non-agreement among the edges to generate a equal-width epsilon schedule: `bazel run //main:all [input_filename] [output_prefix] [default_eps] [auto-batch] [eps-schedule-length]`.

* If you want to run the experiments interactively, then just run ```bazel run //main:all [input_filename] [output_prefix] [default_eps]``.

* All the output files would be put in the `data\*.out` files tagged with the `output_prefix`.


## Documentation
Use `doxygen` to generate the docmentation for the project. 
```sh
doxygen Doxyfile
```
## Testing 
For testing, just run the following set of commands
```sh
bazel test --test_output=all --show_progress  //tests:graph_test
```
