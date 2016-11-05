#include "TabReader.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

//returns true if nbit is present
bool bit_presence(int byte, int bit){
  byte = byte>> bit;
  return byte & 0b1;
}

//prints byte in binary form
void print_byte(int byte){
  bool bit = 0;
  for (int i = 0; i < 8 ; i++){
    bit = byte & 0b1;
    cout<< bit;
    byte = byte >> 1;
  }
  cout<<endl;
}


///READING PRIMITIVES////

int gp_read_byte_int(FILE* file){
  char read = (char)fgetc(file);
  return (int)read;
}
//reads size of the string and then reads the string
void gp_read_string(FILE* file, char**buf){
  int n = gp_read_byte_int(file);
  *buf = new char[n];
  fgets(*buf, n+1, file);
}

void gp_read_lyrics_string(FILE* file, char**buf, int len){
  *buf = new char[(int)len];
  fgets(*buf, len+1, file);
}

//reads single character and throws it away
void gp_read_null_string(FILE* file){
  fgetc(file);
}

//reads 4 characters and constructs the int
int gp_read_int(FILE* file){
  char read;
  int result = 0;
  for (int i=0; i<4; i++){
    result<<8;
    read = (char)fgetc(file);
    result += (int)read;
  }
  return result;
}

//reads 2 characters and constructs the short int
int gp_read_short_int(FILE* file){
  char read;
  int result = 0;
  for (int i=0; i<2; i++){
    result<<8;
    read = (char)fgetc(file);
    result += (int)read;
  }
  return result;
}


///READING FUNCTIONS////

void gp_read_chord_diagram(FILE* file, chord_diagram* diag){
  diag->header = gp_read_byte_int(file);
  diag->sharp = gp_read_byte_int(file);

  gp_read_byte_int(file); //blank bytes
  gp_read_byte_int(file);
  gp_read_byte_int(file);
  
  diag->root = gp_read_byte_int(file);
  diag->major_minor = gp_read_byte_int(file);
  diag->nine_eleven_thirteen = gp_read_byte_int(file);
  diag->bass = gp_read_int(file);
  diag->diminished_augmented = gp_read_int(file);
  diag->add = gp_read_byte_int(file);
  //gp_read_string(file, &diag->name);
  gp_read_lyrics_string(file, &diag->name,20);

  gp_read_byte_int(file); //blank bytes
  gp_read_byte_int(file);
  
  diag->fifth = gp_read_byte_int(file);
  diag->ninth = gp_read_byte_int(file);
  diag->eleventh = gp_read_byte_int(file);
  diag->base_fret = gp_read_int(file);

  for (int i=0; i<7; i++)
    diag->frets[i] = gp_read_int(file);
  
  diag->number_of_barres = gp_read_byte_int(file);
  for (int i=0; i<5; i++)
    diag->fret_of_barre[i] = gp_read_byte_int(file);

  for (int i=0; i<5; i++)
    diag->barre_start[i] = gp_read_byte_int(file);

  for (int i=0; i<5; i++)
    diag->barre_end[i] = gp_read_byte_int(file);
  
  diag->omission1 = gp_read_byte_int(file);
  diag->omission3 = gp_read_byte_int(file);
  diag->omission5 = gp_read_byte_int(file);
  diag->omission7 = gp_read_byte_int(file);
  diag->omission9 = gp_read_byte_int(file);
  diag->omission11 = gp_read_byte_int(file);
  diag->omission13 = gp_read_byte_int(file);

  gp_read_byte_int(file); //blank byte

  for (int i=0; i<7; i++)
    diag->fingering[i] = gp_read_byte_int(file);

  diag->show_diag_fingering = gp_read_byte_int(file);
  
}

void gp_read_bend(FILE* file, bend* b){
  b->type = gp_read_byte_int(file);
  b->value = gp_read_int(file);
  b->npoints = gp_read_int(file);
  b->listpoints = new point[b->npoints];
  for (int i = 0 ; i<b->npoints; i++){
    b->listpoints[i].absolute_time_position = gp_read_int(file);
    b->listpoints[i].vertical_position = gp_read_int(file);
    b->listpoints[i].vibrato = gp_read_byte_int(file);
  }
}

