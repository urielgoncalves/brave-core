/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_call.h"
#include <sstream>
#include <string>
#include <vector>
#include "brave/third_party/blink/brave_page_graph/graph_item/edge.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_script.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_webapi.h"
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::string;
using ::std::stringstream;
using ::std::to_string;
using ::std::vector;

namespace brave_page_graph {

string graphml_edge_call_arguments(void* edge) {
  stringstream builder;
  builder << "(";
  const vector<string> args = static_cast<EdgeCall*>(edge)->Arguments();
  const size_t num_args = args.size();

  for (size_t i = 0; i < num_args; i += 1) {
    builder << to_string(i) << ": " << args[i];
    if (i < num_args - 1) {
      builder << ", ";
    }
  }
  builder << ")";
  return builder.str();
}

EdgeCall::EdgeCall(const PageGraph* graph, const PageGraphId id,
    const NodeScript* out_node, const NodeWebAPI* in_node,
    const vector<string>& arguments) :
      Edge(graph, id, out_node, in_node),
      arguments_(arguments) {}

EdgeCall::~EdgeCall() {}

string EdgeCall::ItemName() const {
  return "EdgeCall#" + ::std::to_string(id_);
}

GraphMLFuncAttrMap EdgeCall::GraphMLAttributeDefs() const {
  GraphMLFuncAttrMap mapping = GraphItem::GraphMLAttributeDefs();
  mapping.emplace(
    &graphml_edge_call_arguments,
    GraphMLAttr::Create(GraphMLAttrForTypeEdge, "arguments",
      GraphMLAttrTypeString));
  return mapping;
}

vector<string> EdgeCall::Arguments() const {
  return arguments_;
}

string EdgeCall::ToStringBody() const {
  stringstream string_builder;
  string_builder << ItemName() + " [arguments:";
  const size_t num_args = arguments_.size();
  const size_t last_index = num_args - 1;
  for (size_t i = 0; i < num_args; i += 1) {
    if (i == last_index) {
      string_builder << arguments_[i];
    } else {
      string_builder << arguments_[i] << ", ";
    }
  }
  string_builder << "]";
  return string_builder.str();
}

}  // brave_page_graph
