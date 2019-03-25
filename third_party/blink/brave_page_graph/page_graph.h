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
#include "brave/third_party/blink/brave_page_graph/types.h"

namespace brave_page_graph {

class Edge;
class GraphItem;
class InAirRequest;
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
    const RequestUrl url, const RequestType type);
  void RegisterRequestStartFromCurrentScript(const RequestUrl url,
    const RequestType type);

  // Local scripts are scripts that define their code inline.
  void RegisterLocalScript(const DOMNodeId node_id, const SourceCodeHash hash);
  // Remote scripts are scripts that reference remote code (eg src=...).
  void RegisterRemoteScript(const DOMNodeId node_id, const UrlHash hash);

  void RegisterLocalScriptCompilation(const SourceCodeHash hash,
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

  // Map to keep track of requests that are still "in the air".
  // Once we know if a request succeeded, failed, or was
  // blocked by shields, its removed from here.  Note that this
  // assumes only one request per URL will be made at a time.  If this turns
  // out to be frequently wrong, this will need to be redone.
  std::map<RequestUrl, const std::unique_ptr<InAirRequest> > in_air_requests_;

  // Keeps track of which scripts are running, and conceptually mirrors the
  // JS stack.
  std::vector<ScriptId> active_script_stack_;

  // Tracking <script> elements to compiled V8 code units.
  // ---
  // The following eight maps are all used for keeping track of which
  // script unit belongs to which page element.  Because different pieces
  // of information are available at different points in time (and processed
  // away at other points), we need this semi-convoluted system.  At
  // a high level, we do the following (indexed and reverse indexed):
  //   1) Associate the <script> element with the JS code unit (either
  //      by the code itself, for inline script, or the URL of the code
  //      for remote code).
  //   2) For remote fetched code, associate the fetched / compiled JS
  //      with the URL it came from.
  //   3) Associate the compiled JS code with the V8 assigned "script id"
  //      (the identifer v8 uses internally for referring to each script within
  //      a context).
  // 
  // Maps used for step 1.  Maps are from node id to multiple script hashes
  // because its possible that the text of a script tag could change over time,
  // (e.g. changing the src attr of the script element to point to a new
  // URL, changing the innerText of the <script> element).
  std::map<DOMNodeId, std::vector<UrlHash>> node_id_to_script_url_hashes_;
  std::map<UrlHash, std::vector<DOMNodeId>> script_src_hash_to_node_ids_;
  std::map<DOMNodeId, std::vector<SourceCodeHash>> node_id_to_source_hashes_;
  std::map<SourceCodeHash, std::vector<DOMNodeId>> source_hash_to_node_ids_;

  //  Maps used for step 2.
  std::map<UrlHash, SourceCodeHash> script_url_hash_to_source_hash_;
  std::map<SourceCodeHash, UrlHash> source_hash_to_script_url_hash_;

  //  Maps used for step 3.
  std::map<SourceCodeHash, ScriptId> source_hash_to_script_id_;
  std::map<ScriptId, SourceCodeHash> script_id_to_source_hash_;
};

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_PAGE_GRAPH_H_