void gp_read_beat_effect(FILE* file, effect_beat* be){
  be->first_byte = gp_read_byte_int(file);
  be->second_byte = gp_read_byte_int(file);

  if(bit_presence(be->first_byte,5))
    be->tapping_slapping_popping = gp_read_byte_int(file);
  if(bit_presence(be->first_byte,2))
    gp_read_bend(file, &be->tremolo_bar);
  if(bit_presence(be->first_byte,6))
    be->upstroke = gp_read_byte_int(file);
  if(bit_presence(be->first_byte,6)) //STRANGE
    be->downstroke = gp_read_byte_int(file);
  if(bit_presence(be->second_byte,1)) 
    be->pickstroke = gp_read_byte_int(file);
}

void gp_read_mix_table_change_event(FILE* file, mix_table_change_event* mtce){
  mtce->instrument = gp_read_byte_int(file);
  mtce->volume = gp_read_byte_int(file);
  mtce->pan = gp_read_byte_int(file);
  mtce->chorus = gp_read_byte_int(file);
  mtce->reverb = gp_read_byte_int(file);
  mtce->phaser = gp_read_byte_int(file);
  mtce->tremolo = gp_read_byte_int(file);
  mtce->tempo = gp_read_int(file);
  mtce->volume_change_duration = gp_read_byte_int(file);
  mtce->pan_change_duration = gp_read_byte_int(file);
  mtce->chorus_change_duration = gp_read_byte_int(file);
  mtce->reverb_change_duration = gp_read_byte_int(file);
  mtce->phaser_change_duration = gp_read_byte_int(file);
  mtce->tremolo_change_duration = gp_read_byte_int(file);
  mtce->tempo_change_duration = gp_read_byte_int(file);
  mtce->change_header = gp_read_byte_int(file);
}

void gp_read_grace_note(FILE* file, grace_note* gn){
  gn->fret = gp_read_byte_int(file);
  gn->dynamic = gp_read_byte_int(file);
  gn->transition = gp_read_byte_int(file);
  gn->duration = gp_read_byte_int(file);
}

void gp_read_note_effect(FILE* file, effect_note* en){
  en->first_byte = gp_read_byte_int(file);
  en->second_byte = gp_read_byte_int(file);
  if(bit_presence(en->first_byte,0))
    gp_read_bend(file, &en->note_bend);
  if(bit_presence(en->first_byte,4))
    gp_read_grace_note(file, &en->grace);
  if(bit_presence(en->second_byte,2))
    en->tremolo_picking = gp_read_byte_int(file);
  if(bit_presence(en->second_byte,3))
    en->slide = gp_read_byte_int(file);
  if(bit_presence(en->second_byte,4))
    en->harmonics = gp_read_byte_int(file);
  if(bit_presence(en->second_byte,5)){
    en->trill_fret = gp_read_byte_int(file);
    en->trill_period = gp_read_byte_int(file);
  }
}



void gp_read_note(FILE* file, note* n){
  n->header = gp_read_byte_int(file);

  if(bit_presence(n->header,5)) 
    n->note_type = gp_read_short_int(file);
  if(bit_presence(n->header,0)){
    n->note_time_duration = gp_read_byte_int(file);
    n->note_ntuplet = gp_read_byte_int(file);
  }
  if(bit_presence(n->header,4)) 
    n->note_dynamic = gp_read_byte_int(file);
  if(bit_presence(n->header,5)) 
    n->fret_number = gp_read_byte_int(file);
  if(bit_presence(n->header,7)){
    n->fingering_first = gp_read_byte_int(file);
    n->fingering_second = gp_read_byte_int(file);
  }
  if(bit_presence(n->header,3)) 
    gp_read_note_effect(file, &n->effects_on_note); 
}




