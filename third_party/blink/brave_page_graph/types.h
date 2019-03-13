/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_TYPES_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_TYPES_H_

#include <map>
#include <string>

using ::std::map;
using ::std::string;

namespace brave_page_graph {

class GraphMLAttr;

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
string request_type_to_string(const RequestType type) noexcept;

typedef enum {
  ScriptTypeClassic = 0,
  ScriptTypeModule,
  ScriptTypeExtension,
  ScriptTypeUnknown
} ScriptType;
string script_type_to_string(const ScriptType type) noexcept;

typedef string (*GraphMLValueFunc)(void*);
typedef map<GraphMLValueFunc,GraphMLAttr> GraphMLFuncAttrMap;
typedef string MethodName;
typedef unsigned long long DOMNodeId;
typedef unsigned long long PageGraphId;
typedef int ScriptId;

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_TYPES_H_
