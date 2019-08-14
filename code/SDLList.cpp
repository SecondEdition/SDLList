#include <iostream>
#include <string>
#include <memory>
#include "SDLList.h"

using namespace std;

SDLList::SDLList(){
    // Using default constructor is undefined
}

SDLList::SDLList(int max_size){
    InitPlaylist(max_size);
}

SDLList::~SDLList(){
}

void SDLList::SetCatalog(SongCatalog *CATALOG){
    SONGS = CATALOG;
}

void SDLList::InitPlaylist(int max_size){
    shared_ptr<Fplaylist> fav (new Fplaylist);
    fav->curr_size = 0;
    fav->first = (nullptr);
    fav->last = (nullptr);
    fav->max_size = max_size;
    FAVDATA = fav;
}

shared_ptr<Fsong> SDLList::InitFSong(){

    shared_ptr<Fsong> ret (new Fsong);
    ret->next = nullptr;
    ret->prev = nullptr;
    ret->omitted = false;
    ret->songfile = nullptr;

    return ret;
}

int SDLList::GetLowCount() {
    if (IsEmpty()){
        return 0;
    }
    return FAVDATA->last->songfile->fav_count;
}

bool SDLList::IsEmpty() {
    if (!FAVDATA->first && !FAVDATA->last) {
        return true;
    }
    return false;
}

bool SDLList::IsFull() {
    if (FAVDATA->curr_size == FAVDATA->max_size) {
        return true;
    }
    return false;
}

shared_ptr<Fsong> SDLList::Find(int id){
    if (IsEmpty()) {
        return nullptr;
    }
    shared_ptr<Fsong> curr_node (FAVDATA->first);
    while (curr_node){
        if (curr_node->songfile->id == id) {
            return curr_node;
        }
        curr_node = curr_node->next;
    }
    return nullptr;
}

void SDLList::AddSong(shared_ptr<Song> song){

    shared_ptr<Fsong> new_fav (InitFSong()); 
    new_fav->songfile = song;
    
    // Swap out new node with the playlist's last node if it is full
    if (IsFull()) {
        FAVDATA->last->prev->next = new_fav;
        new_fav->prev = FAVDATA->last->prev;
        FAVDATA->last = new_fav;
        SortCheck(new_fav);
    }
    // Set's root metadata for empty list
    else if (IsEmpty()) {
        FAVDATA->last = new_fav;
        FAVDATA->first = new_fav;
        FAVDATA->curr_size++;
    }
    // Appending new song to the end of non-full list:
    else {
        FAVDATA->curr_size++;
        new_fav->prev = FAVDATA->last;
        FAVDATA->last->next = new_fav;
        FAVDATA->last = new_fav;
        SortCheck(new_fav);
    }
}

void SDLList::SortCheck(shared_ptr<Fsong> song) {
    // Skip update if song is the head of list already
    if (song == FAVDATA->first) {
        return;
    }
    // Initiate sort if song now has a higher count than its predecessor. 
    else if (song->songfile->fav_count > song->prev->songfile->fav_count) {
        Sort();
    }
}

void SDLList::Sort(){
    if (IsEmpty()) {
        return;
    }
    else {
        bool sorted = true;
        shared_ptr<Fsong> song (FAVDATA->first);

        // if swap happens with last two nodes song becomes nullptr so we check for that also.
        while (song != FAVDATA->last && song) {  
            if (song->songfile->fav_count < song->next->songfile->fav_count) {
                Swap(song, song->next);
                sorted = false;
            }
            song = song->next;
        }
        if (!sorted){
            Sort();
        }
    }
}

void SDLList::Swap(shared_ptr<Fsong> left, shared_ptr<Fsong> right){

    // Different actions if left is the root node
    if (left == FAVDATA->first){
        right->prev = nullptr;
        FAVDATA->first = right;
    }
    else {
        left->prev->next = right;
        right->prev = left->prev;
    }
    
    left->prev = right;

    // Different actions if right is the tail node
    if (right == FAVDATA->last){
        left->next = nullptr;
        FAVDATA->last = left;
    } 
    else {
        left->next = right->next;
        left->next->prev = left;
    }

    right->next = left;
}

void SDLList::RemoveSong(int id){
    shared_ptr<Fsong> song (Find(id));
    if (song){
        cout << "Removing song.. \"" << song->songfile->name << "\"" << endl;
        song->songfile->fav_count = 0;

        if (song == FAVDATA->first && song == FAVDATA->last) {
            FAVDATA->first = nullptr;
            FAVDATA->last = nullptr;
        }
        else if (song == FAVDATA->first) {
            song->next->prev = nullptr;
            FAVDATA->first = song->next;
        }
        else if (song == FAVDATA->last) {
            song->prev->next = nullptr;
            FAVDATA->last = song->prev;
        }
        else {
            song->prev->next = song->next;
            song->next->prev = song->prev;
        }
        FAVDATA->curr_size--;
    }
}

void SDLList::Update(int id){
    if (!IsEmpty()){
        // Update sort if played song is already in playlist
        shared_ptr<Fsong> Fav_Song (Find(id));
        if (Fav_Song){
            SortCheck(Fav_Song);
        }
        // Automatically add song to underfilled playlist
        else if (!IsFull() || SONGS->GetPlaycount(id) > GetLowCount()){
            AddSong(SONGS->GetSong(id));
        }
    }
    // add song if favorites is empty.
    else {
        AddSong(SONGS->GetSong(id));
    }
}

void SDLList::Clear(){
    cout << "Clearing the Favorites list and all song fav_counts" << endl;
    while (FAVDATA->first){
        shared_ptr<Fsong> curr_node (FAVDATA->first);
        RemoveSong(curr_node->songfile->id);
    }
    SONGS->ResetFavCounts();
}

//===================================================//
//                                                   // 
//   PRINT METHODS BELOW - PRIMARILY FOR SIMULATION  //
//                                                   //
//===================================================//


void SDLList::PrintFSong(shared_ptr<Fsong> Fsong) {
    cout << Fsong->songfile->name;
}

void SDLList::PrintPlaylist(){
    if (!FAVDATA->first) {
        cout << "<Favorites Playlist is Empty!>" << endl;
    } 
    else {
        cout << "FAVORITES: ";
        shared_ptr<Fsong> curr_node (FAVDATA->first);
        int song_num =1;
        while (curr_node) {
            cout << curr_node->songfile->name << " ; ";
            cout << to_string(curr_node->songfile->fav_count);
            cout << " | ";
            curr_node = curr_node->next;
            song_num++;
        }
        cout << "[Songname ; playcount]" << endl;
    }
}

void SDLList::PrintPlaylistReverse(){
    if (!FAVDATA->first) {
        cout << "THE FAVORITES PLAYLIST IS EMPTY" << endl;
    } 
    
    else {
        cout << "REVERSE FAVORITES [Songname_playcount] : ";
        shared_ptr<Fsong> curr_node (FAVDATA->last);
        int song_num = FAVDATA->max_size;
        while (curr_node) {
            cout << curr_node->songfile->name << " ";
            cout << to_string(curr_node->songfile->fav_count);
            cout << " ;  ";
            curr_node = curr_node->prev;
            song_num--;
        }
        cout << endl;
    }
}
