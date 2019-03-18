/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graph_item/edge.h"
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
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

Edge::Edge(const PageGraph* graph, const PageGraphId id,
    const Node* const out_node, const Node* const in_node) :
      GraphItem(graph, id),
      out_node_(out_node),
      in_node_(in_node) {}

GraphMLXML Edge::GraphMLTag() const {
  const vector<const GraphMLXML> graphml_attributes = GraphMLAttributes();
  const bool has_graphml_attrs = graphml_attributes.size() > 0;

  stringstream builder;
  builder << "<edge id\"" << GraphMLId() << "\" " <<
                    "source=\"" << out_node_->GraphMLId() << "\" " <<
                    "target=\"" << in_node_->GraphMLId() << "\"";
  if (has_graphml_attrs == false) {
    builder << "/>" << endl;
    return builder.str();
  }

  builder << ">" << endl;
  for (const GraphMLXML& elm : graphml_attributes) {
    builder << "\t" << elm << endl;
  }
  builder << "</edge>" << endl;
  return builder.str();
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
