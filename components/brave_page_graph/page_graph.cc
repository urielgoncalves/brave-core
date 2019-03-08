/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/page_graph.h"
#include <iostream>
#include "brave/components/brave_page_graph/graph_item/node/node_storage_cookiejar.h"
#include "brave/components/brave_page_graph/graph_item/node/node_storage_localstorage.h"
#include "brave/components/brave_page_graph/graph_item/node/node_parser.h"
#include "brave/components/brave_page_graph/graph_item/node/node_html.h"
#include "brave/components/brave_page_graph/graph_item/node/node_script.h"
#include "brave/components/brave_page_graph/graph_item/node/node_shields.h"
#include "brave/components/brave_page_graph/types.h"

namespace brave_page_graph {

PageGraph::PageGraph() {
  std::cout << "Allocating PageGraph\n";
  graph_id_counter_ = 0;
  node_parser_ = new NodeParser(this, graph_id_counter_++);
  graph_items_.push_back(node_parser_);

  node_shields_ = new NodeShields(this, graph_id_counter_++);
  graph_items_.push_back(node_shields_);

  node_cookie_jar_ = new NodeStorageCookieJar(this, graph_id_counter_++);
  graph_items_.push_back(node_cookie_jar_);

  node_local_storage_ = new NodeStorageLocalStorage(this, graph_id_counter_++);
  graph_items_.push_back(node_local_storage_);
}

PageGraph::~PageGraph() {
  std::cout << "Deallocating PageGraph\n";
  // Since graph item is the one place that owns the items in the graph,
  // so go ahead and delete them here.
  for (GraphItem* elm : graph_items_) {
    delete elm;
  }
}

NodeHTML* PageGraph::GetHTMLNode(const DOMNodeId node_id) const {
  if (html_nodes_.find(node_id) == html_nodes_.end()) {
    return nullptr;
  }
  return html_nodes_.at(node_id);
}

void PageGraph::SetParentOfHTMLNode(const DOMNodeId parent_node_id,
    const DOMNodeId child_node_id) {
}

void PageGraph::RegisterHTMLNodeInserted(const string& tag_name) {
  std::cout << "RegisterHTMLNodeInserted: " << tag_name << "\n";
}

}  // namespace brave_page_graph
