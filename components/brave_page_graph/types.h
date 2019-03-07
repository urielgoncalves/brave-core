/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_TYPES_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_TYPES_H_

#include <memory>
#include <string>
#include <vector>
#include "third_party/blink/renderer/platform/graphics/dom_node_id.h"
#include "brave/components/brave_page_graph/graph_item/edge.h"

using ::std::string;
using ::std::unique_ptr;
using ::std::vector;
using ::std::weak_ptr;

namespace brave_page_graph {

typedef enum {
  RequestTypeImage = 0,
  RequestTypeScriptClassic,
  RequestTypeScriptModule,
  RequestTypeCSS,
  RequestTypeVideo,
  RequestTypeAudio,
  RequestTypeSVG,
  RequestTypeFont,
  RequestTypeDocument,
  RequestTypeUnknown
} RequestType;
string request_type_to_string(const RequestType type);

typedef enum {
  ScriptTypeClassic = 0,
  ScriptTypeModule,
  ScriptTypeExtension,
  ScriptTypeUnknown
} ScriptType;
string script_type_to_string(const ScriptType type);

typedef blink::DOMNodeId DOMNodeId;
typedef uint32_t PageGraphId;
typedef int ScriptId;

}  // namespace brave_page_graph

#endif BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_TYPES_H_
