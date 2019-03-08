/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_PAGE_GRAPH_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_PAGE_GRAPH_H_

#include <map>
#include <string>
#include <vector>
#include "brave/components/brave_page_graph/graph_item/edge.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/graph_item/node/node_storage_cookiejar.h"
#include "brave/components/brave_page_graph/graph_item/node/node_storage_localstorage.h"
#include "brave/components/brave_page_graph/graph_item/node/node_html.h"
#include "brave/components/brave_page_graph/graph_item/node/node_parser.h"
#include "brave/components/brave_page_graph/graph_item/node/node_script.h"
#include "brave/components/brave_page_graph/graph_item/node/node_shields.h"
#include "brave/components/brave_page_graph/graph_item/node/node_webapi.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::map;
using ::std::string;
using ::std::vector;

namespace brave_page_graph {

class PageGraph {
 public:
  PageGraph();
  ~PageGraph();
  NodeHTML* GetHTMLNode(const DOMNodeId node_id) const;
  void SetParentOfHTMLNode(const DOMNodeId parent_node_id,
    const DOMNodeId child_node_id);
  void RegisterHTMLNodeInserted(const string& tag_name);

 protected:
  // Monotonically increasing counter, used so that we can replay the
  // the graph's construction if needed.
  PageGraphId graph_id_counter_;

  // This vector owns the all the items that are shared and indexed across
  // the rest of the graph.
  vector<GraphItem*> graph_items_;

  // Non-owning references to singleton items in the graph. (the owning
  // references will be in the above vectors).
  NodeParser* node_parser_;
  NodeShields* node_shields_;
  NodeStorageCookieJar* node_cookie_jar_;
  NodeStorageLocalStorage* node_local_storage_;

  // Index structure for storing and looking up webapi nodes.
  // This map does not own the references.
  map<MethodName, NodeWebAPI*> webapi_nodes_;

  // Index structure for looking up HTML nodes.
  // This map does not own the references.
  map<DOMNodeId, NodeHTML*> html_nodes_;

  // Index structure for looking up script nodes.
  // This map does not own the references.
  map<ScriptId, NodeScript*> script_nodes_;
};

}

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_PAGE_GRAPH_H_
