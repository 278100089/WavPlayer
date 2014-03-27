// PLAY EDITOR by Gabriel Gonzalez

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/soundcard.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define MONO 1
#define STEREO 2


unsigned char chunkID[4];
unsigned char format[4];
unsigned char subChunk1ID[4];
unsigned char subChunk2ID[4];
unsigned int chunkSize;
unsigned int subChunk1IDSize;
unsigned int audioFormat;
unsigned int numChannels;
unsigned int sampleRate;
unsigned int byteRate;
unsigned int blockAlign;
unsigned int bitPerSample;
unsigned int wavFileSize;

int main(int argc, char *argv[]){

FILE *fileSound;
int soundCard;

        printf("Reproductor de sonido por Gabriel González\n\n");
        soundCard = open("/dev/dsp", O_WRONLY);


        if( soundCard < 0){
                printf("ERROR to open the sound card! \n");
        }else{
                printf("Sound Card Opened !\n");
        }

        if( fileSound = fopen(argv[1],"r")){
                printf("Wav file ok! \n");
        }
        else{
                printf("Error opening wav file \n");
        }

        printf("Description: \n");

        fseek(fileSound , 0L, SEEK_SET);
        fread(&chunkID,4 , 1 , fileSound);
        printf("RIFF: %s \n", chunkID) ;

        fseek(fileSound , 4L, SEEK_SET);
        fread(&chunkSize , 4, 1 , fileSound);
        printf("CHUNCK SIZE: %d\n", ( chunkSize / 1024 ) / 1024 );

        fseek(fileSound , 8L , SEEK_SET);
        fread(&format, 4, 1, fileSound);
        printf("FILE FORMAT: %s\n",format);

        fseek(fileSound , 12L , SEEK_SET);
        fread(&subChunk1ID , 4, 1 , fileSound);
        printf("FMT ID %s\n",subChunk1ID);

        fseek(fileSound , 16L , SEEK_SET);
        fread(&subChunk1IDSize , 4, 1 , fileSound);
        printf("FMT Size: %d\n",subChunk1IDSize);

        fseek(fileSound , 20L , SEEK_SET);
        fread(&audioFormat , 2, 1 , fileSound);

        if(audioFormat == 1)
                printf("NO HAY COMPRESION\n");
        else
                printf("HAY ALGUN TIPO DE COMPRESION\n");

        fseek(fileSound , 22L , SEEK_SET);
        fread(&numChannels , 2 , 1 , fileSound);


        if(numChannels == 1)
                printf("Play in MONO:\n" );

        else if(numChannels == 2)
                printf("Play in STEREO\n",numChannels);

        fseek(fileSound , 24L , SEEK_SET);
        fread(&sampleRate , 4, 1 , fileSound);
        printf("SampleRate %d\n", sampleRate);

        fseek(fileSound , 28L , SEEK_SET);
        fread(&byteRate , 4 , 1, fileSound);
        printf("SAMPLE RATE: %d\n",byteRate);

        fseek(fileSound , 32L , SEEK_SET);
        fread(&blockAlign , 2 , 1, fileSound);
        printf("BLOCK ALIGN %d\n", blockAlign);

        fseek(fileSound , 34L , SEEK_SET);
        fread(&bitPerSample , 2 , 1, fileSound);
        printf("BITS PER SAMPLE %d\n",bitPerSample);

        fseek(fileSound , 36L , SEEK_SET);
        fread(&subChunk2ID , 4 , 1 , fileSound);
        printf("DATA : %s\n",subChunk2ID);

        fseek(fileSound , 40L , SEEK_SET);
        fread(&wavFileSize , 4 , 1 , fileSound);
        printf("File size %d\n",wavFileSize);

        fseek(fileSound , 44L , SEEK_SET);

        printf("Configurando tarjeta de sonido ...\n");


         if (numChannels == 1){

            if (ioctl(soundCard, SOUND_PCM_WRITE_CHANNELS, MONO) == -1){

                printf("I can't configure channels in MONO\n");

            }else{

                printf("Playing in MONO\n");

            }
        }

        if (numChannels == 2){

              if (ioctl(soundCard, SOUND_PCM_WRITE_CHANNELS, STEREO) == -1)                
		printf("I can't configure channels in  STEREO\n");

              else{
                printf("Playing in STEREO\n");

              }
        }

        if (ioctl(soundCard, SOUND_PCM_WRITE_RATE, &sampleRate) == -1){
              printf("I can't configure sample rate\n");
        }


        if( ioctl(soundCard , SOUND_PCM_WRITE_BITS , &bitPerSample ) == -1){
                printf("I can't send the configuration BITS PER SAMPLE \n");
        }else{
                printf("BITS configurated!\n");
        }


        int sample=0;

         
         while(feof(fileSound) == 0)
         {
            fread(&sample, 1, 1, fileSound);
            write(soundCard, &sample, 1);
         }

       fclose(fileSound);
       close(soundCard);

     return EXIT_SUCCESS;

} 

