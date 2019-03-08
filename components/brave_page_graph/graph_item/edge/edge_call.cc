/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item/edge/edge_call.h"
#include <sstream>
#include <string>
#include <vector>
#include "brave/components/brave_page_graph/graph_item/edge.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;
using ::std::stringstream;
using ::std::vector;

namespace brave_page_graph {

EdgeCall::EdgeCall(const PageGraphId id, const Node* in_node,
    const Node* out_node, const vector<string>& arguments) :
    Edge(id, in_node, out_node),
    arguments_(arguments) {}

EdgeCall::~EdgeCall() {}

string EdgeCall::ItemName() const {
  return "EdgeCall#" + ::std::to_string(id_);
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
