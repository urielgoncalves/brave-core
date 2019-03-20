/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_PAGE_GRAPH_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_PAGE_GRAPH_H_

#include <map>
#include <memory>
#include <string>
#include <vector>
// #include "third_party/blink/renderer/core/dom/element.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

namespace brave_page_graph {

class Edge;
class GraphItem;
class Node;
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
    const std::string& tag_name);
  void RegisterHTMLTextNodeCreated(const DOMNodeId node_id,
    const std::string& text);
  void RegisterHTMLElementNodeInserted(const DOMNodeId node_id,
    const std::string& tag_name, const DOMNodeId parent_node_id,
    const DOMNodeId before_sibling_id);
  void RegisterHTMLTextNodeInserted(const DOMNodeId node_id,
    const DOMNodeId parent_node_id, const DOMNodeId before_sibling_id);
  void RegisterHTMLElementNodeRemoved(const DOMNodeId node_id);
  void RegisterHTMLTextNodeRemoved(const DOMNodeId node_id);

  void RegisterAttributeSet(const DOMNodeId node_id,
    const std::string& attr_name, const std::string& attr_value);
  void RegisterAttributeDelete(const DOMNodeId node_id,
    const std::string& attr_name);

  GraphMLXML ToGraphML() const;
  NodeActor* GetCurrentActingNode() const;

  const std::vector<std::unique_ptr<Node> >& Nodes() const;
  const std::vector<std::unique_ptr<const Edge> >& Edges() const;
  const std::vector<const GraphItem*>& GraphItems() const;

 protected:
  void AddNode(Node* const node);
  void AddEdge(const Edge* const edge);

  // Monotonically increasing counter, used so that we can replay the
  // the graph's construction if needed.
  PageGraphId id_counter_;

  // These vectors owns the all the items that are shared and indexed across
  // the rest of the graph.  All the other pointers (the weak pointers)
  // do not own their data.
  std::vector<std::unique_ptr<Node> > nodes_;
  std::vector<std::unique_ptr<const Edge> > edges_;
  
  // Vectors for tracking other ways of referencing graph elements, non-owning.
  std::vector<const GraphItem*> graph_items_;

  // Non-owning references to singleton items in the graph. (the owning
  // references will be in the above vectors).
  NodeParser* const parser_node_;
  NodeShields* const shields_node_;
  NodeStorageCookieJar* const cookie_jar_node_;
  NodeStorageLocalStorage* const local_storage_node_;

  // Non-owning reference to the HTML root of the document (i.e. <html>).
  NodeHTML* html_root_node_;

  // Index structure for storing and looking up webapi nodes.
  // This map does not own the references.
  std::map<MethodName, NodeWebAPI* const> webapi_nodes_;

  // Index structure for looking up HTML nodes.
  // This map does not own the references.
  std::map<DOMNodeId, NodeHTMLElement* const> element_nodes_;
  std::map<DOMNodeId, NodeHTMLText* const> text_nodes_;

  // Index structure for looking up script nodes.
  // This map does not own the references.
  std::map<ScriptId, NodeScript* const> script_nodes_;

  // Index (and reverse index) to keep track of requests that are still
  // "in the air".  Once we know if a request succeeded, failed, or was
  // blocked by shields, its removed from here.  Note that this
  // assumes only one request per URL will be made at a time.  If this turns
  // out to be frequently wrong, this will need to be redone.
  // std::map<const RequestUrl, const blink::Element*> url_to_elm_map_;
  // std::map<const blink::Element*, const RequestUrl> elm_to_url_map;
};

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_PAGE_GRAPH_H_
