/**
 * @file NaiveCorrelationClustering.h
 * @author Ali Shakiba (a.shakiba.iran@gmail.com)
 * @brief Proposed Method and Comparison with Non-Index One 
 * @version 0.1
 * @date 2022-12-08
 * @copyright GNU GPLv3
 */

#include <iostream>
#include <chrono>
#include <fstream>
#include "lib/Graph.h"
#include "lib/NAO.h"
#include "lib/NaiveCorrelationClustering.h"
#include "lib/IndexBasedCorrelationClustering.h"

const unsigned int NUM_ARGS = 3;
std::vector<double> eps_schedule;
std::vector<std::pair<double, std::vector<unsigned long>*>>* hierarchical = nullptr;

void write_clustering_to_file(std::string filename, std::vector<unsigned long>* output);
void write_distribution_to_file(std::string filename, std::map<double, unsigned long>* output);
unsigned short show_menu();
void get_nao_construction_time(Graph *g);
void get_correlation_clustering(Graph *g, double eps, std::string output_prefix);
void get_index_based_correlation_clustering(Graph *g, double eps, std::string output_prefix);
std::map<double, unsigned long>* get_all_eps(Graph *g, std::string output_prefix);
void get_eps_schedule();
void set_eps_schedule();
void get_hierarchical_correlation_clustering(Graph *g, std::string output_prefix);

int main(int argc, char* argv[]) {
    if (argc < NUM_ARGS + 1) {
        std::cerr << "./main [input_filename] [output_prefix] [default_eps] [batch] [eps-schedule-file]" << std::endl;
        std::cerr << "./main [input_filename] [output_prefix] [default_eps] [auto-batch] [eps-schedule-length]" << std::endl;
        std::exit(1);
    }
    // reading the graph in memory
    std::string input_filename = argv[1];
    std::string output_prefix = argv[2];
    auto default_eps = std::stod(argv[3]);
    Graph *g = new Graph();
    auto t1 = std::chrono::high_resolution_clock::now();
    g->load_from_file(input_filename);
    auto t_read = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - t1
    );
    auto n = g->get_n();
    auto m = g->get_positive_m();
    std::cout << "Time to read the grph from file with n: " << n
            << ", m: " << m
            << " is: " << t_read.count() << " ms"
            << std::endl;
    auto get_all_eps_vect = new std::map<double, unsigned long>();
    unsigned long eps_schedule_len = 0ul;
    if (argc == NUM_ARGS + 1 + 2) {
        auto batch_mode = argv[4];
        if (batch_mode == "batch") {
            auto eps_schedule_file = argv[5];
            std::ifstream eps_file_handler(eps_schedule_file, std::ios::in);
            while(! eps_file_handler.eof()) {
                double eps = 0;
                eps_file_handler >> eps;
                eps_schedule.push_back(eps);
            }   
            std::sort(eps_schedule.begin(), eps_schedule.end());
        }
        else {
            // it is auto 
            eps_schedule_len = std::stoul(argv[5]);
        }
        get_nao_construction_time(g);
        get_correlation_clustering(g, default_eps, input_filename + "_" + output_prefix);
        get_index_based_correlation_clustering(g, default_eps, input_filename + "_" + output_prefix);
        get_all_eps_vect = get_all_eps(g, input_filename + "_" + output_prefix);
        if (eps_schedule_len != 0ul) {
            // we get `eps_schedule_len` of the eps values from
            // get_all_eps_vect
            auto count_diff_eps = get_all_eps_vect->size();
            auto sorted_eps_vect = new std::vector<double>(count_diff_eps, 0);
            for(auto e: *get_all_eps_vect) {
                sorted_eps_vect->push_back(e.first);
            }
            std::sort(sorted_eps_vect->begin(), sorted_eps_vect->end());
            auto step = count_diff_eps / eps_schedule_len;
            if (step == 0)
                step == 1; // get all of them 
            auto i = 0ul;
            for(auto e: *sorted_eps_vect) {
                if (i % step == 0)
                    eps_schedule.push_back(e);
                ++i;
            }
            eps_schedule.push_back(1.99);
        }
        get_eps_schedule();
        get_hierarchical_correlation_clustering(g, input_filename + "_" + output_prefix);
    }
    else {
        while (true) {
            auto command = show_menu();
            switch (command) {
                case 0: 
                    return EXIT_SUCCESS;
                    break;
                case 1:
                    get_nao_construction_time(g);
                    break;
                case 2: 
                    get_correlation_clustering(g, default_eps, input_filename + "_" + output_prefix);
                    break;
                case 3: 
                    get_index_based_correlation_clustering(g, default_eps, input_filename + "_" + output_prefix);
                    break;
                case 4: 
                    get_all_eps_vect = get_all_eps(g, input_filename + "_" + output_prefix);
                    break;
                case 5: 
                    get_eps_schedule();
                    break;
                case 6:
                    set_eps_schedule();
                    break; 
                case 7:
                    if (eps_schedule.size() > 0)
                        get_hierarchical_correlation_clustering(g, input_filename + "_" + output_prefix);
                    else
                        std::cerr << "No epsilon schedule is defined." << std::endl;
                    break; 
                case 8:
                    break; 
                case 9:
                    std::cout << "Default epsilon value is " 
                        << default_eps << std::endl;
                    break; 
                case 10:
                    std::cout << "Give me the default epsilon value: ";
                    std::cin >> default_eps;
                    break; 
                default:
                    std::cerr << "Invalid choice.";
            }
        }
    }
    return EXIT_SUCCESS;
}

