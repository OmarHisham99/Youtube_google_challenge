#include "videolibrary.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "helper.h"
#include "video.h"

VideoLibrary::VideoLibrary() {
  std::ifstream file("./src/videos.txt");
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      std::stringstream linestream(line);
      std::string title;
      std::string id;
      std::string tag;
      std::vector<std::string> tags;
      std::getline(linestream, title, '|');
      std::getline(linestream, id, '|');
      while (std::getline(linestream, tag, ',')) {
        tags.emplace_back(trim(std::move(tag)));
      }
      Video video = Video(trim(std::move(title)), trim(id), std::move(tags));
      mVideos.emplace(trim(std::move(id)), std::move(video));
    }
  } else {
    std::cout << "Couldn't find videos.txt" << std::endl;
  }
}


std::vector<Video> VideoLibrary::getVideos() const {
  std::vector<Video> result;
  for (const auto& video : mVideos) {
    result.emplace_back(video.second);
  }
  return result;
}

const Video* VideoLibrary::getVideo(const std::string& videoId) const {
  const auto found = mVideos.find(videoId);
  if (found == mVideos.end()) {
    return nullptr;
  } else {
    return &(found->second);
  }
}
     
//returns a list with all playlists available
vector<VideoPlaylist> VideoLibrary::getPlaylists() const {
     vector<VideoPlaylist> result; 
     for (const auto& playlist : mplaylists){
          result.emplace_back(playlist.second); 
     }
     return result; 
}
    
//returns the playlist if found or nullptr,nullptr refers to not found.
const VideoPlaylist* VideoLibrary::getPlaylist(const string& playlistName) const {
     string id = toLower(playlistName);
     const auto found = mplaylists.find(id); 
     if (found == mplaylists.end()) {
          return nullptr; 
     }
     else {
          return &(found->second);
     }
}

void VideoLibrary::AddNewPlaylist(string& playlistName) const {
     string playlist_id = toLower(playlistName); 
     string title = playlistName; 
     //if playlist with the same name is found it will refuse to create new one.
     if (getPlaylist(playlist_id) != nullptr){
          cout << "Cannot create playlist: A playlist with the same name already exists" << endl;
          return; 
     }
     //create a new playlist.
     vector<string> Videos; 
     VideoPlaylist playlist = VideoPlaylist(move(playlistName), move(Videos));
     mplaylists.emplace(trim(move(playlist_id)),move(playlist));
     cout << "Successfully created new playlist: " << title << endl; 
}

void VideoLibrary::AddVideoToPlaylist(string& playlistName, string& VideoId)const {
    const VideoPlaylist* playlist = getPlaylist(playlistName);
    //check if there is not a playlist with this name.
    if (playlist == nullptr) {
         cout << "Cannot add video to " << playlistName << ": Playlist does not exist" << endl;
         return;
    }
    auto const& vid = getVideo(VideoId);
    //check if there is not a video with this id.
    if (vid == nullptr) {
         cout << "Cannot add video to " << playlistName << ": Video does not exist" << endl;
         return;
    }
    //check if the video is added before to this playlist,VideoFound returns true if found in the playlist.
    bool addedbefore = playlist->VideoFound(VideoId);
    if (addedbefore) {
         cout << "Cannot add video to " << playlistName << ": Video already added" << endl;
         return;
    }
    //add the video if it is not added before.
    playlist->AddVideo(VideoId);
    cout << "Added video to " << playlistName << ": " << vid->getTitle() << endl;

}

void VideoLibrary::RemoveVideoFromPlaylist(string& playlistName, string& Videoid)const {
     const VideoPlaylist* playlist = getPlaylist(playlistName);
     //check if the playlist is not found.
     if (playlist == nullptr) {
          cout << "Cannot remove video from " << playlistName << ": Playlist does not exist"<<endl;
          return; 
     }
     //check if the video is not found
     auto const vid = getVideo(Videoid); 
     if (vid == nullptr) {
          cout << "Cannot remove video from " << playlistName << ": Video does not exist" << endl;
          return; 
     }
     //check if the video is not in the playlist.
     bool videoExist = playlist->VideoFound(Videoid);
     if (!videoExist) {
          cout << "Cannot remove video from " << playlistName << ": Video is not in playlist" << endl;
          return; 
     }
     //delete the video from the playlist.
     playlist->RemoveVideo(Videoid); 
     cout << "Removed video from " << playlistName <<": "<<vid->getTitle()<<endl; 
}

//search for the playlist in and delete it by erase function used for maps.
void VideoLibrary::DeletePlaylist(string& playlistName)const {
     string key = toLower(playlistName); 
     map<string, VideoPlaylist>::iterator it; 
     it = mplaylists.find(key); 
     mplaylists.erase(it); 
}

bool VideoLibrary::isFlagged(string videoId) const {
     auto found = mflaggedVideos.find(videoId);
     //check the video is not flagged, false refers to not found.
     if (found == mflaggedVideos.end()) {
          return false;
     }
     //retrun true if flagged, true refers to flagged.
     else
          return true; 
}

//get flagged video reason.
string VideoLibrary::getReason(string videoId) const {
     string vidId = toLower(videoId); 
     auto found = mflaggedVideos.find(vidId); 
     return found->second; 
}

//add the video to flagged videos list. 
void VideoLibrary::AddNewFlaggedVideo(const string videoId, const string reason)const {
     mflaggedVideos.emplace(trim(move(videoId)), move(reason));
}

//remove the video from the flagged videos.
void VideoLibrary::RemoveFlaggedVideo(string videoId) const {
     string key = toLower(videoId); 
     map<string, string>::iterator it;
     it = mflaggedVideos.find(key); 
     mflaggedVideos.erase(it); 
}

//get the number of flagged video.
int VideoLibrary::getFlaggedVideosNumber()const {
     return mflaggedVideos.size(); 
}