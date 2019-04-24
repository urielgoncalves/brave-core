/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/script_tracker.h"
#include <map>
#include <vector>
#include "base/logging.h"
#include "brave/third_party/blink/brave_page_graph/types.h"
#include "third_party/blink/renderer/bindings/core/v8/script_source_code.h"
#include "third_party/blink/renderer/core/dom/dom_node_ids.h"
#include "third_party/blink/renderer/platform/weborigin/kurl.h"

using ::blink::DOMNodeId;
using ::blink::KURL;
using ::blink::ScriptSourceCode;
using ::std::map;
using ::std::vector;

namespace brave_page_graph {

ScriptTracker::ScriptTracker() {}

ScriptTracker::~ScriptTracker() {}

void ScriptTracker::AddScriptUrlForElm(const KURL& url,
    const DOMNodeId node_id) {
  const UrlHash url_hash(url.GetString().Impl()->GetHash());
  if (node_id_to_script_url_hashes_.count(node_id) == 0) {
    node_id_to_script_url_hashes_.emplace(node_id, vector<UrlHash>());
  }
  node_id_to_script_url_hashes_.at(node_id).push_back(url_hash);

  if (script_src_hash_to_node_ids_.count(url_hash) == 0) {
    script_src_hash_to_node_ids_.emplace(url_hash, vector<DOMNodeId>());
  }
  script_src_hash_to_node_ids_.at(url_hash).push_back(node_id);
}

void ScriptTracker::AddScriptSourceForElm(const ScriptSourceCode& code,
    const DOMNodeId node_id) {
  const SourceCodeHash code_hash(code.Source().ToString().Impl()->GetHash());
  if (node_id_to_source_hashes_.count(node_id) == 0) {
    node_id_to_source_hashes_.emplace(node_id, vector<SourceCodeHash>());
  }
  node_id_to_source_hashes_.at(node_id).push_back(code_hash);

  if (source_hash_to_node_ids_.count(code_hash) == 0) {
    source_hash_to_node_ids_.emplace(code_hash, vector<DOMNodeId>());
  }
  source_hash_to_node_ids_.at(code_hash).push_back(node_id);
}

void ScriptTracker::AddCodeFetchedFromUrl(
    const ScriptSourceCode& code, const KURL& url) {
  const UrlHash url_hash(url.GetString().Impl()->GetHash());
  const SourceCodeHash code_hash(code.Source().ToString().Impl()->GetHash());

  // There should be no situations where we're receiving script code
  // from an unknown URL.
  LOG_ASSERT(script_src_hash_to_node_ids_.count(url_hash) > 0);

  script_url_hash_to_source_hash_.emplace(url_hash, code_hash);
  source_hash_to_script_url_hash_.emplace(code_hash, url_hash);
}

void ScriptTracker::SetScriptIdForCode(const ScriptId script_id,
    const ScriptSourceCode& code) {
  const SourceCodeHash code_hash(code.Source().ToString().Impl()->GetHash());
  // There should be no situtions where V8 has compiled source code that
  // we don't know about (TODO: handle cases of partial compilation,
  // eval, and similar).
  LOG_ASSERT(source_hash_to_script_url_hash_.count(code_hash) > 0 || 
    source_hash_to_node_ids_.count(code_hash) > 0);
  source_hash_to_script_id_.emplace(code_hash, script_id);
  script_id_to_source_hash_.emplace(script_id, code_hash);
}

vector<DOMNodeId> ScriptTracker::GetElmsForScriptId(
    const ScriptId script_id) const {
  vector<DOMNodeId> node_ids;

  // If we've never seen this code before, then we trivially can't know
  // what HTML nodes it belongs to, so return an empty vector.
  if (script_id_to_source_hash_.count(script_id) == 0) {
    return node_ids;
  }

  const SourceCodeHash source_hash = script_id_to_source_hash_.at(script_id);

  if (source_hash_to_node_ids_.count(source_hash) > 0) {
    for (const auto& a_node_id : source_hash_to_node_ids_.at(source_hash)) {
      node_ids.push_back(a_node_id);
    }
  }

  if (source_hash_to_script_url_hash_.count(source_hash) > 0) {
    const UrlHash url_hash = source_hash_to_script_url_hash_.at(source_hash);
    if (script_src_hash_to_node_ids_.count(url_hash) > 0) {
      for (const auto& a_node_id : script_src_hash_to_node_ids_.at(url_hash)) {
        node_ids.push_back(a_node_id);
      }
    }
  }

  return node_ids;
}

vector<ScriptId> ScriptTracker::GetScriptIdsForElm(
    const DOMNodeId node_id) const {
  vector<ScriptId> script_ids;

  const bool node_has_urls = node_id_to_script_url_hashes_.count(node_id) > 0;
  const bool node_has_sources = node_id_to_source_hashes_.count(node_id) > 0; 

  if (node_has_urls == false && node_has_sources == false) {
    return script_ids;
  }

  if (node_has_urls) {
    for (const UrlHash& url_hash : node_id_to_script_url_hashes_.at(node_id)) {
      if (script_url_hash_to_source_hash_.count(url_hash) > 0) {
        const SourceCodeHash code_hash = script_url_hash_to_source_hash_.at(url_hash);
        const ScriptId script_id_for_code_hash = source_hash_to_script_id_.at(code_hash);
        script_ids.push_back(script_id_for_code_hash);
      }
    }
  }

  if (node_has_sources) {
    for (const SourceCodeHash& code_hash : node_id_to_source_hashes_.at(node_id)) {
      const ScriptId script_id_for_code_hash = source_hash_to_script_id_.at(code_hash);
      script_ids.push_back(script_id_for_code_hash);
    }
  }

  return script_ids;
}

}  // namespace brave_page_graph
