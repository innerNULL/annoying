/**
 * date: 2020-08-28
 */


#ifndef V2VK_ANNOY_INDEX_BUILDER_H_
#define V2VK_ANNOY_INDEX_BUILDER_H_

#ifndef ANNOYLIB_MULTITHREADED_BUILD
#define ANNOYLIB_MULTITHREADED_BUILD
#endif

#include <stdio.h>
#include <unordered_map>

#include "./index_register.h"
#include "../3rdparty/annoy/src/annoylib.h"
#include "../3rdparty/annoy/src/kissrandom.h"
#include "../3rdparty/wrapidjson/include/wrapidjson/handler/handler.h"
#include "../3rdparty/wrapidjson/include/wrapidjson/querier/querier.h"
#include "../3rdparty/wrapidjson/include/wrapidjson/type_register/type_register.h"
#include "../3rdparty/cill/include/cill/str_kit/str_helper.h"




namespace v2vk {


template<typename ANNOY_IDX_TYPE>
inline ANNOY_IDX_TYPE init_annoy_index(const int32_t vec_dim) {
  ANNOY_IDX_TYPE index = ANNOY_IDX_TYPE(vec_dim);
  return index;
}


template<typename ANNOY_IDX_TYPE, int VEC_DIM, typename ID_VAL_TYPE=std::string>
class AnnoyIndex {
 public:
  AnnoyIndex() { return; }

  int32_t build_index(const std::string json_lines_path, 
      const std::string id_key, const std::string vec_key, const std::string auxiliary_keys="") {
    json_lines2index(this->index, this->auxiliary_info, this->index_id2record_id, 
        json_lines_path, id_key, vec_key, auxiliary_keys);

    return 0;
  }

  int32_t v2vid(std::vector< std::pair<ID_VAL_TYPE, float> >& query_results, 
      std::vector<float> query_vec, int32_t top_n) {
    // Internal vars and initialization
    std::vector<int32_t> index_id_results;
    std::vector<double> result_simlarity_score;
    double* query_array = (double*)malloc(query_vec.size() * sizeof(double));
    for (int32_t i = 0; i < query_vec.size(); ++i) { query_array[i] = query_vec[i]; }

    this->index.get_nns_by_vector(query_array, top_n, -1, &index_id_results, &result_simlarity_score);

    for (int32_t i = 0; i < index_id_results.size(); ++i) {
      int32_t index_id = index_id_results[i];
      float simlarity_score = result_simlarity_score[i];
      query_results.emplace_back(std::pair<ID_VAL_TYPE, float>(index_id2record_id[index_id], simlarity_score));
    }

    free(query_array);
    return 0;
  }

  int32_t v2vid(wrapidjson::Document& query_results, std::vector<float> query_vec, int32_t top_n, 
      std::string id_key="id", std::string sim_score_key="score", std::string result_key="result") {
    // Internal vars
    std::vector< std::pair<ID_VAL_TYPE, float> > raw_query_results;
    std::vector<wrapidjson::Document> tmp_result_holder;
    std::vector<wrapidjson::Document*> tmp_result_pointer_holder;

    v2vid(raw_query_results, query_vec, top_n);
    tmp_result_holder.resize(raw_query_results.size());

    int32_t result_id = 0;
    for(auto& result : tmp_result_holder) {
      auto tmp_json_obj = wrapidjson::json_obj_init(true);
      wrapidjson::json_insert_str(tmp_json_obj, id_key, raw_query_results[result_id].first, true);
      wrapidjson::json_insert_num<float>(tmp_json_obj, sim_score_key, raw_query_results[result_id].second, true);
      wrapidjson::doc_assign(result, tmp_json_obj);
      ++result_id;
    }

    for (auto& result : tmp_result_holder) { tmp_result_pointer_holder.emplace_back(&result); } 
    wrapidjson::json_insert_doc_array(query_results, result_key, tmp_result_pointer_holder, true);
    return 0;
  }

  static int32_t json_lines2index(
      ANNOY_IDX_TYPE& target_index, 
      std::unordered_map<ID_VAL_TYPE, wrapidjson::Document>& auxiliary_info, 
      std::unordered_map<int64_t, ID_VAL_TYPE>& index_id2record_id, 
      const std::string json_lines_path, const std::string id_key, const std::string vec_key, 
      const std::string auxiliary_keys="") {
   
    //std::cout << VEC_DIM << std::endl;
    FILE* json_lines_file = std::fopen(json_lines_path.c_str(), "r");

    if (json_lines_file == NULL) { return 1; }

    // Internal vars
    char io_buffer[4096];
    wrapidjson::Document json_line_obj = wrapidjson::json_obj_init(true);
    std::vector<std::string> auxiliary_keys_vec = cill::str_spliter(auxiliary_keys, ",");

    while(fgets(io_buffer, sizeof(io_buffer) - 1, json_lines_file) != NULL) {

      json_line_obj.Parse(io_buffer);
      //std::cout << wrapidjson::json_obj2string(json_line_obj) << std::endl;

      // name scope vars initialization
      ID_VAL_TYPE record_id = json_line_obj[id_key.c_str()].GetString();
      int64_t index_id = index_id2record_id.size();
      std::vector<double> target_vec; 
      wrapidjson::json_num_array2std_vec<double>(target_vec, json_line_obj, vec_key);
      double* target_array = (double*)malloc(target_vec.size() * sizeof(double));
      for (int32_t i = 0; i < target_vec.size(); ++i) { target_array[i] = target_vec[i]; }

      if (auxiliary_info.find(record_id) == auxiliary_info.end()) {
        target_index.add_item(index_id, target_array);
        index_id2record_id.insert(std::pair<int64_t, ID_VAL_TYPE>(index_id, record_id));
        // TODO (innerNULL) Add auxiliary_info cache logic.
        //auxiliary_info.insert(std::pair<>)
      }

      free(target_array);
    }
    target_index.build(1);

    fclose(json_lines_file);
    return 0;
  }

 private:
  ANNOY_IDX_TYPE index = ANNOY_IDX_TYPE(VEC_DIM);
  std::unordered_map<ID_VAL_TYPE, wrapidjson::Document> auxiliary_info;
  std::unordered_map<int64_t, ID_VAL_TYPE> index_id2record_id;
};


} // namespace v2vk


#endif
