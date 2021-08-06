#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <map>
#include "video.h"
#include "videoplaylist.h"


using namespace std; 
/**
 * A class used to represent a Video Library.
 */
class VideoLibrary {
 private:
      //used map to auto order the videos.
  std::map<std::string, Video> mVideos;
  mutable map<string, VideoPlaylist> mplaylists;
  mutable map<string,string> mflaggedVideos; 

 public:
  VideoLibrary();

  // This class is not copyable to avoid expensive copies.
  VideoLibrary(const VideoLibrary&) = delete;
  VideoLibrary& operator=(const VideoLibrary&) = delete;

  // This class is movable.
  VideoLibrary(VideoLibrary&&) = default;
  VideoLibrary& operator=(VideoLibrary&&) = default;

  std::vector<Video> getVideos() const;
  const Video *getVideo(const std::string& videoId) const;

  //flagged videos methods
  bool isFlagged(string videoId) const; 
  string getReason(string videoId) const; 
  int getFlaggedVideosNumber()const; 
  void AddNewFlaggedVideo(string videoId, string reason)const;
  void RemoveFlaggedVideo(string vidoeId) const;

  //playlist methods 
  vector<VideoPlaylist> getPlaylists() const; 
  const VideoPlaylist* getPlaylist(const string& playlistName) const; 
  void AddNewPlaylist(string& playlistName) const; 
  void AddVideoToPlaylist(string& playlistName, string& Videoid) const; 
  void RemoveVideoFromPlaylist(string& playlistName, string& Videoid)const; 
  void DeletePlaylist(string& playlistName) const;
};