//reads an int, and depending on its value reads the string or reads a null string.
void gp_read_header_info(FILE* file, char**buf){
  if(gp_read_int(file)!=1){
    gp_read_string(file, buf);
  }
  else
    gp_read_null_string(file);
}

void Tab::load_tab(const char* filename){
  //open the file and check that it opened successfully
  FILE* file = fopen(filename, "r");
  if (file==NULL){
    cout<<"file reading error"<<endl;
    return;
  }
  
  /***** Read the header *****/
  
  //read file version, fixed number of character
  gp_read_byte_int(file); //throw away 
  fheader.file_version = new char[30]; //still set size to 30 because we read 30 characters
  fgets(fheader.file_version, 31, file);

  //read header info
  gp_read_header_info(file, &fheader.title);
  gp_read_header_info(file, &fheader.subtitle);
  gp_read_header_info(file, &fheader.interpret);
  gp_read_header_info(file, &fheader.album);
  gp_read_header_info(file, &fheader.author);
  gp_read_header_info(file, &fheader.copyright);
  gp_read_header_info(file, &fheader.tab_author);
  gp_read_header_info(file, &fheader.instructional);

  //read number of notices
  int n = gp_read_int(file);
  //create the notices and fill in the data
  fheader.notices = new char*[n];
  for (int i=0; i<n; i++)
    gp_read_header_info(file, &fheader.notices[i]);

  //read the triplet_feel
  fheader.triplet_feel = gp_read_byte_int(file);
 
  //read the lyrics
  gp_read_int(file); //number of the track corresponding to the lyrics. Don't care
  fheader.lyrics = new char*[5];
  
  for (int i=0;i<5;i++){
    n = gp_read_int(file);
    n = gp_read_int(file);
    gp_read_lyrics_string(file, &fheader.lyrics[i], n);
  }
  

  fheader.tempo = gp_read_int(file);
  fheader.key = gp_read_byte_int(file);
  fheader.octave = gp_read_byte_int(file);
  
  
  for (int i = 0; i<4 ; i++){
    for (int j = 0; j<16 ; j++){
      fheader.midi_channels[i][j].instrument = gp_read_int(file);
      fheader.midi_channels[i][j].volume = gp_read_byte_int(file);
      fheader.midi_channels[i][j].balance = gp_read_byte_int(file);
      fheader.midi_channels[i][j].chorus = gp_read_byte_int(file);
      fheader.midi_channels[i][j].reverb = gp_read_byte_int(file);
      fheader.midi_channels[i][j].phaser = gp_read_byte_int(file);
      fheader.midi_channels[i][j].tremolo = gp_read_byte_int(file);
      gp_read_byte_int(file); //throw away for backwards compatibility
      gp_read_byte_int(file);
    }
  }

  fheader.measures = gp_read_int(file);
  fheader.tracks = gp_read_int(file);

  //initialize the list of measures
  measures = new measure[fheader.measures];
  
  for (int i = 0; i<fheader.measures ; i++){
    measures[i].header = gp_read_byte_int(file);
    if (bit_presence(measures[i].header,0))
      measures[i].num_key = gp_read_byte_int(file);
    if (bit_presence(measures[i].header,1))
      measures[i].denom_key = gp_read_byte_int(file);
    if (bit_presence(measures[i].header,3))
      measures[i].end_repeat = gp_read_byte_int(file);
    if (bit_presence(measures[i].header,4))
      measures[i].alternate = gp_read_byte_int(file);
    if (bit_presence(measures[i].header,5)){
      measures[i].markerlen = gp_read_int(file)-1; //don't know why this is useful
      gp_read_string(file, &measures[i].markername);
      cout<<measures[i].markername<<endl;
      
      //gp_read_lyrics_string(file, &measures[i].markername, measures[i].markerlen);
      measures[i].markercolor = gp_read_int(file);
    }
    if (bit_presence(measures[i].header,6))
      measures[i].tonality = gp_read_byte_int(file);
  }

  //ALL IS GOOD UNTIL HERE

  //TRACK READING IS BUGGY
  
  //tracks
  
  tracks = new track[fheader.tracks];
  cout<<fheader.tracks<<endl;
  
  for (int i = 0; i<fheader.tracks ; i++){
    tracks[i].header = gp_read_byte_int(file);
    print_byte(tracks[i].header);
    int prout = gp_read_int(file); //throw away the int giving the size of the string??
    cout<<prout<<endl;
    gp_read_byte_int(file);
    gp_read_lyrics_string(file, &tracks[i].name, 40);
    //gp_read_string(file, &tracks[i].name);
    //gp_read_string(file, &tracks[i].name);
    cout<<"track name "<<tracks[i].name<<endl;
    tracks[i].nstrings = gp_read_int(file);
    cout<<"nstrings "<<tracks[i].nstrings<<endl;
    for (int j = 0; j<tracks[i].nstrings; j++)
      tracks[i].tuningstrings[j] = gp_read_int(file);
    
    tracks[i].port = gp_read_int(file);
    cout<<"port "<<tracks[i].port<<endl;

    tracks[i].channel = gp_read_int(file);
    cout<<"channel " << tracks[i].channel<<endl;
    tracks[i].channelE = gp_read_int(file);
    cout<<"channelE "<< tracks[i].channelE<<endl;
    tracks[i].nfrets = gp_read_int(file);
    cout<<"nfrets : "<< tracks[i].nfrets<<endl;
    tracks[i].heightcapo = gp_read_int(file);
    cout<<"heightcapo "<< tracks[i].heightcapo<<endl;
    
    tracks[i].trackcolor = gp_read_int(file);

   
  }
  
  //measure track pairs and beats
  beats = new beat*[fheader.measures * fheader.tracks];
  for (int i = 0; i<fheader.measures; i++){
    for (int j = 0; j<fheader.tracks; j++){
      int nbeats = gp_read_int(file);
      int k = i*fheader.tracks + j;
      beats[k] = new beat[nbeats];
      for (int l = 0; l<nbeats; l++){
	beats[k][l].header = gp_read_byte_int(file);
	if(bit_presence(beats[k][l].header,6))
	  beats[k][l].status = gp_read_byte_int(file);
	beats[k][l].beatduration = gp_read_byte_int(file);
	if(bit_presence(beats[k][l].header,5))
	  beats[k][l].ntuplet = gp_read_int(file);
	if(bit_presence(beats[k][l].header,1)){
	  beats[k][l].nchords = gp_read_int(file);
	  beats[k][l].chord_diagrams = new chord_diagram[beats[k][l].nchords];
	  for (int m = 0 ; m<beats[k][l].nchords ; m++){
	    if(m==176)
	      gp_read_chord_diagram(file,&beats[k][l].chord_diagrams[m]);
	    else
	      gp_read_chord_diagram(file,&beats[k][l].chord_diagrams[m]);
	  }
	}
	
	if(bit_presence(beats[k][l].header,2))
	  gp_read_string(file,&beats[k][l].text);

	if(bit_presence(beats[k][l].header,3))
	  gp_read_beat_effect(file,&beats[k][l].eb);
	if(bit_presence(beats[k][l].header,4))
	  gp_read_mix_table_change_event(file,&beats[k][l].mtce);

	gp_read_note(file, &beats[k][l].n);
      }
      cout<<"i : "<<i<<" | j : "<<j<<endl;
    }
  }
  
  
  
  cout<<"done"<<endl;
  //to see what the rest of the file looks like, uncomment
  /*while ( (read = (char)fgetc(file)) != (char)EOF){
    cout<<read<<endl;
    }*/
  fclose(file);
}


int main(int argc, char** argv){
  Tab* tab;
  if (argc > 1)
    tab = new Tab(argv[1]);
  else
    tab = new Tab("eagles.gp4");
  return 0;
}
