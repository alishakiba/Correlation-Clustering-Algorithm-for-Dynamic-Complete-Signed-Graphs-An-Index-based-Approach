#!python3 
'''!
 @file preprocess.py
 @author Ali Shakiba (a.shakiba.iran@gmail.com)
 @brief Preprocesses the SNAP files and makes them 
 ready for my code
 @version 0.1
 @date 2022-12-02
 @copyright GNU GPLv3
'''
import sys

if __name__ == "__main__":
    input_filename = sys.argv[1]
    output_filename = sys.argv[2]
    print(f'Going to read from {input_filename} and writting to {output_filename}.')
    g = open(output_filename, 'w')
    with open(input_filename, 'r') as f:
        n, m = f.readline().strip().split()
        g.write(f'{n}\t{m}\n')
        last_index = 0
        id_to_n = dict()
        for line in f:
            u, v = line.strip().split()
            if not u in id_to_n:
                id_to_n[u] = last_index
                last_index += 1
            if not v in id_to_n:
                id_to_n[v] = last_index
                last_index += 1
            u, v = min(u,v), max(u,v)
            g.write(f'{id_to_n[u]}\t{id_to_n[v]}\n')
    g.close()
    print(f'Total: {n} vertices with {m} edges. last index: {last_index}')