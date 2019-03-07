/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_STORAGE_LOCALSTORAGE_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_STORAGE_LOCALSTORAGE_H_

#include <string>
#include "brave/components/brave_page_graph/graph_item/node/node_storage.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

class NodeStorageLocalStorage : public NodeStorage {
 public:
  NodeStorageLocalStorage() = delete;
  explicit NodeStorageLocalStorage(const PageGraphId id);
  explicit NodeStorageLocalStorage(const NodeStorageLocalStorage& node) = default;
  ~NodeStorageLocalStorage() = default;
  string ItemName() const;
};

}  // namespace brave_page_graph

#endif BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_STORAGE_LOCALSTORAGE_H_
