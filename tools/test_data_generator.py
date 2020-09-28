# -*- coding: utf-8 -*-
# Date: 2020-09-27


import os
import random
import json




def generate_random_vec(dim=32):
    output = [round(random.random(), 2) for i in range(dim)]
    return output


def generate_random_id(id_len=16):
    output = [str(random.randint(0, 9)) for i in range(id_len)]
    output = "".join(output)
    return output


def generate_random_json_record(id_key="id", vec_key="vec", id_len=16, vec_dim=32):
    output = {}
    output[id_key] = generate_random_id(id_len)
    output[vec_key] = generate_random_vec(vec_dim)

    output = json.dumps(output, separators=(',', ':'))
    return output


def build_random_vec_dict(path, record_num, id_key="id", vec_key="vec", id_len=16, vec_dim=32):
    vec_dict_file = open(path, "w")
    for i in range(record_num):
        curr_random_record = generate_random_json_record(id_key, vec_key, id_len, vec_dim)
        vec_dict_file.write(curr_random_record + "\n")
    vec_dict_file.close()
    return 0



if __name__ == "__main__":
    """
    test0 = generate_random_vec(32)
    print(test0)
    test1 = generate_random_id(16)
    print(test1)
    test3 = generate_random_json_record("id", "vec", 16, 32)
    print(test3)
    """
    build_random_vec_dict("../test_data/json_lines.txt", 100000, "id", "vec", 16, 32)

    
