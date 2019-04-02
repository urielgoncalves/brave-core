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
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_actor.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html_element.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html_text.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_parser.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_script.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_shields.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_storage_cookiejar.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_storage_localstorage.h"
#include "brave/third_party/blink/brave_page_graph/requests/in_air_request.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::endl;
using ::std::make_unique;
using ::std::map;
using ::std::string;
using ::std::to_string;
using ::std::unique_ptr;
using ::std::vector;

namespace brave_page_graph {

const DOMNodeId kRootNodeId = ULLONG_MAX;

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
    const RequestUrl url, const RequestType type) {
  // For now, explode if we're getting duplicate requests for the same
  // URL in the same document.  This might need to be changed.
  // LOG_ASSERT(in_air_requests_.count(url) == 0);
  LOG_ASSERT(element_nodes_.count(node_id) == 1);
  NodeHTMLElement* node = element_nodes_.at(node_id);
  in_air_requests_.emplace(url, make_unique<InAirRequest>(url, type, node));
}

void PageGraph::RegisterRequestStartFromCurrentScript(const RequestUrl url,
    const RequestType type) {
  NodeActor* const current_script = GetCurrentActingNode();
  LOG_ASSERT(current_script->IsScript());
  unique_ptr<InAirRequest> ptr = make_unique<InAirRequest>(url, type,
    static_cast<NodeScript*>(current_script));
  in_air_requests_.emplace(url, std::move(ptr));
}

void PageGraph::RegisterLocalScript(const DOMNodeId node_id,
    const SourceCodeHash hash) {
  if (node_id_to_source_hashes_.count(node_id) == 0) {
    node_id_to_source_hashes_.emplace(node_id, vector<SourceCodeHash>());
  }
  node_id_to_source_hashes_.at(node_id).push_back(hash);

  if (source_hash_to_node_ids_.count(hash) == 0) {
    source_hash_to_node_ids_.emplace(hash, vector<DOMNodeId>());
  }
  source_hash_to_node_ids_.at(hash).push_back(node_id);
}

void PageGraph::RegisterRemoteScript(const DOMNodeId node_id,
    const UrlHash hash) {
  if (node_id_to_script_url_hashes_.count(node_id) == 0) {
    node_id_to_script_url_hashes_.emplace(node_id, vector<UrlHash>());
  }
  node_id_to_script_url_hashes_.at(node_id).push_back(hash);

  if (script_src_hash_to_node_ids_.count(hash) == 0) {
    script_src_hash_to_node_ids_.emplace(hash, vector<DOMNodeId>());
  }
  script_src_hash_to_node_ids_.at(hash).push_back(node_id);
}

void PageGraph::RegisterLocalScriptCompilation(const SourceCodeHash hash,
    const ScriptId script_id) {
  source_hash_to_script_id_.emplace(hash, script_id);
  script_id_to_source_hash_.emplace(script_id, hash);
}

void PageGraph::RegisterRemoteScriptCompilation(const UrlHash url_hash,
    const SourceCodeHash code_hash, const ScriptId script_id) {
  script_url_hash_to_source_hash_.emplace(url_hash, code_hash);
  source_hash_to_script_url_hash_.emplace(code_hash, url_hash);

  source_hash_to_script_id_.emplace(code_hash, script_id);
  script_id_to_source_hash_.emplace(script_id, code_hash);
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

void PageGraph::AddNode(Node* const node) {
  nodes_.push_back(unique_ptr<Node>(node));
  graph_items_.push_back(node);
}

void PageGraph::AddEdge(const Edge* const edge) {
  edges_.push_back(unique_ptr<const Edge>(edge));
  graph_items_.push_back(edge);
}

vector<DOMNodeId> PageGraph::NodeIdsForScriptId(const ScriptId script_id) const {
  SourceCodeHash code_hash = script_id_to_source_hash_.at(script_id);
  vector<DOMNodeId> node_ids;

  if (source_hash_to_node_ids_.count(code_hash) == 1) {
    for (DOMNodeId const &node_id : source_hash_to_node_ids_.at(code_hash)) {
      node_ids.push_back(node_id);
    }
  }

  if (source_hash_to_script_url_hash_.count(code_hash) == 1) {
    UrlHash script_url_hash = source_hash_to_script_url_hash_.at(code_hash);

    if (script_src_hash_to_node_ids_.count(script_url_hash) == 1) {
      for (const DOMNodeId &node_id : script_src_hash_to_node_ids_.at(script_url_hash)) {
        node_ids.push_back(node_id);
      }
    }
  }

  return node_ids;
}

vector<ScriptId> PageGraph::ScriptIdsForNodeId(const DOMNodeId node_id) const {
  vector<ScriptId> script_ids;

  if (node_id_to_source_hashes_.count(node_id) == 1) {
    for (const ScriptId &a_script_id : node_id_to_source_hashes_.at(node_id)) {
      script_ids.push_back(a_script_id);
    }
  }

  if (node_id_to_script_url_hashes_.count(node_id) == 1) {
    for (const ScriptId &url_hash: node_id_to_script_url_hashes_.at(node_id)) {
      if (script_url_hash_to_source_hash_.count(url_hash) == 1) {
        ScriptId script_id_for_url = script_url_hash_to_source_hash_.at(url_hash);
        script_ids.push_back(script_id_for_url);
      }
    }
  }

  return script_ids;
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
