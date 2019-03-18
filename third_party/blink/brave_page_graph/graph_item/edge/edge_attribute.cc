/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_attribute.h"
#include <string>
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_actor.h"
#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

EdgeAttribute::EdgeAttribute(const PageGraph* graph, const PageGraphId id,
    const NodeActor* const out_node, const Node* const in_node,
    const string& name) :
      Edge(graph, id, out_node, in_node),
      name_(name) {}

const string& EdgeAttribute::AttributeName() const {
  return name_;
}

GraphMLXMLGroup EdgeAttribute::GraphMLAttributes() const {
  GraphMLXMLGroup attrs;
  attrs.push_back(graphml_attr_def_for_type(GraphMLAttrDefKey)
      ->ToValue(AttributeName()));
  return attrs;
}

}  // brave_page_graph
