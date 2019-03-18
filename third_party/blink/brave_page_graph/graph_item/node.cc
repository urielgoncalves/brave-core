/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge.h"
#include "brave/third_party/blink/brave_page_graph/graph_item.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::endl;
using ::std::string;
using ::std::stringstream;
using ::std::to_string;
using ::std::vector;

namespace brave_page_graph {

Node::Node(const PageGraph* graph, const PageGraphId id) :
      GraphItem(graph, id) {}

Node::~Node() {}

void Node::AddInEdge(const Edge* const in_edge) {
  in_edges_.push_back(in_edge);
}

void Node::AddOutEdge(const Edge* const out_edge) {
  out_edges_.push_back(out_edge);
}

string Node::GraphMLId() const {
  return "n" + to_string(id_);
}

GraphMLXML Node::GraphMLTag() const {
  stringstream builder;
  builder << "<node id=\"" + GraphMLId() + "\">" << endl;
  for (const GraphMLXML& elm : GraphItem::GraphMLAttributes()) {
    builder << "\t" << elm << endl;
  }
  builder << "</node>" << endl;
  return builder.str();
}

string Node::ToStringPrefix() const {
  stringstream string_builder;
  for (const Edge* elm : in_edges_) {
    string_builder << elm->ItemName() << " -> \n";
  }
  string_builder << "  ";
  return string_builder.str();
}

string Node::ToStringSuffix() const {
  stringstream string_builder;
  string_builder << "\n";
  for (const Edge* elm : out_edges_) {
    string_builder << "     -> " << elm->ItemName() << "\n";
  }
  return string_builder.str();
}

}  // namespace brave_page_graph
