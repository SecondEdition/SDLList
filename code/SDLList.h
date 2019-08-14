#ifndef SDLLIST_H_
#define SDLLIST_H_

#include <fstream>
#include "SongCatalog.h"

using namespace std;

// node containing song information as it pertains to playlist. 
struct Fsong {
    shared_ptr<Fsong> prev; // previous song in playlist
    shared_ptr<Fsong> next; // next song in playlist
    shared_ptr<Song> songfile; // links to <Song> struct in SongCatalog
    bool omitted; // flag used to omit songs for playlist user no longer wants. 
};

// Favorites Playlist metadata stored in this struct. 
struct Fplaylist {
    shared_ptr<Fsong> first; // ptr to first song in the favorites playlist
    shared_ptr<Fsong> last; // prt to last song in the favorites playlist
    int curr_size; // current size of the playlist
    int max_size; // maximum number of songs in playlist
};

// SDLList stands for Self-sorting Doubly Linked List!
class SDLList {
private: 
    // Ptr to struct storing playlist metadata
    shared_ptr<Fplaylist> FAVDATA;

    // Creates playlist struct with starting metadata values.  
    void InitPlaylist(int max_size);

    // Initializes Fsong struct & values when songs are added to playlist. 
    shared_ptr<Fsong> InitFSong();

    // returns lowest playcount of songs in the list
    int GetLowCount();

    // Used by Update() to determine whether a sort is needed. 
    void SortCheck(shared_ptr<Fsong> song);

    // Simple recursive bubble sort algorithm.
    void Sort();

    // Used by Sort() to swap nodes in SDDList. 
    void Swap(shared_ptr<Fsong> left, shared_ptr<Fsong> right);

public:
    // Constructors
    SDLList(); // Default behavior is undefined. 
    SDLList(int max_size);

    SongCatalog * SONGS;

    // Deconstructor
    ~SDLList();
    
    // Sets ptr to the song catalog this SDLList refers to. 
    void SetCatalog(SongCatalog *CATALOG);

    // returns true if list is empty
    bool IsEmpty();
    
    // returns true if list is full (max_size)
    bool IsFull();

    // nullptr if Fsong doesn't exist, otherwise returns ptr. 
    shared_ptr<Fsong> Find(int id);

    // Appends a song to the end of the list. Edge cases where
    // the last node we are replacing has the same fav_count as 
    //it's predecessor(s) means we need to sort after adding 
    // new favorite songs.
    void AddSong(shared_ptr<Song> song);

    // Must be run on the playlist whenever a song 
    // in the catalog is played. Ensures the sort 
    // order and set of favorites stays up-to-date. 
    void Update(int id);
    
    // Removing song also resets the fav_count!
    // Song in linked list is not replaced until 
    // more songs are played
    void RemoveSong(int id);

    // Removes all songs from Favorites playlist and
    // resets the fav_count for all songs in catalog!
    void Clear();

    // Return a vector of Favorites to main app in order
    // to display to user and allow play selection. 
    vector<shared_ptr<Fsong>> GetFavorites ();

    //================================================//
    //                                                // 
    //  PRINT METHODS BELOW - PRIMARILY FOR TESTING   //
    //                                                //
    //================================================//

    // Prints the name of the song Fsong points to
    void PrintFSong(shared_ptr<Fsong> Fsong);

    // Mostly for testing
    // Prints song name, id, and playcount for each song in order.
    void PrintPlaylist();

    // same as PrintPlaylist() but in reverse
    // helps tests whether backwards links between nodes are correct
    void PrintPlaylistReverse();

};

#endif