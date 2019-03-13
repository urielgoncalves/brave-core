/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_parser.h"
#include <string>
#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_actor.h"
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::string;
using ::std::to_string;

namespace brave_page_graph {

string graphml_node_parser_type(void* node) {
  return "parser";
}

NodeParser::NodeParser(const PageGraph* graph, const PageGraphId id) :
      NodeActor(graph, id) {}

NodeParser::~NodeParser() {}

string NodeParser::ItemName() const {
  return "NodeParser#" + to_string(id_);
}

bool NodeParser::IsParser() const {
  return true;
}

GraphMLFuncAttrMap NodeParser::GraphMLAttributeDefs() const {
  GraphMLFuncAttrMap mapping = Node::GraphMLAttributeDefs();
  mapping.emplace(
    &graphml_node_parser_type,
    GraphMLAttr::Create(GraphMLAttrForTypeNode, "type", GraphMLAttrTypeString));
  return mapping;
}

}  // brave_page_graph
