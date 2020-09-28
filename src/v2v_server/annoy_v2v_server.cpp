// date: 2020-09-23


#include <iostream>
#include <string>
#include <sstream>

#include "workflow/WFHttpServer.h"
#include "include/v2vk/annoy_kit/index_builder.h"
#include "include/v2vk/3rdparty/wrapidjson/include/wrapidjson/handler/handler.h"
#include "include/v2vk/3rdparty/wrapidjson/include/wrapidjson/type_register/type_register.h"
#include "include/v2vk/3rdparty/wrapidjson/include/wrapidjson/io_helper/io_helper.h"
#include "include/v2vk/3rdparty/cill/include/cill/str_kit/str_parser.h"




/**
 * @brief
 * Test url: http://127.0.0.1:8888?top_n=10&query_vec=0.2,0.3,0.5,0.8,0.9
 */
int32_t http_task_processor(WFHttpTask* task, 
    v2vk::AnnoyIndex<v2vk::AgnularKiss32RandomMTBIdx, 5, std::string>& vec_index) {
  
  // Internal vars
  protocol::HttpRequest* req = task->get_req();
  protocol::HttpResponse* resp = task->get_resp();
  wrapidjson::Document query_results = wrapidjson::json_obj_init(true);

  // Using vars
  std::string uri = req->get_request_uri();
  std::string uri_param_str = uri; cill::substr_replace(uri_param_str, "/?", "");
  std::unordered_map<std::string, std::string> uri_params;
  cill::str_k_str_v_groups_str_parser(uri_params, uri_param_str, "&", "=");
  //std::cout << uri_param_str << std::endl;
  //for (auto& item : uri_params) { std::cout << item.first << ": " << item.second << std::endl; }

  // Query params
  int32_t top_n = std::stoi(uri_params["top_n"]);
  std::vector<float> query_vec; cill::str2num_vec<float>(query_vec, uri_params["query_vec"], ",");

  vec_index.v2vid(query_results, query_vec, top_n);
  resp->append_output_body(wrapidjson::json_obj2string(query_results));
  return 0;
}


int main(int argc, char* argv[]) {
  std::string conf_path;
  if (argc == 1) {
    conf_path = "../conf/annoy_v2v_server_dev.json";
  } else {
    conf_path = argv[1]; 
  }
  wrapidjson::Document conf = wrapidjson::json_obj_init(true);
  wrapidjson::json_file2json_obj(conf, conf_path);

  std::string vec_dict_path = conf["vec_dict_path"].GetString(); 
  int32_t port = conf["port"].GetInt();
  v2vk::AnnoyIndex<v2vk::AgnularKiss32RandomMTBIdx, 5, std::string> vec_index;
  
  vec_index.build_index(vec_dict_path, conf["record_id_key"].GetString(), conf["record_vec_key"].GetString());

  struct WFServerParams params = HTTP_SERVER_PARAMS_DEFAULT;
  params.request_size_limit = 8 * 1024 * 1024;

  WFHttpServer server(&params, [&vec_index](WFHttpTask *task) { http_task_processor(task, vec_index); });

  if (server.start(port) == 0) { 
    getchar(); 
    server.stop();
  }
  return 0;
}

