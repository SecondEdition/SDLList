#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "../code/SDLList.h"

using namespace std;

// Add a bunch of "songs" to our SongCatalog. This allows us to "play" those songs
// and in so doing simulate how the Favorites Playlist, which uses our SDLList data
// structure, works in this context. 
void FillSmallSongCatalog(SongCatalog *Songs) {
    string line; ifstream songs;
    songs.open ("../input/songs_small.txt");
    if (songs.is_open()) {
        while ( getline (songs,line) ){
            Songs->InitSong(line);
        }
    songs.close();
    }
}

void FillLargeSongCatalog(SongCatalog *Songs) {
    string line; ifstream songs;
    songs.open ("../input/songs_large.txt");
    if (songs.is_open()) {
        while ( getline (songs,line) ){
            Songs->InitSong(line);
        }
    songs.close();
    }
}

// Plays a series of random songs from our Catalog. 
// Each song played runs Update() on our Favorites Playlist with song id to
// to ensure it stays sorted. This is necessary because playcount for each song
// is stored in the Song structs in SongCatalog.h.
void SimulatePlaySongs(SongCatalog *Songs, SDLList *Favorites, int num_plays){
    int random_id;
    srand(time(NULL)); // Helps reset the seed for rand() so there are more random results on each run
    for (size_t i = 0; i < num_plays; i++) {
        random_id = rand() % Songs->GetSize(); // gives random songs in catalog. 
        Songs->PlaySong(random_id);
        Favorites->Update(random_id); // Must be run anytime a song is played in order to keep playlist up-to-date. 
    }
}

// Removes specified number of random songs from our Favorites playlist. 
void SimulateRemoveSongs(SongCatalog *Songs, SDLList *Favorites, int num_removals){
    int random_id;
    int counter = 0;
    srand(time(NULL)); // Helps reset the seed for rand() so there are more random results on each run
    while (counter != num_removals){
        random_id = rand() % Songs->GetSize(); // gives random songs in catalog. 
        // Remove song if it is in playlist otherwise try again. 
        if (Favorites->Find(random_id)) {
            Favorites->RemoveSong(random_id);
            counter++;
        }
    }
}

void RunSmallSimulation(){
    cout << endl;
    cout << "SIMULATION STARTING" << endl; 
    cout << "PLAYING SONGS FROM CATALOG AND ADDING TO FAVORITE'S PLAYLIST:" << endl;
    cout << "Performing setup..." << endl;
    int small_playlist_size =5;
    SongCatalog SOMESONGS;
    SDLList SOMEFAVORITES(small_playlist_size);
    SOMEFAVORITES.SetCatalog(&SOMESONGS); // Link Favorites Playlist to Song Catalog. 
    FillSmallSongCatalog(&SOMESONGS); // Add "songs" to Catalog, for simulation purposes.
    cout << "Setup complete." << endl; cout << endl;

    cout << "Simulating playing a bunch of songs from our playlist, which get added to playlist as we go..." << endl;
    int num_plays = 50;
    SimulatePlaySongs(&SOMESONGS, &SOMEFAVORITES, num_plays); cout << endl;

    cout << "Favorites Playlist after playing songs:" << endl; 
    SOMEFAVORITES.PrintPlaylist(); cout << endl;
    cout << "Catalog counts after playing songs:" << endl;
    SOMESONGS.PrintCatalog(); cout << endl; cout << endl;

    cout << "SIMULATING REMOVAL OF SONGS:" << endl;
    cout << "Playist BEFORE song removal:" << endl;
    SOMEFAVORITES.PrintPlaylist();
    cout << endl;
    SimulateRemoveSongs(&SOMESONGS, &SOMEFAVORITES, 2);
    cout << endl;
    cout << "Playist AFTER song removal:" << endl;
    SOMEFAVORITES.PrintPlaylist(); cout << endl; cout << endl;

    cout << "SIMULATING CLEARING OF ENTIRE PLAYLIST:" << endl;
    cout << endl;
    SOMEFAVORITES.Clear(); cout << endl;
    cout << "Playist after song removal:" << endl;
    SOMEFAVORITES.PrintPlaylist(); cout << endl;
    cout << "Catalog after using Clear():" << endl;
    SOMESONGS.PrintCatalog();

    cout << endl; cout << "SIMULATION ENDING" << endl; cout << endl;
}

void RunLargeSimulation(){
    cout << endl;
    cout << "SIMULATION STARTING" << endl; 
    cout << "PLAYING SONGS FROM A LARGE CATALOG AND ADDING TO FAVORITE'S PLAYLIST:" << endl;
    cout << "Performing setup..." << endl; 
    int large_playlist_size =20;
    SongCatalog MANYSONGS;
    SDLList MOREFAVORITES(large_playlist_size);
    MOREFAVORITES.SetCatalog(&MANYSONGS); // Link Favorites Playlist to Song Catalog. 
    FillSmallSongCatalog(&MANYSONGS); // Add "songs" to Catalog, for simulation purposes. 
    cout << "Setup complete." << endl; cout << endl;

    cout << "Simulating playing a bunch of songs from our playlist, which get added to playlist as we go..." << endl;
    int num_plays = 500;
    SimulatePlaySongs(&MANYSONGS, &MOREFAVORITES, num_plays); cout << endl;
    
    cout << "Favorites Playlist after playing songs:" << endl; 
    MOREFAVORITES.PrintPlaylist(); cout << endl;
    cout << "Catalog counts after playing songs:" << endl;
    MANYSONGS.PrintCatalog(); cout << endl; cout << endl;

    cout << "SIMULATING REMOVAL OF SONGS:" << endl;
    cout << "Playist BEFORE song removal:" << endl;
    MOREFAVORITES.PrintPlaylist();
    cout << endl;
    SimulateRemoveSongs(&MANYSONGS, &MOREFAVORITES, 8);
    cout << endl;
    cout << "Playist AFTER song removal:" << endl;
    MOREFAVORITES.PrintPlaylist(); cout << endl; cout << endl;

    cout << "SIMULATING CLEARING OF ENTIRE PLAYLIST:" << endl;
    cout << endl;
    MOREFAVORITES.Clear(); cout << endl;
    cout << "Playist after song removal:" << endl;
    MOREFAVORITES.PrintPlaylist(); cout << endl;
    cout << "Catalog after using Clear():" << endl;
    MANYSONGS.PrintCatalog();

    cout << endl; cout << "SIMULATION ENDING" << endl; cout << endl;
}

int main() {
    
    RunSmallSimulation();

    // UNCOMMENT OUT THIS COMMAND TO RUN THE LARGE SIMULATION, WHICH IS OFF BY DEFAULT:
    // RunLargeSimulation();

    return 0;
}