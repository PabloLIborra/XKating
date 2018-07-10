#include <MidiFile.h>
#include <Options.h>
#include <iostream>
#include <vector>
#include <stdio.h>

struct note {
    uint8_t tone;
    uint8_t duration;
};

struct track {
    uint8_t channel;
    uint8_t program;
    uint8_t maxTone = 0;
    uint8_t minTone = 255;
    float   probability = 0.0;
    std::vector<note> notes;
};

int main(int argc, char** argv) {

    if(argc < 2){
        std::cerr << "Error: missing arguments" << std::endl;
        return 0;
    }
    
    const char* m = argv[1];
    
    //Open and read midi file
    smf::MidiFile midifile;
    midifile.read(m);
    
    std::cout << "Reading " << argv[1] << "..." << std::endl;
    //Prepare note duration to be read in seconds
    midifile.doTimeAnalysis();
    midifile.linkNotePairs();

    std::vector<track*> tracks;

    unsigned int noteSizeCounter = 0;

    //===================================================================================
    //  PARSING MIDI
    //===================================================================================
    int trackCount = midifile.getTrackCount();
    //For every track in the midi, read them
    for (int i=0; i<trackCount; i++) {

        track* newTrack = nullptr;

        for (int event=0; event<midifile[i].size(); event++) {

            //If the track is empty and we receive a valid message
            if(newTrack == nullptr && (midifile[i][event].isNoteOn() || midifile[i][event].isTimbre())){
                newTrack = new track;
            }

            //Get channel and program of the track
            if(midifile[i][event].isTimbre()) {
                newTrack->program = midifile[i][event][1];
                newTrack->channel = midifile[i][event].getChannelNibble();
            }

            //Add note to the track
            if(midifile[i][event].isNoteOn() && midifile[i][event].getDurationInSeconds() > 0) {
                double dur = midifile[i][event].getDurationInSeconds(); 
                
                if(dur > 8)
                    dur = 8.0;
                else if (dur < 0.03125)
                    dur = 0.032;

                //Convert the duration value to a number from 0 to 255, every unit being 0.03125 seconds
                uint8_t duration = ((((int)dur)*8)) + (int)((((float)dur)-((int)dur))*32);
                uint8_t tone     = midifile[i][event][1];
                newTrack->notes.push_back(note {tone, duration});

                //Update min and max tone
                if(newTrack->minTone > tone)
                    newTrack->minTone = tone;
                if(newTrack->maxTone < tone)
                    newTrack->maxTone = tone;
            }    
        }

        if(newTrack != nullptr && newTrack->notes.size() > 100 && newTrack->channel != 9) {
            //If track is not empty or has few notes, add track
            tracks.push_back(newTrack);

            //Add probability of the track for being played
            if(newTrack->notes.size() > noteSizeCounter)
                noteSizeCounter = newTrack->notes.size();
            newTrack->probability = newTrack->notes.size();
        }
        else if (newTrack != nullptr && (newTrack->notes.size() <= 100 || newTrack->channel == 9)) {
            delete newTrack;
        }
    }

    //Calculate probability of the track to be played
    for (int i=0; i < tracks.size(); i++) {
        tracks[i]->probability = tracks[i]->probability/(float)noteSizeCounter;
    }

    //===================================================================================
    //  CALCULATING PROBABILITIES
    //===================================================================================
    std::cout << "Calculating probabilities..." << std::endl;

    //A probability counter is a 3dimensional array being
    //  [lastNotePlayed][nextNoteProbability][tone and duration probability]
    //  the 3rd part of the array is and array containing the tone to be played and the probabilities of the
    //  duration of that tone
    std::vector<int***> probabilityCounter;
    std::vector<float***> probabilities;

    //Initialize probability counters
    for(int i = 0; i < tracks.size(); i++) {

        int noteSize = tracks[i]->maxTone - tracks[i]->minTone + 1;
        int*** prC = new int**[noteSize];

        float*** prB = new float**[noteSize];

        for(int j = 0; j < noteSize; j++) {

            prC[j] = new int*[noteSize];

            prB[j] = new float*[noteSize];

            for(int k = 0; k < noteSize; k++) {
                //Initialize counter to 9, [0] for the note tone and [1-8] for duration probability
                prC[j][k] = new int [9];

                prB[j][k] = new float [9];

                for(int l = 0; l < 9; l++) {

                    //Initial count of notes and durations ocurrences
                    if(l == 0)
                        prC[j][k][l] = 0;
                    else 
                        prC[j][k][l] = 1;
                }

                for(int l = 0; l < 9; l++)
                    prB[j][k][l] = 0.0;
            }
        }
        probabilityCounter.push_back(prC);
        probabilities.push_back(prB);
    }
    
    //Go through all the notes and update counters
    for(int i = 0; i < tracks.size(); i++) {

        int noteSize = tracks[i]->maxTone - tracks[i]->minTone + 1;
        int minTone = tracks[i]->minTone;
        uint8_t lastNote = tracks[i]->notes[0].tone;

        for(int j = 1; j < tracks[i]->notes.size(); j++) {

            uint8_t duration = tracks[i]->notes[j].duration;

            int index;

            if      (duration > 200)    index = 8;
            else if (duration > 100)    index = 7;
            else if (duration >  50)    index = 6;
            else if (duration >  25)    index = 5;
            else if (duration >  12)    index = 4;
            else if (duration >   6)    index = 3;
            else if (duration >   3)    index = 2;
            else                        index = 1;

            probabilityCounter[i][lastNote-minTone][tracks[i]->notes[j].tone-minTone][0]++;         //Update tone counter
            probabilityCounter[i][lastNote-minTone][tracks[i]->notes[j].tone-minTone][index]++;     //Update duration counter
            
            lastNote = tracks[i]->notes[j].tone;
        }
    }

    //Go through all the counters and calculate probabilities
    for(int i = 0; i < tracks.size(); i++) {

        int noteSize = tracks[i]->maxTone - tracks[i]->minTone + 1;

        for(int j = 0; j < noteSize; j++) {

            //Update note probability
            int noteCounter = 0;

            for(int k = 0; k < noteSize; k++) {

                //Update duration probability
                int durCounter = 0;

                //Count given cases
                for (int l = 1; l < 9; l++)
                    durCounter += probabilityCounter[i][j][k][l];

                if(durCounter < 1) durCounter = 1;

                //Give probabilities to every possible case
                for (int l = 1; l < 9; l++)
                    probabilities[i][j][k][l] = (float)probabilityCounter[i][j][k][l]/(float)durCounter;

                //Update noteCounter
                noteCounter += probabilityCounter[i][j][k][0];

            }
            //Update notes probability
            if(noteCounter < 1) noteCounter = 1;

            for(int k = 0; k < noteSize; k++) {
                probabilities[i][j][k][0] = (float)probabilityCounter[i][j][k][0]/(float)noteCounter;
            }
        }
    }

    //Output track proabilities
    //for (int i = 0; i < tracks[0]->maxTone - tracks[0]->minTone + 1; i++) {
    //    for(int j = 0; j < tracks[0]->maxTone - tracks[0]->minTone + 1; j++) {
    //        
    //        if(probabilities[0][i][j][0] < 10) {
    //            std::cout << " " << probabilities[0][i][j][0] << " ";
    //        }
    //        else {
    //            std::cout << probabilities[0][i][j][0] << " ";
    //        }
    //    }
    //    std::cout << std::endl;
    //}

    //===================================================================================
    //  WRITING TO BINARY FILE
    //===================================================================================
    std::cout << "Writing binary file..." << std::endl;

    FILE* pFile;
    pFile = fopen("probabilities.bin", "wb");

    //Write number of tracks
    int size = tracks.size();
    fwrite(&size, sizeof(int), 1, pFile);

    for (int i = 0; i < tracks.size(); i++){

        int channel = (int)tracks[i]->channel;
        int program = (int)tracks[i]->program;
        int maxNote = (int)tracks[i]->maxTone;
        int minNote = (int)tracks[i]->minTone;
        int noteSize = tracks[i]->maxTone - tracks[i]->minTone + 1;
        float probTrack = tracks[i]->probability;
        
        //Write per track header
        fwrite(&channel, sizeof(int), 1, pFile);
        fwrite(&program, sizeof(int), 1, pFile);
        fwrite(&maxNote, sizeof(int), 1, pFile);
        fwrite(&minNote, sizeof(int), 1, pFile);
        fwrite(&noteSize, sizeof(int), 1, pFile);
        fwrite(&probTrack, sizeof(float), 1, pFile);

        //Write probabilities
        for(int j = 0; j < noteSize; j++) 
            for(int k = 0; k < noteSize; k++)
                fwrite(probabilities[i][j][k], sizeof(float), 9, pFile);
    }
    fclose(pFile);

    std::cout << "Done!" << std::endl;

    //Delete all the data we've used
    for(int i = 0; i < tracks.size(); i++) {

        int noteSize = tracks[i]->maxTone - tracks[i]->minTone + 1;

        for(int j = 0; j < noteSize; j++) {
            for(int k = 0; k < noteSize; k++) {
                delete [] probabilityCounter[i][j][k];
                delete [] probabilities[i][j][k];
            }
            delete [] probabilityCounter[i][j];
            delete [] probabilities[i][j];
        }
        delete [] probabilityCounter[i];
        delete [] probabilities[i];
    }

    return 0;
}
