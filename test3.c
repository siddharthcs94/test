#include<stdio.h>
#include<stdio.h>
#include<alsa/asoundlib.h>

int main()
{


int size;
unsigned char *buffer;
char *pcm_name;
unsigned int rate = 8000;
int chan;
int rc;
unsigned int  val;
char check;
int pcmreturn;

//Set PCM stream and handle
snd_pcm_t *pcm_handle;
snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;

//Set HARDWARE parameters
snd_pcm_hw_params_t *hwparams;


//pcm_name = (char *)malloc(10);
pcm_name = "plughw:0,0";


//allocate memory for hardware
if(snd_pcm_hw_params_malloc(&hwparams)<0)
	{

		fprintf(stderr,"No memory allocated for hardware");
		return(-1);
	
	}



if (snd_pcm_open(&pcm_handle, pcm_name, stream, 0) < 0) {
      fprintf(stderr, "Error opening PCM device %s\n", pcm_name);
            return(-1);
	        }

//INITIALIZE HARDWARE WITH CONFIGURATION OF THE SOUNDCARD
	if(snd_pcm_hw_params_any(pcm_handle,hwparams)<0)
		{
			fprintf(stderr,"The hardware device cannot be configured\n");
			return(-1);
		}


//SET FORMAT TO 16 BIT LITTLE ENDIAN
snd_pcm_hw_params_set_format(pcm_handle,hwparams,SND_PCM_FORMAT_S16_LE);


/* Two channels (stereo) */
 snd_pcm_hw_params_set_channels(pcm_handle,hwparams,2);

 
    /* 44100 bits/second sampling rate (CD quality) */
      
 if (snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
       fprintf(stderr, "Error setting access.\n");
             return(-1);
	         }


//SETTING RATE ie SAMPLING FREQUENCY

snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &rate, 0);


//SETTING HARDWARE PARAMETERS

	if(rc=(snd_pcm_hw_params(pcm_handle,hwparams))<0)
		{
			fprintf(stderr,"\nCannot set hardware parameters\n");
			return -1;
		}



//snd_pcm_uframes_t periodsize = 8192;


snd_pcm_uframes_t  frames;
frames =32;


//SETTING SOME MORE PARAMETERS

size = frames*4;
//SETTING BUFFER SIZE
/*
if (snd_pcm_hw_params_set_buffer_size(pcm_handle, hwparams, (periodsize*2)>>2) < 0) {
      fprintf(stderr, "Error setting buffersize.\n");
            return(-1);


}
*/


//DISPLAYING INFORMATION ABOUT THE HARDWARE PARAMETERS

printf("\nPCM handle name = %s \n",snd_pcm_name(pcm_handle));


 printf("PCM state = %s\n",snd_pcm_state_name(snd_pcm_state(pcm_handle)));

 snd_pcm_hw_params_get_format(hwparams, &val);
   printf("format = '%s' (%s)\n",
       snd_pcm_format_name((snd_pcm_format_t)val),
           snd_pcm_format_description(
	                                (snd_pcm_format_t)val));




 snd_pcm_hw_params_get_buffer_time(hwparams,
                                     &val, 0);
				       printf("buffer time = %d us\n", val);


snd_pcm_hw_params_get_period_size(hwparams,
                                    &frames,0);
				      printf("period size = %d frames\n", (int)frames);

 snd_pcm_hw_params_get_period_time(hwparams,
                                     &val,0);
				       printf("period time = %d us\n", val);




snd_pcm_hw_params_get_access(hwparams, (snd_pcm_access_t *) &check);
			    printf("access type = %s \n",snd_pcm_access_name((snd_pcm_access_t)check));



 snd_pcm_hw_params_get_buffer_size_max(hwparams,
                          (snd_pcm_uframes_t *) &val);
			    printf("buffer size = %d frames\n", val);



snd_pcm_hw_params_get_periods(hwparams, &val, 0);
  printf("periods per buffer = %d frames\n", val);



 snd_pcm_hw_params_get_channels(hwparams,&chan);
 	printf("\nNumber of channels: %d \n",chan);

//WRITING DATA INTO HARDWARE


FILE *fp  = fopen("sample3.wav","rb");

int f=32;
//unsigned char *data;
//data = (unsigned char*)malloc(40);

int tell=0;
long count=0;
unsigned char *data;
data = (unsigned char*)malloc(2000);

buffer = (unsigned char *)malloc(500);

while(1)
	{
		
		count++;
	//	while((tell=(ftell(fp)-tell))<=100)
		
			//count++;
		fread(data,sizeof(unsigned char),120,fp);
		
	//	printf("\n\n\n\n%ld\n\n\n\n",count);

			if(!feof(fp))
				{
					if(pcmreturn=snd_pcm_writei(pcm_handle,data,30)<0)
						{
							snd_pcm_prepare(pcm_handle);
							printf("\n<<<<<<<<<<Buffer Underrun>>>>>>>\n");
							break;

						}
				
				}
			

			else break;

	}
	printf("\n%ld\n",count);

	 snd_pcm_drain(pcm_handle);
	   snd_pcm_close(pcm_handle);
	     free(buffer);

	       return 0;

}
