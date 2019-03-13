/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graph_item/edge.h"
#include <ostream>
#include <sstream>
#include <string>
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/graph_item.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::endl;
using ::std::string;
using ::std::stringstream;
using ::std::to_string;

namespace brave_page_graph {

string graphml_edge_type(void* edge) {
  return "behavior";
}

Edge::Edge(const PageGraph* graph, const PageGraphId id, const Node* out_node,
    const Node* in_node) :
      GraphItem(graph, id) {
  in_node_ = in_node;
  out_node_ = out_node;
}

string Edge::GraphMLTag() {
  const string graphml_attributes = GraphMLAttributes();
  const bool has_graphml_attrs = graphml_attributes.size() > 0;

  stringstream builder;
  builder << "<edge id\"" << GraphMLId() << "\" " <<
                    "source=\"" << out_node_->GraphMLId() << "\" " <<
                    "target=\"" << in_node_->GraphMLId() << "\"";
  if (has_graphml_attrs == false) {
    builder << "/>" << endl;
    return builder.str();
  }

  builder << ">" << graphml_attributes << "</edge>" << endl;
  return builder.str();
}

GraphMLFuncAttrMap Edge::GraphMLAttributeDefs() const {
  GraphMLFuncAttrMap mapping = GraphItem::GraphMLAttributeDefs();
  mapping.emplace(
    &graphml_edge_type,
    GraphMLAttr::Create(GraphMLAttrForTypeEdge, "type",
      GraphMLAttrTypeString));
  return mapping;
}

string Edge::ToStringPrefix() const {
  return in_node_->ItemName() + " -> ";
}

string Edge::ToStringSuffix() const {
  return " -> " + out_node_->ItemName();
}

string Edge::GraphMLId() const {
  return "e" + to_string(id_);
}

}  // namespace brave_page_graph
