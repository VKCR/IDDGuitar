#include <stdio.h>
#include <iostream>
#include <string.h>
#include "TabReaderXML.h"

using namespace std;

int TabReaderXML::parseTagType(LineXML* lineXML){
  //get the type from the first tag
  int index = 0;
  while (lineXML->first_tag[index] != '\0' && lineXML->first_tag[index] != ' ')
    index++;

  char type[200];
  memcpy(type, lineXML->first_tag, index);
  type[index] = '\0';

  //ADD THE TYPE CASES HERE

  if(!strcmp(type,"work-title"))
    return WORK_TITLE;
  if(!strcmp(type,"creator"))
    return CREATOR;
  if(!strcmp(type,"score-part"))
    return SCORE_PART;
  if(!strcmp(type,"instrument-name"))
    return INSTRUMENT_NAME;
  if(!strcmp(type,"part-name"))
    return PART_NAME;
  if(!strcmp(type,"part"))
    return PART;
  if(!strcmp(type,"measure"))
    return MEASURE;
  if(!strcmp(type,"fret"))
    return FRET;
  if(!strcmp(type,"string"))
    return STRING;
  if(!strcmp(type,"duration"))
    return DURATION;
  if(!strcmp(type,"rest/"))
    return REST;
  if(!strcmp(type,"chord/"))
    return CHORD;
  if(!strcmp(type,"/note"))
    return END_NOTE;
  if(!strcmp(type,"sound"))
    return SOUND;
  
  
  return UNDEFINED;
  
}

//not perfect, can only parse the first arg
void TabReaderXML::parseTagArgs(LineXML* lineXML, char* arg_buff, int type_arg){
  //get the type from the first tag
  int index = 0;
  while (lineXML->first_tag[index] != '\0' && lineXML->first_tag[index] != ' ')
    index++;
  //if there is no args
  if (index == strlen(lineXML->first_tag)){
    strcpy(arg_buff, "");
    return;
  }

  int oldindex = index+1;
  
  while (lineXML->first_tag[index] != '=')
    index++;
  
  char typeArg[200];
  memcpy(typeArg, &lineXML->first_tag[oldindex], index-oldindex);
  typeArg[index-oldindex] = '\0';
  

  char dataArg[200];
  index+=2;
  oldindex = index;
  
  
  while (lineXML->first_tag[index] != '\"')
    index++;
  
  memcpy(dataArg, &lineXML->first_tag[oldindex], index-oldindex);
  dataArg[index-oldindex] = '\0';

  //INSERT THE DIFFERENT DATATYPES HERE
  if(!strcmp(typeArg, "id") && type_arg == ID)
    strcpy(arg_buff, dataArg);
  else if(!strcmp(typeArg, "number") && type_arg == NUMBER)
    strcpy(arg_buff, dataArg);
  else if(!strcmp(typeArg, "tempo") && type_arg == TEMPO)
    strcpy(arg_buff, dataArg);
  else
    strcpy(arg_buff, "");

  return;
 
  //HERE I NEED TO FINISH IN CASE THERE ARE OTHER ARGS
  
}

