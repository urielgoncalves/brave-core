/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item/edge/edge_node.h"
#include "brave/components/brave_page_graph/graph_item/edge.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/graph_item/node/node_actor.h"
#include "brave/components/brave_page_graph/graph_item/node/node_html.h"
#include "brave/components/brave_page_graph/page_graph.h"
#include "brave/components/brave_page_graph/types.h"

namespace brave_page_graph {

EdgeNode::EdgeNode(const PageGraph* graph, const PageGraphId id,
    const NodeActor* out_node, const NodeHTML* in_node) :
    Edge(graph, id, out_node, in_node) {}

}  // brave_page_graph
