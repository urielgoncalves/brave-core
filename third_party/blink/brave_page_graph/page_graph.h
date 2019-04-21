/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_PAGE_GRAPH_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_PAGE_GRAPH_H_

#include <atomic>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "brave/third_party/blink/brave_page_graph/script_tracker.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

namespace brave_page_graph {

class Edge;
class EdgeRequestStart;
class GraphItem;
class Node;
class NodeActor;
class NodeHTML;
class NodeHTMLElement;
class NodeHTMLText;
class NodeParser;
class NodeResource;
class NodeScript;
class NodeShields;
class NodeStorageCookieJar;
class NodeStorageLocalStorage;
class NodeWebAPI;

class PageGraph {
friend GraphItem;
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
    const DOMNodeId parent_node_id, const DOMNodeId before_sibling_id);
  void RegisterHTMLTextNodeInserted(const DOMNodeId node_id,
    const DOMNodeId parent_node_id, const DOMNodeId before_sibling_id);
  void RegisterHTMLElementNodeRemoved(const DOMNodeId node_id);
  void RegisterHTMLTextNodeRemoved(const DOMNodeId node_id);

  void RegisterInlineStyleSet(const DOMNodeId node_id,
    const std::string& attr_name, const std::string& attr_value);
  void RegisterInlineStyleDelete(const DOMNodeId node_id,
    const std::string& attr_name);
  void RegisterAttributeSet(const DOMNodeId node_id,
    const std::string& attr_name, const std::string& attr_value);
  void RegisterAttributeDelete(const DOMNodeId node_id,
    const std::string& attr_name);

  void RegisterRequestStartFromElm(const DOMNodeId node_id,
    const NetworkRequestId request_id, const RequestUrl url,
    const RequestType type);
  void RegisterRequestStartFromCurrentScript(const RequestUrl url,
    const RequestType type);

  void RegisterRequestComplete(const NetworkRequestId request_id,
    const ResourceType type);
  void RegisterRequestError(const NetworkRequestId request_id);

  // Methods for handling the registration of script units in the document,
  // and v8 script executing.

  // Local scripts are scripts that define their code inline.
  void RegisterLocalScript(const DOMNodeId node_id,
    const SourceCodeHash code_hash);
  // Remote scripts are scripts that reference remote code (eg src=...).
  void RegisterRemoteScript(const DOMNodeId node_id, const UrlHash url_hash);

  void RegisterLocalScriptCompilation(const SourceCodeHash code_hash,
    const ScriptId script_id);
  void RegisterRemoteScriptCompilation(const UrlHash url_hash,
    const SourceCodeHash code_hash, const ScriptId script_id);

  void RegisterScriptExecStart(const ScriptId script_id);
  // The Script ID is only used here as a sanity check to make sure we're
  // correctly tracking script execution correctly.
  void RegisterScriptExecStop(const ScriptId script_id);

  GraphMLXML ToGraphML() const;
  NodeActor* GetCurrentActingNode() const;

  const std::vector<std::unique_ptr<Node> >& Nodes() const;
  const std::vector<std::unique_ptr<const Edge> >& Edges() const;
  const std::vector<const GraphItem*>& GraphItems() const;

  NetworkRequestId GetNewRequestId();
  ChildFrameId GetNewChildFrameId();

 protected:
  void AddNode(Node* const node);
  void AddEdge(const Edge* const edge);

  std::vector<DOMNodeId> NodeIdsForScriptId(const ScriptId script_id) const;
  std::vector<ScriptId> ScriptIdsForNodeId(const DOMNodeId nodeId) const;

  void PushActiveScript(const ScriptId script_id);
  ScriptId PopActiveScript();
  ScriptId PeekActiveScript() const;

  // Monotonically increasing counter, used so that we can replay the
  // the graph's construction if needed.
  PageGraphId id_counter_ = 0;

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
  NodeHTMLElement* html_root_node_;

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

  // Request handling
  // ---
  std::atomic<NetworkRequestId> current_max_request_id_;
  std::atomic<ChildFrameId> current_max_child_frame_id_;
  // Tracks requests that have started, but have not completed yet.
  std::map<NetworkRequestId, EdgeRequestStart* const> current_requests_;
  // Makes sure we don't have more than one node in the graph representing
  // a single URL (not required for correctness, but keeps things tidier
  // and makes some kinds of queries nicer).
  std::map<RequestUrl, NodeResource* const> resource_nodes_;

  // Keeps track of which scripts are running, and conceptually mirrors the
  // JS stack.
  std::vector<ScriptId> active_script_stack_;

  // Data structure used for mapping HTML script elements (and other
  // sources of script in a document) to v8 script units.
  ScriptTracker script_tracker_;
};

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_PAGE_GRAPH_H_
