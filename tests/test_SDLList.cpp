#include <gtest/gtest.h>

#define private public
#include "../code/SDLList.h"

using namespace std;

#include <fstream>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

class test_SDLList : public ::testing::Test {
protected:

	// this function runs before every TEST_F function
	void SetUp() override {}

	// this function runs after ever TEST_F function
	void TearDown() override {}
	
};

// Test cases

TEST_F(test_SDLList,TestInit) {
	// PUBLIC MEMBER FUNCTIONALITY:
    SDLList FooList(10);
    ASSERT_FALSE(FooList.IsFull());
    ASSERT_TRUE(FooList.IsEmpty());

	SongCatalog FooCatalog;
	ASSERT_EQ(FooCatalog.GetSize(), 0);
	FooCatalog.InitSong("foo1");
	ASSERT_EQ(FooCatalog.GetSize(), 1);
	FooCatalog.InitSong("foo2");
	ASSERT_EQ(FooCatalog.GetSize(), 2);
	FooCatalog.InitSong("foo3");
	ASSERT_EQ(FooCatalog.GetSize(), 3);

	// PRIVATE MEMBER/VARIABLE CHECK:
	shared_ptr<Song> foo_song;
	foo_song = FooCatalog.Catalog[0];
	ASSERT_EQ(foo_song->name, "foo1");
	foo_song = FooCatalog.Catalog[1];
	ASSERT_EQ(foo_song->name, "foo2");
	foo_song = FooCatalog.Catalog[2];
	ASSERT_EQ(foo_song->name, "foo3");

	shared_ptr<Fsong> foo_fsong (FooList.InitFSong());
	ASSERT_EQ(foo_fsong->next, nullptr);
	ASSERT_EQ(foo_fsong->prev, nullptr);
	ASSERT_EQ(foo_fsong->omitted, false);
	ASSERT_EQ(foo_fsong->songfile, nullptr);
}


TEST_F(test_SDLList, TestAddSong) {
	// PRIVATE MEMBER/VARIABLE CHECK:
	// There is no public member functionality for AddSong()
	// because additions are based on fav_count in SongCatalog, 
	// which is incremented each time a song is played. 
	SDLList FooList(3);
	SongCatalog FooCatalog;
	FooList.SetCatalog(&FooCatalog);
	FooCatalog.InitSong("foo1");
	FooCatalog.InitSong("foo2");
	FooCatalog.InitSong("foo3");
	FooCatalog.InitSong("foo4");
	FooCatalog.InitSong("foo5");

	ASSERT_TRUE(FooList.IsEmpty());

	FooList.AddSong(FooCatalog.Catalog[0]);
	ASSERT_EQ(FooList.FAVDATA->first->songfile->name,"foo1");
	ASSERT_EQ(FooList.FAVDATA->last->songfile->name,"foo1");
	ASSERT_FALSE(FooList.IsFull());

	FooList.AddSong(FooCatalog.Catalog[1]);
	ASSERT_EQ(FooList.FAVDATA->first->songfile->name,"foo1");
	ASSERT_EQ(FooList.FAVDATA->first->next->songfile->name, "foo2" );
	ASSERT_EQ(FooList.FAVDATA->last->songfile->name,"foo2");
	ASSERT_FALSE(FooList.IsFull());
	
	FooList.AddSong(FooCatalog.Catalog[2]);
	ASSERT_EQ(FooList.FAVDATA->first->songfile->name,"foo1");
	ASSERT_EQ(FooList.FAVDATA->first->next->next->songfile->name, "foo3" );
	ASSERT_EQ(FooList.FAVDATA->last->songfile->name,"foo3");
	ASSERT_TRUE(FooList.IsFull()); // should be full now

	// should not happen given playcounts, but we support it
	FooList.AddSong(FooCatalog.Catalog[3]);
	ASSERT_EQ(FooList.FAVDATA->first->songfile->name,"foo1");
	ASSERT_EQ(FooList.FAVDATA->first->next->next->songfile->name, "foo4" );
	ASSERT_EQ(FooList.FAVDATA->last->songfile->name,"foo4");
	ASSERT_TRUE(FooList.IsFull());

	FooList.AddSong(FooCatalog.Catalog[4]);
	ASSERT_EQ(FooList.FAVDATA->first->songfile->name,"foo1");
	ASSERT_EQ(FooList.FAVDATA->first->next->next->songfile->name, "foo5" );
	ASSERT_EQ(FooList.FAVDATA->last->songfile->name,"foo5");
	ASSERT_TRUE(FooList.IsFull());

	// Sort() should be run as part of AddSong() in order to account for edge
	// cases where a song is added and the song it replaces has the same fav_count
	// as it's predecessor(s), requiring the song we are adding be moved up in the list. 
	// We do not test for that here. Whether this happens is shown in the app simulation. 
}

