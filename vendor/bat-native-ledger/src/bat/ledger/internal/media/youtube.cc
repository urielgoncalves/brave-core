/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <cmath>
#include <vector>

#include "bat/ledger/internal/ledger_impl.h"
#include "bat/ledger/internal/media/helper.h"
#include "bat/ledger/internal/media/youtube.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace braveledger_media {

MediaYouTube::MediaYouTube(bat_ledger::LedgerImpl* ledger):
  ledger_(ledger) {
}

MediaYouTube::~MediaYouTube() {
}

// static
std::string MediaYouTube::GetMediaIdFromParts(
    const std::map<std::string, std::string>& parts) {
  std::string result;
  std::map<std::string, std::string>::const_iterator iter =
        parts.find("docid");
  if (iter != parts.end()) {
    result = iter->second;
  }

  return result;
}

// static
uint64_t MediaYouTube::GetMediaDurationFromParts(
    const std::map<std::string, std::string>& data,
    const std::string& media_key) {
  uint64_t duration = 0;
  std::map<std::string, std::string>::const_iterator iter_st = data.find("st");
  std::map<std::string, std::string>::const_iterator iter_et = data.find("et");
  if (iter_st != data.end() && iter_et != data.end()) {
    std::vector<std::string> start_time = braveledger_bat_helper::split(
        iter_st->second,
        ',');
    std::vector<std::string> end_time = braveledger_bat_helper::split(
        iter_et->second,
        ',');
    if (start_time.size() != end_time.size()) {
      return 0;
    }

    // get all the intervals and combine them.
    // (Should only be one set if there were no seeks)
    for (size_t i = 0; i < start_time.size(); i++) {
      std::stringstream tempET(end_time[i]);
      std::stringstream tempST(start_time[i]);
      double st = 0;
      double et = 0;
      tempET >> et;
      tempST >> st;

      // round instead of truncate
      // also make sure we include previous iterations
      // if more than one set exists
      duration += (uint64_t)std::round(et - st);
    }
  }

  return duration;
}

// static
std::string MediaYouTube::GetVideoUrl(const std::string& media_id) {
  std::string res;
  DCHECK(!media_id.empty());
  return "https://www.youtube.com/watch?v=" + media_id;
}

// static
std::string MediaYouTube::GetChannelUrl(const std::string& publisher_key) {
  std::string res;
  DCHECK(!publisher_key.empty());
  return "https://www.youtube.com/channel/" + publisher_key;
}

// static
std::string MediaYouTube::GetFavIconUrl(const std::string& data) {
  std::string favicon_url = braveledger_media::ExtractData(
      data,
      "\"avatar\":{\"thumbnails\":[{\"url\":\"", "\"");

  if (favicon_url.empty()) {
    favicon_url = braveledger_media::ExtractData(
      data,
      "\"width\":88,\"height\":88},{\"url\":\"", "\"");
  }

  return favicon_url;
}

// static
std::string MediaYouTube::GetChannelId(const std::string& data) {
  std::string id = braveledger_media::ExtractData(data, "\"ucid\":\"", "\"");
  if (id.empty()) {
    id = braveledger_media::ExtractData(
        data,
        "HeaderRenderer\":{\"channelId\":\"", "\"");
  }

  if (id.empty()) {
    id = braveledger_media::ExtractData(
        data,
        "<link rel=\"canonical\" href=\"https://www.youtube.com/channel/",
        "\">");
  }

  if (id.empty()) {
    id = braveledger_media::ExtractData(
      data,
      "browseEndpoint\":{\"browseId\":\"",
      "\"");
  }

  return id;
}

// static
std::string MediaYouTube::GetPublisherName(const std::string& data) {
  std::string publisher_name;
  std::string publisher_json_name = braveledger_media::ExtractData(
      data,
      "\"author\":\"", "\"");
  std::string publisher_json = "{\"brave_publisher\":\"" +
      publisher_json_name + "\"}";
  // scraped data could come in with JSON code points added.
  // Make to JSON object above so we can decode.
  braveledger_bat_helper::getJSONValue(
      "brave_publisher", publisher_json, &publisher_name);
  return publisher_name;
}

void MediaYouTube::OnMediaActivityError(const ledger::VisitData& visit_data,
                                        uint64_t window_id) {
  std::string url = YOUTUBE_TLD;
  std::string name = YOUTUBE_MEDIA_TYPE;

  if (!url.empty()) {
    ledger::VisitData new_data;
    new_data.domain = url;
    new_data.url = "https://" + url;
    new_data.path = "/";
    new_data.name = name;

    ledger_->GetPublisherActivityFromUrl(window_id, new_data, std::string());
  } else {
      BLOG(ledger_, ledger::LogLevel::LOG_ERROR)
        << "Media activity error for "
        << YOUTUBE_MEDIA_TYPE << " (name: "
        << name << ", url: "
        << visit_data.url << ")";
  }
}

void MediaYouTube::ProcessMedia(const std::map<std::string, std::string>& parts,
                                const ledger::VisitData& visit_data) {
  std::string media_id = GetMediaIdFromParts(parts);
  if (media_id.empty()) {
    return;
  }

  std::string media_key = braveledger_media::GetMediaKey(media_id,
                                                         YOUTUBE_MEDIA_TYPE);
  uint64_t duration = GetMediaDurationFromParts(parts, media_key);

  BLOG(ledger_, ledger::LogLevel::LOG_DEBUG) << "Media key: " << media_key;
  BLOG(ledger_, ledger::LogLevel::LOG_DEBUG) << "Media duration: " << duration;

  ledger_->GetMediaPublisherInfo(media_key,
      std::bind(&MediaYouTube::OnMediaPublisherInfo,
                this,
                media_id,
                media_key,
                duration,
                visit_data,
                0,
                _1,
                _2));
}

