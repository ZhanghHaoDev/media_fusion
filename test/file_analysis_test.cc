#include "aac_file.h"
#include "wav_file.h"

int main(int argc, char *argv[]){
    wav_file wav("../wav_test.wav");

    aac_file aac("../aac_test.aac");
    return 0;
}