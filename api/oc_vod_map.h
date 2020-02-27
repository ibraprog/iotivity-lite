/*
// Copyright (c) 2020 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#include "oc_rep.h"
#include "util/oc_list.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef OC_VOD_MAP_H
#define OC_VOD_MAP_H

/*
{
  "vods" : [
  {"vod_id":"virtual_device_id-1", "econame": "UPnP", "index":1},
  {"vod_id":"virtual_device_id-2", "econame": "ZigBee", "index":2}
  ],
  "next_index": 3
}
*/

typedef struct oc_vod_list_t
{
  OC_LIST_STRUCT(vods);
  size_t next_index;
} oc_vod_list_t;

typedef struct oc_vod_t
{
  struct oc_vod_t *next;
  uint8_t *vod_id;
  size_t vod_id_size;
  oc_string_t econame;
  size_t index;
} oc_vod_t;

/*
 * open vod_map file from creds directory and populate
 * oc_vod_list_t
 */
void oc_vod_map_init();

/*
 * release all of the memory
 */
void oc_vod_map_free();
/*
 * returns index of the vod or 0 if not found
 */
size_t oc_vod_map_get_id_index(uint8_t *vod_id, size_t vod_id_size,
                               char *econame);

/*
 * add the vod_id to the the oc_vod_list_t
 * update next_index
 * write updated vod_map file
 * return index of just added vod
 */
size_t oc_vod_map_add_id(uint8_t *vod_id, size_t vod_id_size, char *econame);

/*
 * NOT NEEDED IN HEADER FILE REMOVE ONCE IMPLEMENTED
 */
bool oc_vod_map_decode(oc_rep_t *rep, bool from_storage);

/*
 * load vod_map file and pass bytes to decode to populate oc_vod_list_t
 *
 * reference oc_sec_load_acl(size_t device) in oc_store.c
 */
void oc_vod_map_load();

/*
 * responsible for encoding the oc_vod_list_t to cbor
 * function will be used by dump_vod_map()
 */
void oc_vod_map_encode();
/*
 * convert the oc_vod_list_t to cbor
 * dump cbor bytes to vod_map file
 *
 * reference oc_sec_dump_acl(size_t device) in oc_store.c
 */
void oc_vod_map_dump();

#endif // OC_VOD_MAP_H