void MediaYouTube::OnMediaPublisherInfo(
    const std::string& media_id,
    const std::string& media_key,
    const uint64_t duration,
    const ledger::VisitData& visit_data,
    const uint64_t window_id,
    ledger::Result result,
    std::unique_ptr<ledger::PublisherInfo> publisher_info) {
  if (result != ledger::Result::LEDGER_OK &&
      result != ledger::Result::NOT_FOUND) {
    BLOG(ledger_, ledger::LogLevel::LOG_ERROR)
      << "Failed to get publisher info";
    return;
  }

  if (!publisher_info && !publisher_info.get()) {
    std::string media_url = GetVideoUrl(media_id);
    auto callback = std::bind(
        &MediaYouTube::OnEmbedResponse,
        this,
        duration,
        media_key,
        media_url,
        visit_data,
        window_id,
        _1,
        _2,
        _3);

    const std::string url = (std::string)YOUTUBE_PROVIDER_URL + "?format=json&url=" +
        ledger_->URIEncode(media_url);

    FetchDataFromUrl(url, callback);
  } else {
    ledger::VisitData updated_visit_data(visit_data);
    updated_visit_data.name = publisher_info->name;
    updated_visit_data.url = publisher_info->url;
    updated_visit_data.provider = YOUTUBE_MEDIA_TYPE;
    updated_visit_data.favicon_url = publisher_info->favicon_url;
    std::string id = publisher_info->id;
    ledger_->SaveMediaVisit(id, updated_visit_data, duration, window_id);
  }
}

void MediaYouTube::OnEmbedResponse(
    const uint64_t duration,
    const std::string& media_key,
    const std::string& media_url,
    const ledger::VisitData& visit_data,
    const uint64_t window_id,
    int response_status_code,
    const std::string& response,
    const std::map<std::string, std::string>& headers) {
  ledger_->LogResponse(__func__, response_status_code, response, headers);

  if (response_status_code != 200) {
    // embedding disabled, need to scrape
    if (response_status_code == 401) {
      FetchDataFromUrl(visit_data.url,
          std::bind(&MediaYouTube::OnPublisherPage,
                    this,
                    duration,
                    media_key,
                    std::string(),
                    std::string(),
                    visit_data,
                    window_id,
                    _1,
                    _2,
                    _3));
    }
    return;
  }

  std::string publisher_url;
  braveledger_bat_helper::getJSONValue("author_url", response, &publisher_url);
  std::string publisher_name;
  braveledger_bat_helper::getJSONValue("author_name",
                                       response,
                                       &publisher_name);

  auto callback = std::bind(&MediaYouTube::OnPublisherPage,
                            this,
                            duration,
                            media_key,
                            publisher_url,
                            publisher_name,
                            visit_data,
                            window_id,
                            _1,
                            _2,
                            _3);

  FetchDataFromUrl(publisher_url, callback);
}

void MediaYouTube::OnPublisherPage(
    const uint64_t duration,
    const std::string& media_key,
    std::string publisher_url,
    std::string publisher_name,
    const ledger::VisitData& visit_data,
    const uint64_t window_id,
    int response_status_code,
    const std::string& response,
    const std::map<std::string, std::string>& headers) {
  if (response_status_code != 200 && publisher_name.empty()) {
    OnMediaActivityError(visit_data, window_id);
    return;
  }

  if (response_status_code == 200) {
    std::string fav_icon = GetFavIconUrl(response);
    std::string channel_id = GetChannelId(response);

    if (publisher_name.empty()) {
      publisher_name = GetPublisherName(response);
    }

    if (publisher_url.empty()) {
      publisher_url = GetChannelUrl(channel_id);
    }

    SavePublisherInfo(duration,
                      media_key,
                      publisher_url,
                      publisher_name,
                      visit_data,
                      window_id,
                      fav_icon,
                      channel_id);
  }
}

void MediaYouTube::SavePublisherInfo(const uint64_t duration,
                                     const std::string& media_key,
                                     const std::string& publisher_url,
                                     const std::string& publisher_name,
                                     const ledger::VisitData& visit_data,
                                     const uint64_t window_id,
                                     const std::string& fav_icon,
                                     const std::string& channel_id) {
  std::string publisher_id;
  std::string url;
  publisher_id = (std::string)YOUTUBE_MEDIA_TYPE + "#channel:";
  if (channel_id.empty()) {
    BLOG(ledger_, ledger::LogLevel::LOG_ERROR) <<
      "Channel id is missing for: " << media_key;
    return;
  }

  publisher_id += channel_id;
  url = publisher_url + "/videos";

  if (publisher_id.empty()) {
    BLOG(ledger_, ledger::LogLevel::LOG_ERROR) <<
      "Publisher id is missing for: " << media_key;
    return;
  }

  ledger::VisitData updated_visit_data(visit_data);

  if (fav_icon.length() > 0) {
    updated_visit_data.favicon_url = fav_icon;
  }

  updated_visit_data.provider = YOUTUBE_MEDIA_TYPE;
  updated_visit_data.name = publisher_name;
  updated_visit_data.url = url;

  ledger_->SaveMediaVisit(publisher_id,
                          updated_visit_data,
                          duration,
                          window_id);
  if (!media_key.empty()) {
    ledger_->SetMediaPublisherInfo(media_key, publisher_id);
  }
}

void MediaYouTube::FetchDataFromUrl(
    const std::string& url,
    braveledger_media::FetchDataFromUrlCallback callback) {
  ledger_->LoadURL(url,
                   std::vector<std::string>(),
                   std::string(),
                   std::string(),
                   ledger::URL_METHOD::GET,
                   callback);
}


}  // namespace braveledger_media