void TabReaderXML::ReadTabXML(const char* filename, Tab* tab)
{
  FILE* file = fopen(filename, "r");
  if (file==NULL){
    cout<<"file reading error"<<endl;
    return;
  }

  char line[200]; //current line buffer from the file
  char arg[200]; //array to get args results
  LineXML lineXML; //current parsed XML line
  int note_fret = 0; //current note info
  int note_string = 0;
  bool note_rest = false;
  int chord_duration = 0; //current chord duration info;
  bool in_chord = false; //if the note is in the current chord
  int tempo = 0;
  //TrackList* currTrack = NULL;
  Track* currTrack = NULL; //pointer to current track
  Measure* currMeasure = NULL; //pointer to current measure
  Chord* currChord = NULL; //pointer to current chord

  //int count = 0;
  
  while (fgets(line,200,file) != NULL) {
    //cout<<line<<endl;
    parseLine(line, &lineXML);
    
    switch(parseTagType(&lineXML)){
    case WORK_TITLE:
      tab->setWorkTitle(lineXML.center);
      break;
      
    case CREATOR:
      tab->setCreator(lineXML.center);
      break;
      
    case SCORE_PART:
      parseTagArgs(&lineXML, arg, ID);
      if(tab->tracks == NULL)
	tab->tracks = new LinkedList<Track>(new Track(arg), NULL);
      else
	tab->tracks->append(new Track(arg));
      break;
      
    case INSTRUMENT_NAME:
      tab->tracks->getLast()->setInstrumentName(lineXML.center);
      break;
      
    case PART_NAME:
      tab->tracks->getLast()->setPartName(lineXML.center);
      break;

    case PART:
      parseTagArgs(&lineXML, arg, ID);
      currTrack = tab->tracks->get(arg);
      break;

    case MEASURE:
      parseTagArgs(&lineXML, arg, NUMBER);
      if(currTrack->measures == NULL){
	currTrack->measures = new LinkedList<Measure>(new Measure(atoi(arg)), NULL);
	currMeasure = currTrack->measures->getHead();
      }
      else
	currMeasure = currTrack->measures->append(new Measure(atoi(arg)));
      currMeasure->setTempo(tempo); //set the tempo
      break;

    case FRET: //keep info about the last read note fret
      note_fret = atoi(lineXML.center);
      break;

    case STRING: //keep info about the last read note string
      note_string = atoi(lineXML.center);
      break;

    case DURATION: //keep info about the last read note duration
      chord_duration = atoi(lineXML.center);
      break;

    case REST: //trigger the rest flag
      note_rest = true;
      break;
     
    case CHORD: //trigger the chord flag
      in_chord = true;
      break;
      
    case END_NOTE: //add note to corresponding chord, depending on the chord flag
      if(!in_chord){ //if it's in a new chord
	if (currMeasure->chords == NULL){
	  currMeasure->chords = new LinkedList<Chord>(new Chord(chord_duration), NULL);
	  currChord = currMeasure->chords->getHead();
	}
	else
	  currChord = currMeasure->chords->append(new Chord(chord_duration));
      } 
      //add the note
      if(currChord->notes == NULL)
	currChord->notes = new LinkedList<Note>(new Note(note_fret,note_string,note_rest),NULL);
      else
	currChord->notes->append(new Note(note_fret,note_string,note_rest));
      
      //reset the flags and note values
      note_fret = 0;
      note_string = 0;
      note_rest = false;
      chord_duration = 0;
      in_chord = false;
      break;

    case SOUND:
      parseTagArgs(&lineXML, arg, TEMPO);
      tempo = atoi(arg);
      currMeasure->setTempo(tempo);
      break;
      
    case UNDEFINED:
      break;
      }
      
  }
  fclose(file);
  return;
}

void TabReaderXML::parseLine(char* line, LineXML* lineXML){
  int index = 0;
  lineXML->first_tag[0] = '\0';
  lineXML->second_tag[0] = '\0';
  lineXML->center[0] = '\0';
  lineXML->nelems = 1;
  
  while (line[index] != '>')
    index++;
  
  memcpy(lineXML->first_tag, &line[1], index-1);
  lineXML->first_tag[index-1] = '\0';
  
  //if we reach the end of the line
  if(index + 2 == strlen(line))
    return;
  
  int oldindex = index + 1;
  
  while (line[index] != '<')
    index++;

  lineXML->nelems = 3;
  
  memcpy(lineXML->center, &line[oldindex], index-oldindex);
  lineXML->center[index-oldindex] = '\0';
  
  //copy the rest
  memcpy(lineXML->second_tag, &line[index + 1], strlen(line)-index - 3);
  lineXML->second_tag[strlen(line)-index-3] = '\0';
}
