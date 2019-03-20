/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/types.h"
#include <string>

using ::std::string;

namespace brave_page_graph {

string graphml_type_to_string(const GraphMLAttrType type) noexcept {
  switch (type) {
    case kGraphMLAttrTypeString:
      return "string";
    case kGraphMLAttrTypeBoolean:
      return "boolean";
    case kGraphMLAttrTypeInt:
      return "int";
    case kGraphMLAttrTypeLong:
      return "long";
    case kGraphMLAttrTypeFloat:
      return "float";
    case kGraphMLAttrTypeDouble:
      return "double";
    case kGraphMLAttrTypeUnknown:
    default:
      return "unknown";
  }
}

string graphml_for_to_string(const GraphMLAttrForType type) noexcept {
  switch (type) {
    case kGraphMLAttrForTypeNode:
      return "node";
    case kGraphMLAttrForTypeEdge:
      return "edge";
    case kGraphMLAttrForTypeUnknown:
    default:
      return "unknown";
  }
}

string request_type_to_string(const RequestType type) noexcept {
  switch (type) {
    case kRequestTypeImage:
      return "Image";
    case kRequestTypeScriptClassic:
      return "ScriptClassic";
    case kRequestTypeScriptModule:
      return "ScriptModule";
    case kRequestTypeCSS:
      return "CSS";
    case kRequestTypeVideo:
      return "Video";
    case kRequestTypeAudio:
      return "Audio";
    case kRequestTypeSVG:
      return "SVG";
    case kRequestTypeFont:
      return "Font";
    case kRequestTypeDocument:
      return "Document";
    case kRequestTypeUnknown:
    default:
      return "Unknown";
  }
}

string request_type_to_string(const ResourceType type) noexcept {
  switch (type) {
    case kResourceTypeAudio:
      return "audio";
    case kResourceTypeCSS:
      return "cSS";
    case kResourceTypeDocument:
      return "document";
    case kResourceTypeFont:
      return "font";
    case kResourceTypeImage:
      return "image";
    case kResourceTypeVideo:
      return "video";
    case kResourceTypeScript:
      return "script";
    case kResourceTypeSVG:
      return "sVG";
    case kResourceTypeUnknown:
      return "unknown";
  }
}

string script_type_to_string(const ScriptType type) noexcept {
  switch (type) {
    case kScriptTypeClassic:
      return "Classic";
    case kScriptTypeModule:
      return "Module";
    case kScriptTypeExtension:
      return "Extension";
    case kScriptTypeUnknown:
    default:
      return "Unknown";
  }
}

}  // namespace brave_page_graph