#ifndef TABREADER_H
#define TABREADER_H
#include <stdio.h>

//Data Structure for the Tab
class Tab{
 private:
  //a bunch of attributes
  char* file_version;
  char* title;
  char* subtitle;
  char* interpret;
  char* album;
  char* author;
  char* copyright;
  char* tab_author;
  char* instructional;
  char** notices;
  char** lyrics;
  int triplet_feel;

  int tempo;
  int key;
  int octave;
  int channels;
  int measures;
  int tracks;
  //will have to find a suitable data structure to store the information pertaining to the music
  
 public:
  //empty constructor
  Tab(){};
  //if given a filename, load the tab
  Tab(const char* filename){load_tab(filename);};
  //function to load the tab
  void load_tab(const char* filename);
};


//functions for reading in a gp file:
//reads a string
void gp_read_string(FILE*, char**);
//reads a lyric string
void gp_read_lyrics_string(FILE*, char**, int);
//reads a null string (single character)
void gp_read_null_string(FILE*);
//reads an int
int gp_read_int(FILE*);
//reads a header block
void gp_read_header_info(FILE*, char**);


#endif