void write_clustering_to_file(std::string filename, std::vector<unsigned long>* output) {
    auto out_file = new std::ofstream(filename);
    for(unsigned long i = 0ul; i < output->size(); ++i) {
        (*out_file) << i << "\t" << output->at(i) << std::endl;
    }
    out_file->close();
    delete out_file;
    out_file = nullptr;
}

unsigned short show_menu() {
    std::cout << "Please choose an option:" << "\n"
        << "0. Exit" << "\n"
        << "1. Get NAOs' construction time" << "\n"
        << "2. Get Correlation Clustering without indexing (result & time)" << "\n"
        << "3. Get Index-based Correlation Clustering (result & time)" << "\n"
        << "4. Get all values of Non-Agreement" << "\n"
        << "5. Get Hierarchical Schedule" << "\n"
        << "6. Set Hierarchical Schedule" << "\n"
        << "7. Get Hierarchical Clustering" << "\n"
        << "8. Get Flat Clustering" << "\n"
        << "9. Get default eps" << "\n"
        << "10. Set default eps" << "\n"
        << std::endl;
    unsigned short choice = -1;
    std::cin >> choice;
    return choice;
}

void get_nao_construction_time(Graph *g) {
    // constructing the NAOs for the current graph
    // note: it is just for timing purpose, as the 
    // naos would be created inside their corresponding
    // correlation clustering classes
    auto t1 = std::chrono::high_resolution_clock::now();
    NAO ** naos = nullptr;
    auto n = g->get_n();
    naos = new NAO*[n];
    for(unsigned long i = 0ul; i < n; ++i) {
        naos[i] = new NAO(i, g);
    }
    auto t_read = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - t1
    );
    std::cout << "Time to construct NAOs: " << t_read.count() << " ms"
            << std::endl;
    for(unsigned long i = 0ul; i < n; ++i) {
        delete naos[i];
    }
    delete [] naos;
}

void get_correlation_clustering(Graph *g, double eps, std::string output_prefix) {
    // doing the naive correlation clustering, without NAOs
    auto t1 = std::chrono::high_resolution_clock::now();
    auto naive_corr_clust = new NaiveCorrelationClustering(g);
    auto output = naive_corr_clust->query(eps);
    auto t_read = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - t1
    );
    write_clustering_to_file(output_prefix + "_" + std::to_string(eps) + "_naive.out", output);
    std::cout << "Time for naive correlation clustering: " << t_read.count() << " ms"
            << std::endl;
    delete naive_corr_clust;
}

void get_index_based_correlation_clustering(Graph *g, double eps, std::string output_prefix) {
    // doing the index-based correlation clustering with NAOs
    auto t1 = std::chrono::high_resolution_clock::now();
    auto index_corr_clust = new IndexBasedCorrelationClustering(g);
    auto output = index_corr_clust->query(eps);
    auto t_read = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - t1
    );
    write_clustering_to_file(output_prefix + "_" + std::to_string(eps) + "_index.out", output);
    std::cout << "Time for index-based correlation clustering: " << t_read.count() << " ms"
            << std::endl;
    delete index_corr_clust;
}

std::map<double, unsigned long>* get_all_eps(Graph *g, std::string output_prefix) {
    auto index_corr_clust = new IndexBasedCorrelationClustering(g);
    auto output = index_corr_clust->get_all_eps();
    write_distribution_to_file(output_prefix + "_eps.out", output);
    delete index_corr_clust;
    return output;
}

void write_distribution_to_file(std::string filename, std::map<double, unsigned long>* output) {
    auto out_file = new std::ofstream(filename);
    for(auto el: *(output)) {
        (*out_file) << el.first << "\t" << el.second << std::endl;
    }
    out_file->close();
    delete out_file;
    out_file = nullptr;
}

void get_eps_schedule() {
    std::cout << "Eps-schedule is of length " << eps_schedule.size()
        << std::endl;
    for(auto eps: eps_schedule)
        std::cout << eps << "\t";
    std::cout << std::endl;
}

void set_eps_schedule() {
    double eps = 0;
    std::cout << "Give the eps schedule one per-line, non-positive value to end:"
        << std::endl;
    while(true) {
        std::cin >> eps;
        if (eps <= 0)
            break;
        eps_schedule.push_back(eps);
    }
    std::sort(eps_schedule.begin(), eps_schedule.end());
}

void get_hierarchical_correlation_clustering(Graph *g, std::string output_prefix) {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto index_corr_clust = new IndexBasedCorrelationClustering(g);
    auto t_read = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - t1
    );
    std::cout << "Time for constructing the index-based correlation clustering object: " 
        << t_read.count() << " ms" << std::endl;
    
    for(auto eps: eps_schedule) {    
        // first, naive correlation clustering
        t1 = std::chrono::high_resolution_clock::now();
        auto naive_corr_clust = new NaiveCorrelationClustering(g);
        auto output_naive = naive_corr_clust->query(eps);
        write_clustering_to_file(output_prefix + "_" + std::to_string(eps) + "_naive.out", output_naive);
        t_read = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1
        );
        std::cout << "Time for querying naive correlation clustering (eps = " << eps << ") is: "
            << t_read.count() << " ms" << std::endl;
        index_corr_clust->reset_naos();
        // now use the index
        t1 = std::chrono::high_resolution_clock::now();
        auto output = index_corr_clust->query(eps);
        write_clustering_to_file(output_prefix + "_" + std::to_string(eps) + "_index.out", output);
        t_read = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1
        );
        std::cout << "Time for querying correlation clustering (eps = " << eps << ") is: "
            << t_read.count() << " ms" << std::endl;
        index_corr_clust->reset_naos();
    }

    delete index_corr_clust;
}
