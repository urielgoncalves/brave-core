/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_webapi.h"
#include <string>
#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::string;
using ::std::to_string;

namespace brave_page_graph {

NodeWebAPI::NodeWebAPI(const PageGraph* graph, const PageGraphId id,
    const MethodName method) :
      Node(graph, id),
      method_name_(method) {}

NodeWebAPI::~NodeWebAPI() {}

string NodeWebAPI::ItemName() const {
  return "NodeWebAPI#" + to_string(id_);
}

const MethodName& NodeWebAPI::Method() const {
  return method_name_;
}

string NodeWebAPI::ToStringBody() const {
  return ItemName() + " [method:" + method_name_ + "]"; 
}

}  // brave_page_graph
