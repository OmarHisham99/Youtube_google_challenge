#include "videoplayer.h"

#include <iostream>
#include <algorithm>
#include <queue>
#include <regex>
#include "helper.h"
#include "videoplaylist.h"
#include "videolibrary.h"

using namespace std;
void VideoPlayer::numberOfVideos() {
     std::cout << mVideoLibrary.getVideos().size() << " videos in the library"
          << std::endl;
}

void VideoPlayer::showAllVideos() {
     cout << "Here's a list of all available videos:" << endl;
     vector<Video> result = mVideoLibrary.getVideos();
     //displaying all videos
     for (const auto& vid : result) {
          string tags = vid.ConvertTagsToString();
          cout << vid.getTitle() << " (" << vid.getVideoId() << ") ";
          cout << "[" << tags << "]";
          //if the video flagged it will print flagged case.
          if (mVideoLibrary.isFlagged(vid.getVideoId())) {
               cout << " - FLAGGED (reason: " << mVideoLibrary.getReason(vid.getVideoId()) << ")";    
          }
          cout << endl; 
     }
     
}

void VideoPlayer::playVideo(const std::string& videoId) {
     //check that the video is flagged.
     if (mVideoLibrary.isFlagged(videoId)) {
          cout << "Cannot play video: Video is currently flagged (reason: " << mVideoLibrary.getReason(videoId)
               << ")" << endl; 
          return;
     }
     //check that video is availble to play.
     if (mVideoLibrary.getVideo(videoId) != nullptr) {
          //if no videos was played before it will start immediatley
          if (playingVideos.empty()) {
               cout << "Playing video: " << mVideoLibrary.getVideo(videoId)->getTitle() << endl;
               playingVideos.push(videoId);
          }
          //if there is a video playing it will stop it and play the new video.
          else if (!playingVideos.empty()) {
               playingVideos.push(videoId);
               stopVideo();
               cout << "Playing video: " << mVideoLibrary.getVideo(playingVideos.front())->getTitle() << endl;
          }
     }
     else {
          cout << "Cannot play video: Video does not exist" << endl;
     }
  

}

void VideoPlayer::stopVideo() {
     //check that no videos are playing.
     if (playingVideos.empty()) {
          cout << "Cannot stop video: No video is currently playing" << endl;
          return;
     }
     //clear the video from playing list and paused list to stop it.
     else {
          cout << "Stopping video: " << mVideoLibrary.getVideo(playingVideos.front())->getTitle() << endl;
          playingVideos.pop();
          pausedVideos.clear();
     }
}

void VideoPlayer::playRandomVideo() {

     //get all videos availble and put them to result.
     vector<Video> result = mVideoLibrary.getVideos();
     //generate a random number to play a random video by using rand function.
     int videoSize = result.size();
     int randomVideo = rand() % videoSize;
     //if all videos are flagged or the no available videos it will not do anything.
     if (videoSize == 0 || mVideoLibrary.getFlaggedVideosNumber() == videoSize) {
          cout << "No videos available" << endl;
          return;
     }
     //get a random video id to play. 
     string vidId = result.at(randomVideo).getVideoId();
     //check if this video is flagged or not.
     if (mVideoLibrary.isFlagged(vidId)) {
          cout << "Cannot play video: Video is currently flagged (reason: " << mVideoLibrary.getReason(vidId)
               << ")" << endl;
          return;
     }
     //check that it is the first video to play. 
     if (playingVideos.empty()) {
          playVideo(result.at(randomVideo).getVideoId());
     }
     //check that there is a video is currently playing so it stops the video and play the new random one.
     else {
          stopVideo();
          playVideo(result.at(randomVideo).getVideoId());
     }
}

