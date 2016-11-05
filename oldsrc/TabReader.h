#ifndef TABREADER_H
#define TABREADER_H
#include <stdio.h>




//Midi channel structure
struct midi_channel{
  int instrument;
  int volume;
  int balance;
  int chorus;
  int reverb;
  int phaser;
  int tremolo;
};

struct measure{
  int header;
  int num_key;
  int denom_key;
  int end_repeat;
  int alternate;
  int markerlen;
  char* markername;
  int markercolor;
  int tonality;
};

struct track{
  int header;
  char* name;
  int nstrings;
  int tuningstrings[7];
  int port;
  int channel;
  int channelE;
  int nfrets;
  int heightcapo;
  int trackcolor;
};


struct chord_diagram{
  int header;
  int sharp;
  int root;
  int major_minor;
  int nine_eleven_thirteen;
  int bass;
  int diminished_augmented;
  int add;
  char* name;
  int fifth;
  int ninth;
  int eleventh;
  int base_fret;
  int frets[7];
  int number_of_barres;
  int fret_of_barre[5];
  int barre_start[5];
  int barre_end[5];
  int omission1,omission3,omission5,omission7,omission9,omission11,omission13;
  int fingering[7];
  int show_diag_fingering;
};

struct point{
  int absolute_time_position;
  int vertical_position;
  int vibrato;
};

struct bend{
  int type;
  int value;
  int npoints;
  point *listpoints; 
};

struct grace_note{
  int fret;
  int dynamic;
  int transition;
  int duration;
};

struct effect_beat{
  int first_byte;
  int second_byte;
  int tapping_slapping_popping;
  bend tremolo_bar;
  int upstroke;
  int downstroke;
  int pickstroke;
};

struct effect_note{
  int first_byte;
  int second_byte;
  bend note_bend;
  grace_note grace;
  int tremolo_picking;
  int slide;
  int harmonics;
  int trill_fret;
  int trill_period;
};



struct mix_table_change_event{
  int instrument;
  int volume;
  int pan;
  int chorus;
  int reverb;
  int phaser;
  int tremolo;
  int tempo;
  int volume_change_duration;
  int pan_change_duration;
  int chorus_change_duration;
  int reverb_change_duration;
  int phaser_change_duration;
  int tremolo_change_duration;
  int tempo_change_duration;
  int change_header;
};

struct note{
  int header;
  int note_type;
  int note_time_duration;
  int note_ntuplet;
  int note_dynamic;
  int fret_number;
  int fingering_first;
  int fingering_second;
  effect_note effects_on_note;
  //int effects_on_note;
};





struct beat{
  int header;
  int status;
  int beatduration;
  int ntuplet;
  int nchords;
  chord_diagram* chord_diagrams;
  char* text;
  effect_beat eb;
  mix_table_change_event mtce;
  note n;
};



struct file_header{
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
  midi_channel midi_channels[4][16];
  int measures;
  int tracks;
};



//Data Structure for the Tab
class Tab{
 private:
  file_header fheader;
  measure* measures;
  track* tracks;
  beat** beats;
  
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
//reads a byte and converts to an int
int gp_read_byte_int(FILE*);
//reads a header block
void gp_read_header_info(FILE*, char**);


#endif