TEST_F(test_SDLList, TestFind) {

	SDLList FooList(3);
	SongCatalog FooCatalog;
	FooList.SetCatalog(&FooCatalog);
	FooCatalog.InitSong("foo1");
	FooCatalog.InitSong("foo2");
	FooCatalog.InitSong("foo3");

	FooList.AddSong(FooCatalog.Catalog[0]);
	FooList.AddSong(FooCatalog.Catalog[1]);
	FooList.AddSong(FooCatalog.Catalog[2]);

	ASSERT_TRUE(FooList.Find(0));
	ASSERT_TRUE(FooList.Find(1));
	ASSERT_TRUE(FooList.Find(2));
	ASSERT_FALSE(FooList.Find(3));
	ASSERT_FALSE(FooList.Find(4));

}

TEST_F(test_SDLList, TestSorting) {
	// SORTING OCCURS INTERNALLY SO WE TEST WITH INTERNAL MEMBERS & DATA:
	SDLList FooList(5);
	SongCatalog FooCatalog;
	FooList.SetCatalog(&FooCatalog);
	FooCatalog.InitSong("foo1");
	FooCatalog.InitSong("foo2");
	FooCatalog.InitSong("foo3");
	FooCatalog.InitSong("foo4");
	FooCatalog.InitSong("foo5");

	FooList.AddSong(FooCatalog.Catalog[0]);
	FooList.AddSong(FooCatalog.Catalog[1]);
	FooList.AddSong(FooCatalog.Catalog[2]);
	FooList.AddSong(FooCatalog.Catalog[3]);
	FooList.AddSong(FooCatalog.Catalog[4]);

	// Sanity check:
	// Start order should be: [0, 1, 2, 3, 4]
	ASSERT_EQ(FooList.FAVDATA->first->songfile, FooCatalog.Catalog[0]);
	ASSERT_EQ(FooList.FAVDATA->first->next->songfile, FooCatalog.Catalog[1]);
	ASSERT_EQ(FooList.FAVDATA->first->next->next->songfile, FooCatalog.Catalog[2]);
	ASSERT_EQ(FooList.FAVDATA->first->next->next->next->songfile, FooCatalog.Catalog[3]);
	ASSERT_EQ(FooList.FAVDATA->first->next->next->next->next->songfile, FooCatalog.Catalog[4]);

	shared_ptr<Song> foo_song;
	
	foo_song = FooCatalog.Catalog[0];
	foo_song->fav_count = 2;
	FooList.Sort(); // should not change order
	ASSERT_EQ(FooList.FAVDATA->first->songfile, FooCatalog.Catalog[0]);

	foo_song = FooCatalog.Catalog[2];
	foo_song->fav_count = 2; // should move up one spot, but not to front
	FooList.Sort();
	// New order should be: [0, 2, 1, 3, 4]
	// check from front to make sure link between swapped songs is still valid
	ASSERT_EQ(FooList.FAVDATA->first->next->next->songfile, FooCatalog.Catalog[1]);
	// check from back to make sure link between swapped songs is still valid
	ASSERT_EQ(FooList.FAVDATA->last->prev->prev->prev->songfile, FooCatalog.Catalog[2]);

	foo_song = FooCatalog.Catalog[4];
	foo_song->fav_count = 4; // should move to front. Should have new front and new last.
	FooList.Sort();
	// New order should be: [4, 0, 2, 1, 3]
	ASSERT_EQ(FooList.FAVDATA->first->songfile, FooCatalog.Catalog[4]);
	ASSERT_EQ(FooList.FAVDATA->last->songfile, FooCatalog.Catalog[3]);

	foo_song = FooCatalog.Catalog[3];
	foo_song->fav_count = 3; // should move up three spots. Should have new last in list. 
	FooList.Sort();
	// New order should be: [4, 3, 0, 2, 1]
	ASSERT_EQ(FooList.FAVDATA->first->next->songfile, FooCatalog.Catalog[3]);
	ASSERT_EQ(FooList.FAVDATA->last->songfile, FooCatalog.Catalog[1]);
}

TEST_F(test_SDLList, TestRemoveSong) {
	// 
	SDLList FooList(5);
	SongCatalog FooCatalog;
	FooList.SetCatalog(&FooCatalog);
	FooCatalog.InitSong("foo1");
	FooCatalog.InitSong("foo2");
	FooCatalog.InitSong("foo3");
	FooCatalog.InitSong("foo4");
	FooCatalog.InitSong("foo5");

	FooList.AddSong(FooCatalog.Catalog[0]);
	FooList.AddSong(FooCatalog.Catalog[1]);
	FooList.AddSong(FooCatalog.Catalog[2]);
	FooList.AddSong(FooCatalog.Catalog[3]);
	FooList.AddSong(FooCatalog.Catalog[4]);

	FooList.RemoveSong(2);
	ASSERT_FALSE(FooList.Find(2));

	FooList.RemoveSong(0);
	ASSERT_FALSE(FooList.Find(0));
	ASSERT_EQ(FooList.FAVDATA->first->songfile, FooCatalog.Catalog[1]);

	FooList.RemoveSong(4);
	ASSERT_FALSE(FooList.Find(4));
	ASSERT_EQ(FooList.FAVDATA->last->songfile, FooCatalog.Catalog[3]);
}

