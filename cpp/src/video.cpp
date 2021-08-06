#include "video.h"

#include <iostream>
#include <utility>
#include <vector>
using namespace std; 
Video::Video(std::string&& title, std::string&& videoId,
             std::vector<std::string>&& tags) :
  mTitle(std::move(title)),
  mVideoId(std::move(videoId)),
  mTags(std::move(tags)) {
}

const std::string& Video::getTitle() const { return mTitle; }

const std::string& Video::getVideoId() const { return mVideoId; }

const std::vector<std::string>& Video::getTags() const { return mTags; }


//return all tags as one string.
const std::string Video::ConvertTagsToString() const{
     std::string s ;
     for (const auto& tag : mTags) {
          s += tag; 
          s += " ";
     }
     s = s.substr(0, s.size() - 1);
     return s; 
}