void VideoPlayer::pauseVideo() {
     //check if video is not playing.
     if (pausedVideos.empty() && playingVideos.empty()) {
          cout << "Cannot pause video: No video is currently playing" << endl;
     }
     //check if there is a video playing and not paused.
     else if (pausedVideos.empty() && !playingVideos.empty()) {
          pausedVideos.emplace_back(playingVideos.front());
          cout << "Pausing video: " << mVideoLibrary.getVideo(pausedVideos.front())->getTitle() << endl;
     }
     //check if the video is paused already.
     else if (!pausedVideos.empty()) {
          cout << "Video already paused: " << mVideoLibrary.getVideo(pausedVideos.front())->getTitle() << endl;
     }
     
}

void VideoPlayer::continueVideo() {
     //to continue the video must be not paused.
     if (pausedVideos.empty() && !playingVideos.empty()) {
          cout << "Cannot continue video: Video is not paused" << endl;
     }
     //check if there is video currently playing.
     else if (pausedVideos.empty() && playingVideos.empty()) {
          cout << "Cannot continue video: No video is currently playing" << endl;
     }
     else {
          cout << "Continuing video: " << mVideoLibrary.getVideo(pausedVideos.front())->getTitle() << endl;
          //delete the paused video from paused videos list.
          pausedVideos.clear();
     }
     
}

void VideoPlayer::showPlaying() {
    //check if there is video currently playing or not.
     if (playingVideos.empty()) {
          cout << "No video is currently playing" << endl;
     }
     //display the currently playing video and it's state, pasued or not.
     else {
          const auto video = mVideoLibrary.getVideo(playingVideos.front());
          string tags = video->ConvertTagsToString();
          cout << "Currently playing: " << video->getTitle() << " ("
               << video->getVideoId() << ") ";
          cout << "[" << tags << "]"; 
         //check if there are in videos paused.
          if (!pausedVideos.empty())
               cout << " - PAUSED";
          cout << endl;
     }
}

void VideoPlayer::createPlaylist(const std::string& playlistName) {
     string title = playlistName;
     mVideoLibrary.AddNewPlaylist(title);
 
}

void VideoPlayer::addVideoToPlaylist(const std::string& playlistName,
     const std::string& videoId) {
     string plName = playlistName;
     string vidId = videoId;
     //check if the video is flagged, if flaaged it will not be added to the playlist. 
     if (mVideoLibrary.isFlagged(vidId)) {
          cout << "Cannot add video to " << plName << ": Video is currently flagged"; 
          cout << " (reason: " << mVideoLibrary.getReason(vidId) <<")"<<endl;
          return; 
     }
     mVideoLibrary.AddVideoToPlaylist(plName, vidId);
    
}

void VideoPlayer::showAllPlaylists() {
     vector<VideoPlaylist> result = mVideoLibrary.getPlaylists();
     //check if there are playlist found.
     if (result.empty()) {
          cout << "No playlists exist yet" << endl;
          return;
     }
     cout << "Showing all playlists:" << endl;

     //displaying playlists found.
     for (const auto& playlist : result) {
          cout <<" "<<playlist.getTitle()<<endl; 

     }
}

void VideoPlayer::showPlaylist(const std::string& playlistName) {
     const VideoPlaylist* result = mVideoLibrary.getPlaylist(playlistName);

     //check if the playlist is found or not, nullptr refers to not found.
     if (result == nullptr) {
          cout << "Cannot show playlist " << playlistName << ": Playlist does not exist" << endl; 
          return; 
     }
     cout << "Showing playlist: " << playlistName << endl;
     //get all videos from the playlist
     vector<string> videosInPlaylist = result->getPlaylistVideos();
     //check if there are no videos in the playlist
     if (videosInPlaylist.empty()) {
          cout << " No videos here yet" << endl; 
          return;
     }
     //show all playlist videos.
     for (auto const& video : videosInPlaylist) {
          auto vid = mVideoLibrary.getVideo(video); 
          cout << " " << vid->getTitle() << " ";
          cout << "(" << vid->getVideoId() << ") ";
          cout << "[" << vid->ConvertTagsToString() << "]";
          //check if the video is flagged or not. 
          if (mVideoLibrary.isFlagged(vid->getVideoId())) {
               cout << " - FLAGGED (reason: " << mVideoLibrary.getReason(vid->getVideoId()) << ")";
          }
          cout << endl;
     }
}

