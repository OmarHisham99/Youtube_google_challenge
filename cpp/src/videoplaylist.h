#pragma once
#include <string>
#include <vector>
#include "Video.h"

/* A class used to represent a Playlist */

using namespace std;

class VideoPlaylist {
private:
   string mTitle; 
   mutable vector<string> playlistVideos; 

public:
     VideoPlaylist() = default; 
     VideoPlaylist(string name, vector<string> videosid);
     const string& getTitle()const; 
     const vector<string>& getPlaylistVideos()const;
     const bool VideoFound(string& Videoid) const; 
     const void AddVideo(string& Videoid) const;
     const void RemoveVideo(string& Videoif)const; 
     const void ClearPlaylist()const; 
};
