/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/requests/in_air_request.h"
#include "base/logging.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

namespace brave_page_graph {

InAirRequest::InAirRequest(const RequestUrl url, const RequestType type,
    NodeScript* const script_node) :
      url_(url),
      type_(type),
      script_node_(script_node),
      elm_node_(nullptr) {}

InAirRequest::InAirRequest(const RequestUrl url, const RequestType type,
    NodeHTMLElement* const html_elm_node) : 
      url_(url),
      type_(type),
      script_node_(nullptr),
      elm_node_(html_elm_node) {}

InAirRequest::~InAirRequest() {}

RequestUrl InAirRequest::GetRequestUrl() const {
  return url_;
}

RequestType InAirRequest::GetRequestType() const {
  return type_;
}

bool InAirRequest::IsScriptInitiated() const {
  return script_node_ != nullptr;
}

NodeScript* InAirRequest::GetScriptNode() const {
  LOG_ASSERT(IsScriptInitiated());
  return script_node_;
}

bool InAirRequest::IsElementInitiated() const {
  return elm_node_ != nullptr;
}

NodeHTMLElement* InAirRequest::GetElementNode() const {
  LOG_ASSERT(IsElementInitiated());
  return elm_node_;
}

}  // namespace brave_page_graph
