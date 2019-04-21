/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include <signal.h>
#include <climits>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include "base/logging.h"
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_attribute_set.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_attribute_delete.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_node_create.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_node_delete.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_node_insert.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_node_remove.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/request/edge_request.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/request/edge_request_start.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/request/edge_request_error.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/request/edge_request_complete.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_actor.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html_element.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html_text.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_parser.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_resource.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_script.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_shields.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_storage_cookiejar.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_storage_localstorage.h"
#include "brave/third_party/blink/brave_page_graph/script_tracker.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::endl;
using ::std::make_unique;
using ::std::map;
using ::std::move;
using ::std::string;
using ::std::to_string;
using ::std::unique_ptr;
using ::std::vector;

namespace brave_page_graph {

namespace {
  PageGraph* yuck = nullptr;
}

void log(const string& msg) {
  LOG(INFO) << "*PageGraph* " << msg;
}

void write_to_disk(int signal) {
  std::cout << "GOT THAT SIG" << endl;
  std::ofstream outfile("/tmp/pagegraph.log");
  string output = yuck->ToGraphML();
  std::cout << output;
  outfile.write(output.c_str(), output.size());
  outfile.close();
}

PageGraph::PageGraph() :
    parser_node_(new NodeParser(this)),
    shields_node_(new NodeShields(this)),
    cookie_jar_node_(new NodeStorageCookieJar(this)),
    local_storage_node_(new NodeStorageLocalStorage(this)),
    html_root_node_(new NodeHTMLElement(this, kRootNodeId, "(root)")) {
  log("init");
  AddNode(parser_node_);
  AddNode(shields_node_);
  AddNode(cookie_jar_node_);
  AddNode(local_storage_node_);
  AddNode(html_root_node_);
  element_nodes_.emplace(kRootNodeId, html_root_node_);
  yuck = this;
  signal(30, &write_to_disk);
}

PageGraph::~PageGraph() {}

NodeHTML* PageGraph::GetHTMLNode(const DOMNodeId node_id) const {
  LOG_ASSERT(element_nodes_.count(node_id) + text_nodes_.count(node_id) == 1);
  if (element_nodes_.count(node_id) == 1) {
    return element_nodes_.at(node_id);
  }
  return text_nodes_.at(node_id);
}

NodeHTMLElement* PageGraph::GetHTMLElementNode(const DOMNodeId node_id) const {
  log("GetHTMLElementNode: " + to_string(node_id));
  if (node_id == kRootNodeId) {
    return html_root_node_;
  }
  LOG_ASSERT(element_nodes_.count(node_id) == 1);
  return element_nodes_.at(node_id);
}

NodeHTMLText* PageGraph::GetHTMLTextNode(const DOMNodeId node_id) const {
  LOG_ASSERT(text_nodes_.count(node_id) == 1);
  return text_nodes_.at(node_id);
}

void PageGraph::RegisterHTMLElementNodeCreated(const DOMNodeId node_id,
    const string& tag_name) {
  log("RegisterHTMLElementNodeCreated: " + to_string(node_id) + " (" + tag_name + ")");
  LOG_ASSERT(element_nodes_.count(node_id) == 0);
  NodeHTMLElement* const new_node = new NodeHTMLElement(this,
    node_id, tag_name);

  AddNode(new_node);
  element_nodes_.emplace(node_id, new_node);

  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeNodeCreate* const edge = new EdgeNodeCreate(this,
    acting_node, new_node);
  AddEdge(edge);

  new_node->AddInEdge(edge);
  acting_node->AddOutEdge(edge);
}

void PageGraph::RegisterHTMLTextNodeCreated(const DOMNodeId node_id,
    const string& text) {
  log("RegisterHTMLTextNodeCreated: " + to_string(node_id) + ", " + text);
  LOG_ASSERT(text_nodes_.count(node_id) == 0);
  NodeHTMLText* const new_node = new NodeHTMLText(this,
    node_id, text);
  AddNode(new_node);
  text_nodes_.emplace(node_id, new_node);

  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeNodeCreate* const edge = new EdgeNodeCreate(this,
    acting_node, new_node);
  AddEdge(edge);

  new_node->AddInEdge(edge);
  acting_node->AddOutEdge(edge);
}

void PageGraph::RegisterHTMLElementNodeInserted(const DOMNodeId node_id,
    const DOMNodeId parent_node_id, const DOMNodeId before_sibling_id) {

  const DOMNodeId inserted_parent_node_id = (parent_node_id) 
    ? parent_node_id
    : kRootNodeId;

  log("RegisterHTMLElementNodeInserted: " + to_string(node_id) + ", "
    + to_string(inserted_parent_node_id) + ", " + to_string(before_sibling_id));

  LOG_ASSERT(element_nodes_.count(node_id) == 1);
  NodeHTMLElement* const inserted_node = element_nodes_.at(node_id);

  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeNodeInsert* const edge = new EdgeNodeInsert(this,
    acting_node, inserted_node, inserted_parent_node_id, before_sibling_id);
  AddEdge(edge);

  inserted_node->AddInEdge(edge);
  acting_node->AddOutEdge(edge);
}

void PageGraph::RegisterHTMLTextNodeInserted(const DOMNodeId node_id,
    const DOMNodeId parent_node_id, const DOMNodeId before_sibling_id) {

  const DOMNodeId inserted_parent_node_id = (parent_node_id) 
    ? parent_node_id
    : kRootNodeId;

  log("RegisterHTMLTextNodeInserted: (" + to_string(node_id) + "), " +
    to_string(inserted_parent_node_id) + ", " + to_string(before_sibling_id));
  LOG_ASSERT(text_nodes_.count(node_id) == 1);
  NodeHTMLText* const inserted_node = text_nodes_.at(node_id);

  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeNodeInsert* const edge = new EdgeNodeInsert(this,
    acting_node, inserted_node, inserted_parent_node_id, before_sibling_id);
  AddEdge(edge);

  inserted_node->AddInEdge(edge);
  acting_node->AddOutEdge(edge);
}

void PageGraph::RegisterHTMLElementNodeRemoved(const DOMNodeId node_id) {
  log("RegisterHTMLElementNodeRemoved: " + to_string(node_id));
  LOG_ASSERT(element_nodes_.count(node_id) == 1);
  NodeHTMLElement* const removed_node = element_nodes_.at(node_id);

  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeNodeRemove* const edge = new EdgeNodeRemove(this,
    static_cast<NodeScript*>(acting_node), removed_node);
  AddEdge(edge);

  acting_node->AddOutEdge(edge);
  removed_node->AddInEdge(edge);
}

void PageGraph::RegisterHTMLTextNodeRemoved(const DOMNodeId node_id) {
  log("RegisterHTMLTextNodeRemoved: " + to_string(node_id));
  LOG_ASSERT(text_nodes_.count(node_id) == 1);
  NodeHTMLText* const removed_node = text_nodes_.at(node_id);

  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeNodeRemove* const edge = new EdgeNodeRemove(this,
    static_cast<NodeScript*>(acting_node), removed_node);
  AddEdge(edge);

  acting_node->AddOutEdge(edge);
  removed_node->AddInEdge(edge);
}

void PageGraph::RegisterInlineStyleSet(const DOMNodeId node_id,
    const std::string& attr_name, const std::string& attr_value) {
  log("RegisterInlineStyleSet: " + to_string(node_id) + ", "
    + attr_name + "=" + attr_value);
  LOG_ASSERT(element_nodes_.count(node_id) == 1);

  NodeHTMLElement* const target_node = element_nodes_.at(node_id);
  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeAttributeSet* const edge = new EdgeAttributeSet(this,
    acting_node, target_node, attr_name, attr_value, true);
  AddEdge(edge);

  acting_node->AddOutEdge(edge);
  target_node->AddInEdge(edge);
}

void PageGraph::RegisterInlineStyleDelete(const DOMNodeId node_id,
    const std::string& attr_name) {
  log("RegisterInlineStyleDelete: " + to_string(node_id) + ", "
    + attr_name + "=");
  LOG_ASSERT(element_nodes_.count(node_id) == 1);

  NodeHTMLElement* const target_node = element_nodes_.at(node_id);
  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeAttributeDelete* const edge = new EdgeAttributeDelete(this,
    acting_node, target_node, attr_name, true);
  AddEdge(edge);

  acting_node->AddOutEdge(edge);
  target_node->AddInEdge(edge);
}

void PageGraph::RegisterAttributeSet(const DOMNodeId node_id,
    const string& attr_name, const string& attr_value) {
  log("RegisterAttributeSet: " + to_string(node_id) + ", "
    + attr_name + "=" + attr_value);
  LOG_ASSERT(element_nodes_.count(node_id) == 1);

  NodeHTMLElement* const target_node = element_nodes_.at(node_id);
  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeAttributeSet* const edge = new EdgeAttributeSet(this,
    acting_node, target_node, attr_name, attr_value);
  AddEdge(edge);

  acting_node->AddOutEdge(edge);
  target_node->AddInEdge(edge);
}

void PageGraph::RegisterAttributeDelete(const DOMNodeId node_id,
    const string& attr_name) {
  log("RegisterAttributeDelete: " + to_string(node_id) + ", "
    + attr_name + "=");
  LOG_ASSERT(element_nodes_.count(node_id) == 1);

  NodeHTMLElement* const target_node = element_nodes_.at(node_id);

  NodeActor* const acting_node = GetCurrentActingNode();
  const EdgeAttributeDelete* const edge = new EdgeAttributeDelete(this,
    acting_node, target_node, attr_name);
  AddEdge(edge);

  acting_node->AddOutEdge(edge);
  target_node->AddInEdge(edge);
}

void PageGraph::RegisterRequestStartFromElm(const DOMNodeId node_id,
    const NetworkRequestId request_id, const RequestUrl url,
    const RequestType type) {
  // For now, explode if we're getting duplicate requests for the same
  // URL in the same document.  This might need to be changed.
  log("RegisterRequestStartFromElm: " + to_string(node_id)
    + ", request id: " + to_string(request_id) +
    + ", url:" + url
    + ", type: " + to_string(type));

  // We should know about the node thats issuing the request.
  LOG_ASSERT(element_nodes_.count(node_id) == 1);
  // We should also not have seen a request with this id before.
  LOG_ASSERT(current_requests_.count(request_id) == 0);

  NodeHTMLElement* const requesting_node = element_nodes_.at(node_id);
  NodeResource* requested_node;
  if (resource_nodes_.count(url) == 0) {
    log("RegisterRequestStartFromElm: First time seeing request for " + url);
    requested_node = new NodeResource(this, url);
    AddNode(requested_node);
    resource_nodes_.emplace(url, requested_node);
  } else {
    requested_node = resource_nodes_.at(url);
  }

  const EdgeRequest* const edge = new EdgeRequestStart(this, requesting_node,
    requested_node, request_id, type);
  current_requests_.emplace(request_id, edge);
  AddEdge(edge);

  requesting_node->AddOutEdge(edge);
  requested_node->AddInEdge(edge);
}

void PageGraph::RegisterRequestComplete(const NetworkRequestId request_id,
    const ResourceType type) {
  log("RegisterRequestComplete: " + to_string(request_id) +
    ", successful: " + resource_type_to_string(type));
  // There should be an outstanding request that is being closed here,
  // otherwise, there is a request we didn't correctly register being sent.
  LOG_ASSERT(current_requests_.count(request_id) == 1);

  EdgeRequestStart* const request_edge = current_requests_.at(request_id);
  NodeResource* const resource_node = request_edge->GetResourceNode();
  Node* const requesting_node = request_edge->GetRequestingNode();

  const EdgeRequestComplete* const request_edge = new EdgeRequestComplete(
    this, resource_node, requesting_node, request_id, type);
  current_requests_.erase(request_id);
  AddEdge(request_edge);

  resource_node->AddOutEdge(request_edge);
  requesting_node->AddInEdge(request_edge);
}

void PageGraph::RegisterRequestError(const NetworkRequestId request_id) {
  log("RegisterRequestError: " + to_string(request_id));
  // There should be an outstanding request that is being closed here,
  // otherwise, there is a request we didn't correctly register being sent.
  LOG_ASSERT(current_requests_.count(request_id) == 1);

  EdgeRequestStart* const request_edge = current_requests_.at(request_id);
  NodeResource* const resource_node = request_edge->GetResourceNode();
  Node* const requesting_node = request_edge->GetRequestingNode();

  const EdgeRequestError* const request_edge = new EdgeRequestError(
    this, resource_node, requesting_node, request_id);
  current_requests_.erase(request_id);
  AddEdge(request_edge);

  resource_node->AddOutEdge(request_edge);
  requesting_node->AddInEdge(request_edge);
}


void PageGraph::RegisterRequestStartFromCurrentScript(const RequestUrl url,
    const RequestType type) {
  // This isn't implemented yet...
  LOG_ASSERT(false);
}

void PageGraph::RegisterLocalScript(const DOMNodeId node_id,
    const SourceCodeHash code_hash) {
  script_tracker_.AddScriptSourceForElm(node_id, code_hash);
}

void PageGraph::RegisterRemoteScript(const DOMNodeId node_id,
    const UrlHash url_hash) {
  script_tracker_.AddScriptUrlForElm(node_id, url_hash);
}

void PageGraph::RegisterLocalScriptCompilation(const SourceCodeHash code_hash,
    const ScriptId script_id) {
  script_tracker_.SetScriptIdForCompliedCode(script_id, code_hash);
}

void PageGraph::RegisterRemoteScriptCompilation(const UrlHash url_hash,
    const SourceCodeHash code_hash, const ScriptId script_id) {
  script_tracker_.AddHashOfFetchedSourceFromUrl(code_hash, url_hash);
  script_tracker_.SetScriptIdForCompliedCode(script_id, code_hash);
}

void PageGraph::RegisterScriptExecStart(const ScriptId script_id) {
  PushActiveScript(script_id);
}

void PageGraph::RegisterScriptExecStop(const ScriptId script_id) {
  ScriptId popped_script_id = PopActiveScript();
  LOG_ASSERT(popped_script_id == script_id);
}

GraphMLXML PageGraph::ToGraphML() const {
  return graphml_for_page_graph(this);
}

NodeActor* PageGraph::GetCurrentActingNode() const {
  ScriptId current_script_id = PeekActiveScript();
  if (current_script_id == 0) {
    return parser_node_;
  }
  return script_nodes_.at(current_script_id);
}

const vector<unique_ptr<Node> >& PageGraph::Nodes() const {
  return nodes_;
}

const vector<unique_ptr<const Edge> >& PageGraph::Edges() const {
  return edges_;
}

const vector<const GraphItem*>& PageGraph::GraphItems() const {
  return graph_items_;
}

NetworkRequestId PageGraph::GetNewRequestId() {
  return ++current_max_request_id_;
}

ChildFrameId PageGraph::GetNewChildFrameId() {
  return ++current_max_child_frame_id_;
}

void PageGraph::AddNode(Node* const node) {
  nodes_.push_back(unique_ptr<Node>(node));
  graph_items_.push_back(node);
}

void PageGraph::AddEdge(const Edge* const edge) {
  edges_.push_back(unique_ptr<const Edge>(edge));
  graph_items_.push_back(edge);
}

vector<DOMNodeId> PageGraph::NodeIdsForScriptId(const ScriptId script_id) const {
  script_tracker_.GetElmsForScriptId(script_id);
}

vector<ScriptId> PageGraph::ScriptIdsForNodeId(const DOMNodeId node_id) const {
  script_tracker_.GetScriptIdsForElm(node_id);
}

void PageGraph::PushActiveScript(const ScriptId script_id) {
  active_script_stack_.push_back(script_id);
}

ScriptId PageGraph::PopActiveScript() {
  ScriptId top_script_id = 0;

  if (active_script_stack_.empty() == false) {
    top_script_id = active_script_stack_.back();
    active_script_stack_.pop_back();
  }
  
  return top_script_id;
}

ScriptId PageGraph::PeekActiveScript() const {
  if (active_script_stack_.empty() == true) {
    return 0;
  }
  return active_script_stack_.back();
}

}  // namespace brave_page_graph
