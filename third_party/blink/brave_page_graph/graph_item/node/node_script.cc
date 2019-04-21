/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_script.h"
#include <string>
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_actor.h"
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::to_string;

namespace brave_page_graph {

NodeScript::NodeScript(PageGraph* const graph, const ScriptId script_id,
    const ScriptType type) :
      NodeActor(graph),
      script_id_(script_id),
      type_(type) {}

NodeScript::~NodeScript() {}

ItemName NodeScript::GetItemName() const {
  return "NodeScript#" + to_string(id_);
}

bool NodeScript::IsScript() const {
  return true;
}

GraphMLXMLList NodeScript::GraphMLAttributes() const {
  return GraphMLXMLList({
    graphml_attr_def_for_type(kGraphMLAttrDefNodeType)
      ->ToValue("script"),
    graphml_attr_def_for_type(kGraphMLAttrDefScriptType)
      ->ToValue(script_type_to_string(type_))
  });
}

ItemDesc NodeScript::GetDescBody() const {
  return GetItemName() +
    " [ScriptId:" + to_string(script_id_) +
    ", Type:"  + script_type_to_string(type_) + "]"; 
}

}  // namespace brave_page_graph
