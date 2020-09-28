# v2vk
v2vk(Vevtor2Vector Kit). This project includes all kind of vec2vec utils, such as wraping of Annoy index (faiss? maybe later), with which it will be convinent to build and use a index.  
Besides of that, the project also support servitization of vector-indexing, for now the server is very simple and crude，but it will be better and better in future.

## Vector Data
The item vector data should be saving in a text file, I call it "vector dict", each row shoud be a json string, which represents a single records, the reason using json format is, json is very elastic, and besides record id and vector, we can also saving some extension data in json and returned together with nearest items' id and score during quering.

## Dependencies
This is a header only lib, you can just put the path in your project if you need to integrade it, following are the dependencies.
* **OpenSSL**: Depended by sougou workflow, if mac, run `export OPENSSL_ROOT_DIR=/usr/local/opt/openss` before compile v2vk and workflow.
* **workflow**(https://github.com/sogou/workflow): An easy using cpp web framework.

## Starts Servering
* Update the config file in `conf`.
* run `mkdir build && cd build && cmake ../ && make -j7`
* run `./annoy_v2v_server ../conf/annoy_v2v_server_dev.json`
* test service, run `curl http://127.0.0.1:8888?top_n=10&query_vec=0.52,0.85,0.36,0.6,0.28,0.04,0.7,0.65,0.65,0.03,0.87,0.69,0.97,0.37,0.71,0.26,0.53,0.01,0.67,0.27,0.12,0.94,0.18,0.84,0.86,0.31,0.8,0.62,0.61,0.82,0.12,0.11`
