#include "videoplaylist.h"
#include <string>
#include <vector>



VideoPlaylist::VideoPlaylist(string name, vector<string> videosid) {
     mTitle = move(name);
     playlistVideos = move(videosid);

}

//check if the video found in the playlist, returns true if found ,false if not.
const bool VideoPlaylist::VideoFound(string& Videoid) const{
     bool found = false;
     for (const auto& video : playlistVideos) {
          if (video == Videoid){
               found = true;
               return found; 
          }
     }
     return found; 
     
}

//add video to the playlist.
const void VideoPlaylist::AddVideo(string& videoid)const {
     playlistVideos.push_back(videoid);
}

//get playlist name.
const string& VideoPlaylist::getTitle()const{return mTitle;}

//get playlist videos.
const vector<string>& VideoPlaylist::getPlaylistVideos()const { return playlistVideos; }

//remove the video from the playlist.
const void VideoPlaylist::RemoveVideo(string& Videoid)const {
     int i = 0; 
     for (const auto& video : playlistVideos) {
          if (video == Videoid) {
               playlistVideos.erase(playlistVideos.begin()+i);
          }
          i++;
     }
}
//clear playlist.
const void VideoPlaylist::ClearPlaylist()const {
     playlistVideos.clear(); 
}