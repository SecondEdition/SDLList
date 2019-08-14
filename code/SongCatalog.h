#ifndef SONGCATALOG_H_
#define SONGCATALOG_H_

#include <string>
#include <vector>

using namespace std;

// Theoretically what the song files look like in the app we are using minus
// real song file details. For simulation purposes only.  
// I've included only song info relevant to the favorites list. 
struct Song {
    string name;
    int id;   // id is equal to index in our Catalog vector
    int fav_count;  // determines whether and where in Favorites playlist song falls
};


class SongCatalog {
private:
    // Stores songs in a vector. For simulation purposes only. 
    vector<shared_ptr<Song>> Catalog;

    // Used for assigning unique id value to songs
    int next_id;

public:
    SongCatalog();

    ~SongCatalog();
    
    // Adds song element to Catalog
    void InitSong(string name);

    // Returns current size of Catalog vector
    int GetSize();

    // Get playcount of a song
    int GetPlaycount(int id);
    
    // Gets ptr to song for adding to Favorites
    shared_ptr<Song> GetSong(int id);
    
    // "Plays" the song and increments playcount
    void PlaySong(int id);

    // Resets the fav_count for all songs to 0
    void ResetFavCounts();

    // prints each song name, id, and playcount in order
    void PrintCatalog();
};
#endif