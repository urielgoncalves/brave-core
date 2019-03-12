/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_PAGE_GRAPH_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_PAGE_GRAPH_H_

#include <map>
#include <string>
#include <vector>
#include "brave/components/brave_page_graph/types.h"

using ::std::map;
using ::std::string;
using ::std::vector;

namespace brave_page_graph {

class GraphItem;
class NodeActor;
class NodeHTML;
class NodeHTMLElement;
class NodeHTMLText;
class NodeParser;
class NodeScript;
class NodeShields;
class NodeStorageCookieJar;
class NodeStorageLocalStorage;
class NodeWebAPI;

class PageGraph {
 public:
  PageGraph();
  ~PageGraph();

  NodeHTML* GetHTMLNode(const DOMNodeId node_id) const;
  NodeHTMLElement* GetHTMLElementNode(const DOMNodeId node_id) const;
  NodeHTMLText* GetHTMLTextNode(const DOMNodeId node_id) const;

  void RegisterHTMLElementNodeCreated(const DOMNodeId node_id,
    const string& tag_name);
  void RegisterHTMLTextNodeCreated(const DOMNodeId node_id,
    const string& text);
  void RegisterHTMLElementNodeInserted(const DOMNodeId node_id,
    const DOMNodeId parent_node_id, const DOMNodeId before_sibling_id);
  void RegisterHTMLElementNodeRemoved(const DOMNodeId node_id);

  void RegisterAttributeSet(const DOMNodeId node_id, const string& attr_name,
    const string& attr_value);
  void RegisterAttributeDelete(const DOMNodeId node_id,
    const string& attr_name);

  NodeActor* GetCurrentActingNode() const;

 protected:
  // Monotonically increasing counter, used so that we can replay the
  // the graph's construction if needed.
  PageGraphId graph_id_counter_;

  // This vector owns the all the items that are shared and indexed across
  // the rest of the graph.
  vector<GraphItem*> graph_items_;

  // Non-owning references to singleton items in the graph. (the owning
  // references will be in the above vectors).
  NodeParser* parser_node_;
  NodeShields* shields_node_;
  NodeStorageCookieJar* cookie_jar_node_;
  NodeStorageLocalStorage* local_storage_node_;

  // Non-owning reference to the HTML root of the document (i.e. <html>).
  NodeHTML* html_root_node_;

  // Index structure for storing and looking up webapi nodes.
  // This map does not own the references.
  map<MethodName, NodeWebAPI*> webapi_nodes_;

  // Index structure for looking up HTML nodes.
  // This map does not own the references.
  map<DOMNodeId, NodeHTMLElement*> html_element_nodes_;
  map<DOMNodeId, NodeHTMLText*> html_text_nodes_;

  // Index structure for looking up script nodes.
  // This map does not own the references.
  map<ScriptId, NodeScript*> script_nodes_;
};

}

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_PAGE_GRAPH_H_
