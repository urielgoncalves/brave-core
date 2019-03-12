/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item/node/node_actor.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/page_graph.h"
#include "brave/components/brave_page_graph/types.h"

namespace brave_page_graph {

NodeActor::NodeActor(const PageGraph* graph, const PageGraphId id) :
    Node(graph, id) {}

NodeActor::~NodeActor() {}

bool NodeActor::IsScript() const {
  return false;
};

bool NodeActor::IsParser() const {
  return false;
};

}  // namespace brave_page_graph