TEST_F(test_SDLList, TestClear) {
	// Verifying public member functionality:
	SDLList FooList(5);
	SongCatalog FooCatalog;
	FooList.SetCatalog(&FooCatalog);
	FooCatalog.InitSong("foo1");
	FooCatalog.InitSong("foo2");
	FooCatalog.InitSong("foo3");
	FooCatalog.InitSong("foo4");
	FooCatalog.InitSong("foo5");

	FooList.AddSong(FooCatalog.Catalog[0]);
	FooList.AddSong(FooCatalog.Catalog[1]);
	FooList.AddSong(FooCatalog.Catalog[2]);
	FooList.AddSong(FooCatalog.Catalog[3]);
	FooList.AddSong(FooCatalog.Catalog[4]);

	FooList.Clear();

	ASSERT_FALSE(FooList.FAVDATA->first);
	ASSERT_FALSE(FooList.FAVDATA->last);
	ASSERT_FALSE(FooList.Find(0));
	ASSERT_FALSE(FooList.Find(1));
	ASSERT_FALSE(FooList.Find(2));
	ASSERT_FALSE(FooList.Find(3));
	ASSERT_FALSE(FooList.Find(4));

}

// Update() is the public member function that 
// uses AddSong() and Sort() to ensure 
// playlist stays up-to-date whenever songs in 
// catalog are played and fav_count incremented. 
TEST_F(test_SDLList, TestUpdate) {
	SDLList FooList(5);
	SongCatalog FooCatalog;
	FooList.SetCatalog(&FooCatalog);
	FooCatalog.InitSong("foo1");
	FooCatalog.InitSong("foo2");
	FooCatalog.InitSong("foo3");
	FooCatalog.InitSong("foo4");
	FooCatalog.InitSong("foo5");
	FooCatalog.InitSong("foo6");

	FooCatalog.PlaySong(0);
	FooList.Update(0);
	ASSERT_TRUE(FooList.Find(0));
	ASSERT_FALSE(FooList.IsFull());

	FooCatalog.PlaySong(1);
	FooList.Update(1);
	ASSERT_TRUE(FooList.Find(1));
	ASSERT_FALSE(FooList.IsFull());

	FooCatalog.PlaySong(2);
	FooList.Update(2);
	ASSERT_TRUE(FooList.Find(2));
	ASSERT_FALSE(FooList.IsFull());

	FooCatalog.PlaySong(3);
	FooList.Update(3);
	ASSERT_TRUE(FooList.Find(3));
	ASSERT_FALSE(FooList.IsFull());

	FooCatalog.PlaySong(4);
	FooList.Update(4);
	// Order should be: [0, 1, 2, 3, 4]
	ASSERT_TRUE(FooList.Find(4));
	ASSERT_TRUE(FooList.IsFull());

	FooCatalog.PlaySong(5);
	FooList.Update(5);
	// Order should not have changed
	ASSERT_FALSE(FooList.Find(5));

	FooCatalog.PlaySong(5);
	FooList.Update(5);
	// Order should now be: [5, 0, 1, 2, 3]
	ASSERT_TRUE(FooList.Find(5));
	ASSERT_FALSE(FooList.Find(4)); // This should have fallen off
	ASSERT_EQ(FooList.FAVDATA->first->songfile, FooCatalog.Catalog[5]);

	FooCatalog.PlaySong(4);
	FooList.Update(4);
	// New order should be: [5, 4, 0, 1, 2]
	ASSERT_EQ(FooList.FAVDATA->first->next->songfile, FooCatalog.Catalog[4]);
	ASSERT_FALSE(FooList.Find(3));
	ASSERT_EQ(FooList.FAVDATA->last->songfile, FooCatalog.Catalog[2]);


	FooCatalog.PlaySong(1);
	FooList.Update(1);
	// New order should be: [5, 4, 1, 0, 2]
	// check from front to make sure link still works between swapped songs:
	ASSERT_EQ(FooList.FAVDATA->first->next->next->next->songfile, FooCatalog.Catalog[0]);
	// check from back to make sure link still works between swapped songs:
	ASSERT_EQ(FooList.FAVDATA->last->prev->prev->songfile, FooCatalog.Catalog[1]);

	FooCatalog.PlaySong(2);
	FooList.Update(2);
	FooCatalog.PlaySong(2);
	FooList.Update(2);
	// New order should be: [2, 5, 4, 1, 0]
	ASSERT_EQ(FooList.FAVDATA->first->songfile, FooCatalog.Catalog[2]);
	ASSERT_EQ(FooList.FAVDATA->last->songfile, FooCatalog.Catalog[0]);

	// Check fav_count of each song in favorites list
	shared_ptr<Song> foo_song;
	foo_song = FooCatalog.Catalog[2];
	ASSERT_EQ(foo_song->fav_count, 3);
	foo_song = FooCatalog.Catalog[5];
	ASSERT_EQ(foo_song->fav_count, 2);
	foo_song = FooCatalog.Catalog[4];
	ASSERT_EQ(foo_song->fav_count, 2);
	foo_song = FooCatalog.Catalog[1];
	ASSERT_EQ(foo_song->fav_count, 2);
	foo_song = FooCatalog.Catalog[0];
	ASSERT_EQ(foo_song->fav_count, 1);
}
