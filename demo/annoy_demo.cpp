/**
 * file: httplib_demo.cp
 * date: 2020-08-27
 */


#include <algorithm>  
#include <random>    
#include <string>
#include <unordered_map>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <chrono>
#include <cassert>
#include <locale>
#include <codecvt>
#include <any>
#include <utility>
#include <set>
#include <math.h>
#include <unordered_map>

#include "include/v2vk/annoy_kit/index_builder.h"
#include "include/v2vk/annoy_kit/index_register.h"
#include "include/v2vk/3rdparty/wrapidjson/include/wrapidjson/type_register/type_register.h"




int32_t index_builder_demo() {
  const std::string case0_json_lines_file = "../test_data/json_lines.txt";
  auto case0_annoy_idx = v2vk::init_annoy_index<v2vk::AgnularKiss32RandomMTBIdx>(5);
  std::unordered_map<std::string, wrapidjson::Document> case0_auxiliary_info;
  
  v2vk::AnnoyIndex<v2vk::AgnularKiss32RandomMTBIdx, 5, std::string> case0_index_cls;
  std::unordered_map<int64_t, std::string> index_id2record_id;
  auto case0_tmp1 = v2vk::AnnoyIndex<v2vk::AgnularKiss32RandomMTBIdx, 5, std::string>::json_lines2index(
      case0_annoy_idx, case0_auxiliary_info, index_id2record_id, case0_json_lines_file, "id", "vec");
  std::cout << "Annoy index building result: " << case0_tmp1 << std::endl;

  std::vector<float> case0_query_vec = {0.2, 0.3, 0.5, 0.8, 0.9};
  std::vector< std::pair<std::string, float> > case0_query_results;
  auto case0_json_obj_results = wrapidjson::json_obj_init(true);
  case0_index_cls.build_index(case0_json_lines_file, "id", "vec");
  case0_index_cls.v2vid(case0_query_results, case0_query_vec, 3);
  case0_index_cls.v2vid(case0_json_obj_results, case0_query_vec, 3);
  for (auto x : case0_query_results) { std::cout << x.first << ": " << x.second << std::endl; }
  std::cout << wrapidjson::json_obj2string(case0_json_obj_results) << std::endl;

  return 0;
}


int main(int argc, char* argv[]) {
  index_builder_demo();
  return 0;
}
