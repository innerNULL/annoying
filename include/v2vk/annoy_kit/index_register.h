/**
 * date: 2020-08-28
 */

#ifndef V2VK_ANNOY_INDEX_REGISTER_H_
#define V2VK_ANNOY_INDEX_REGISTER_H_

#ifndef ANNOYLIB_MULTITHREADED_BUILD
#define ANNOYLIB_MULTITHREADED_BUILD
#endif

#include "../3rdparty/annoy/src/annoylib.h"
#include "../3rdparty/annoy/src/kissrandom.h"
#include "../3rdparty/wrapidjson/include/wrapidjson/handler/handler.h"


namespace v2vk {


typedef AnnoyIndex<int, double, Angular, Kiss32Random, AnnoyIndexMultiThreadedBuildPolicy> AgnularKiss32RandomMTBIdx;


} // namespace v2vk


#endif
