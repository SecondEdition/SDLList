#include "SongCatalog.h"
#include <iostream>
#include <memory>

using namespace std;


SongCatalog::SongCatalog() {
    next_id = 0;
}

SongCatalog::~SongCatalog() {
}

void SongCatalog::InitSong(string name){
    shared_ptr<Song> newsong (new Song);
    newsong->id = next_id; 
    next_id++;
    newsong->name = name;
    newsong->fav_count = 0;
    Catalog.push_back(newsong);
}

int SongCatalog::GetSize(){
    return Catalog.size();
}

int SongCatalog::GetPlaycount(int id){
    shared_ptr<Song> song = Catalog.at(id);
    return song->fav_count;
}

shared_ptr<Song> SongCatalog::GetSong(int id){
    return Catalog.at(id);
}

void SongCatalog::PlaySong(int id){
    shared_ptr<Song> song = Catalog.at(id);
    cout << "Playing song.. \"" << song->name << "\"" << endl;
    song->fav_count++;
}

void SongCatalog::ResetFavCounts(){
    shared_ptr<Song> song;
    for (size_t i = 0; i < Catalog.size(); i++) {
        song = Catalog.at(i);
        song->fav_count = 0;
    }
}

void SongCatalog::PrintCatalog(){
    for (size_t i = 0; i < Catalog.size(); i++) {
        shared_ptr<Song> song = Catalog.at(i);
        cout << to_string(song->id) << ": ";
        cout << "count = " << to_string(song->fav_count) << "; ";
        cout << "\"" << song->name << "\"" << endl;
    }
}
