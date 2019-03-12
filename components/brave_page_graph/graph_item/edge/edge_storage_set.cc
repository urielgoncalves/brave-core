/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item/edge/edge_storage_set.h"
#include <string>
#include "brave/components/brave_page_graph/graph_item/edge/edge_storage.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/graph_item/node/node_actor.h"
#include "brave/components/brave_page_graph/graph_item/node/node_storage.h"
#include "brave/components/brave_page_graph/page_graph.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;
using ::std::to_string;

namespace brave_page_graph {

EdgeStorageSet::EdgeStorageSet(const PageGraph* graph, const PageGraphId id,
    const NodeActor* out_node, const NodeStorage* in_node, const string& key,
    const string& value) :
    EdgeStorage(graph, id, out_node, in_node, key),
    value_(value) {}

EdgeStorageSet::~EdgeStorageSet() {}

string EdgeStorageSet::ItemName() const {
  return "EdgeStorageSet#" + to_string(id_);
}

string EdgeStorageSet::ToStringBody() const {
  return ItemName() + " [key:" + key_ + ", value:" + value_ + "]";
}

}  // brave_page_graph