void VideoPlayer::removeFromPlaylist(const std::string& playlistName,
     const std::string& videoId) {
     string plName = playlistName; 
     string VidId = videoId; 
     mVideoLibrary.RemoveVideoFromPlaylist(plName, VidId); 
}

void VideoPlayer::clearPlaylist(const std::string& playlistName) {
     const VideoPlaylist* playlist = mVideoLibrary.getPlaylist(playlistName); 
     //check if the playlist is found or not, nullptr refers to not found.
     if (playlist == nullptr) {
          cout << "Cannot clear playlist " << playlistName << ": Playlist does not exist" << endl; 
          return; 
     }
     playlist->ClearPlaylist();
      cout << "Successfully removed all videos from " << playlistName << endl;
}

void VideoPlayer::deletePlaylist(const std::string& playlistName) {
     const VideoPlaylist* playlist = mVideoLibrary.getPlaylist(playlistName); 
     string plName = playlistName; 
     //check if the playlist is found or not, nullptr refers to its not found.
     if (playlist == nullptr) {
          cout << "Cannot delete playlist " << playlistName << ": Playlist does not exist" << endl;
          return;
     }
     mVideoLibrary.DeletePlaylist(plName);
     cout << "Deleted playlist: " << playlistName << endl; 
}

void VideoPlayer::searchVideos(const std::string& searchTerm) {
     
     //get all videos available 
     vector<Video> result = mVideoLibrary.getVideos();
     //the output videos will be inserted in this vector.
     vector<Video> videos; 
     string search_term = toLower(searchTerm);
     int i = 0;
     for (const auto& video : result) {
          string videoTitle = toLower(video.getTitle());
          // if the result from function find is a number then the term is found,
          //we check also if the video is not flagged, if it met this condtions then we put it in the output vector.
          if (videoTitle.find(search_term)!=SIZE_MAX && !mVideoLibrary.isFlagged(video.getVideoId()) ){
               videos.insert(videos.begin()+i,video); 
               i++; 
          }
     }
     if (i == 0) {
          cout << "No search results for " << searchTerm << endl; 
     }
     else {
          cout << "Here are the results for " << searchTerm << ":" << endl;
          i = 0;
          //show videos with required term.
          while (i<videos.size()) {
               cout << " " << i+1 << ") " << videos.at(i).getTitle() << " ";
               cout << "(" << videos.at(i).getVideoId() << ") ";
               cout << "[" << videos.at(i).ConvertTagsToString() << "]" << endl;
               i++;
          }
          cout << "Would you like to play any of the above? If yes, specify the number of the video." << endl;
          cout << "If your answer is not a valid number, we will assume it's a no." << endl;
          string answer;
          getline(cin, answer);
          //if answer size is bigger than required numbers to enter it will stop.
          if (answer.size() > 1) {
               return;
          }
          int number = stoi(answer);
          //check if the number entered is availble to play.
          if (number-1 <= videos.size()) {
               playVideo(videos.at(number-1).getVideoId());
          }
          else { return; }
          //clear all videos that appears as result for the search.
          videos.erase(videos.begin() + number-1);

     }
}

void VideoPlayer::searchVideosWithTag(const std::string& videoTag) {
    
     //get all videos available 
     vector<Video> result = mVideoLibrary.getVideos();
     //the output videos will be inserted in this vector.
     vector<Video> videos;
     string video_tag = toLower(videoTag);
     //check first if it is a tag term or not by checking on #
     if (video_tag.find("#") == SIZE_MAX) { 
          cout << "No search results for " << videoTag << endl;
          return; 
     }
     int i = 0;
     for (const auto& video : result) {
          //get all video tags 
          string videoTitle = toLower(video.ConvertTagsToString());
          // if the result from function find is a number then the tag is found,
          //we check also if the video is not flagged, if it met this condtions then we put it in the output vector 
          if (videoTitle.find(video_tag) != SIZE_MAX && !mVideoLibrary.isFlagged(video.getVideoId())) {
               videos.insert(videos.begin() + i, video);
               i++;
          }
     }
     if (i == 0) {
          cout << "No search results for " << videoTag << endl;
     }
     else {
          cout << "Here are the results for " << videoTag << ":" << endl;
          i = 0;
          //show videos with required tag
          while (i < videos.size()) {
               cout << " " << i + 1 << ") " << videos.at(i).getTitle() << " ";
               cout << "(" << videos.at(i).getVideoId() << ") ";
               cout << "[" << videos.at(i).ConvertTagsToString() << "]" << endl;
               i++;
          }
          cout << "Would you like to play any of the above? If yes, specify the number of the video." << endl;
          cout << "If your answer is not a valid number, we will assume it's a no." << endl;
          string answer;
          getline(cin, answer);
          //if answer size is bigger than required numbers to enter it will stop.
          if (answer.size() > 1) {
               return;
          }
          int number = stoi(answer);
          //check if the number entered is availble to play.
          if (number - 1 <= videos.size()) {
               playVideo(videos.at(number - 1).getVideoId());
          }
          else { return; }
          //clear all videos that appears as result for the search.
          videos.erase(videos.begin() + number - 1);

     }
}

void VideoPlayer::flagVideo(const std::string& videoId) {
     const auto video = mVideoLibrary.getVideo(videoId); 
    //check if the video found or not, nullptr refers to not found.
     if (video == nullptr) {
          cout << "Cannot flag video: Video does not exist" << endl; 
          return;
     }
     
     //check if the video is flagged already or not. 
     if (mVideoLibrary.isFlagged(videoId)) {
          cout << "Cannot flag video: Video is already flagged" << endl;
          return;
     }

     else {
          if (!playingVideos.empty()) {
               if ((video->getVideoId() == playingVideos.front())) {
                    stopVideo();
               }
          }
          mVideoLibrary.AddNewFlaggedVideo(videoId ,"Not supplied"); 
          cout << "Successfully flagged video: " << mVideoLibrary.getVideo(videoId)->getTitle();
          cout << " (reason: " << mVideoLibrary.getReason(videoId) << ")" << endl;
          
     }
}

void VideoPlayer::flagVideo(const std::string& videoId, const std::string& reason) {
     const auto video = mVideoLibrary.getVideo(videoId);
     
     //check the video if it is exist or not, nullptr refers to it is not found.
     if (video == nullptr) {
          cout << "Cannot flag video: Video does not exist" << endl;
          return;
     }

     //check if the video is flagged already or not. 
     if (mVideoLibrary.isFlagged(videoId)) {
          cout << "Cannot flag video: Video is already flagged" << endl;
          return;
     }

     else {
          //we check if any videos running in the queue already.
          if (!playingVideos.empty()) {
               //if there is a video in the queue it stops it before flagging the video.
               if (video->getVideoId() == playingVideos.front()) {
                    stopVideo();
               }
          }
          //add the video to flagged videos.
          mVideoLibrary.AddNewFlaggedVideo(videoId, reason);
          cout << "Successfully flagged video: " << mVideoLibrary.getVideo(videoId)->getTitle();
          cout << " (reason: " << mVideoLibrary.getReason(videoId) << ")" << endl;

     }
}

void VideoPlayer::allowVideo(const std::string& videoId) {
     auto found = mVideoLibrary.getVideo(videoId); 
     
     //nullptr refers to not found video.
     if (found == nullptr) {
          cout << "Cannot remove flag from video: Video does not exist" << endl; 
          return; 
     }
     // this describe that the video is allowed already
     if (!mVideoLibrary.isFlagged(videoId)) {
          cout << "Cannot remove flag from video: Video is not flagged" << endl;
          return; 
     }

     //remove flagged video deletes the video from the flagged videos
     mVideoLibrary.RemoveFlaggedVideo(videoId); 
     cout << "Successfully removed flag from video: " << mVideoLibrary.getVideo(videoId)->getTitle() << endl;
}
