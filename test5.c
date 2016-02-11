//i<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<USING MMAP>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.

#include<stdio.h>
#include<alsa/asoundlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/time.h>






int main()
{

const snd_pcm_channel_area_t *areas;
int size;
unsigned char *buffer;
char *pcm_name;
unsigned int rate = 44100;
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


		}




//SET FORMAT TO 16 BIT LITTLE ENDIAN
snd_pcm_hw_params_set_format(pcm_handle,hwparams,SND_PCM_FORMAT_S16_LE);


/* Two channels (stereo) */
 snd_pcm_hw_params_set_channels(pcm_handle,hwparams,2);

 
    /* 44100 bits/second sampling rate (CD quality) */
      
 if (snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_MMAP_INTERLEAVED) < 0) {
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


snd_pcm_uframes_t  frames,offset ;//OF TYPE UNSIGNED LONG
snd_pcm_sframes_t commitres;
frames =32;


//SETTING SOME MORE PARAMETERS

size = frames*4;
//SETTING BUFFER SIZE

/*if (snd_pcm_hw_params_set_buffer_size(pcm_handle, hwparams, (periodsize*2)>>2) < 0) {
      fprintf(stderr, "Error setting buffersize.\n");
            return(-1);


}
*/

int first=0;;
int err;
snd_pcm_sframes_t avail = snd_pcm_avail_update(pcm_handle);
printf("\nNumber of frames available is : %d \n",(int)avail);


//-------------------------------------------------------------------------------------------------------
//<<<<<<<<<<<<<<<<<<<<<<DISPLAYING INFORMATION>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//Buffer size

snd_pcm_hw_params_get_buffer_size(hwparams, (snd_pcm_uframes_t*)&val);
printf("\nThe buffer size is %d \n",val);


//Buffer time

snd_pcm_hw_params_get_buffer_time(hwparams, &val,0);
printf("\nBuffer time is : %d \n",val);


//Period size

snd_pcm_hw_params_get_period_size(hwparams,&frames,0);
printf("\nperiod size : %d \n",(int)frames);



//Periods between buffers


err=snd_pcm_mmap_begin(pcm_handle,&areas,&offset,&frames);


//Periods between buffer

snd_pcm_hw_params_get_periods(hwparams,&val,0);
printf("\n Periods between buffer is : %d \n",val);




//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.

if(err<0)
	{
	printf("\nMMAP error\n");
	return -1;

	
	
	}
else printf("\n%d\n",err);



commitres = snd_pcm_mmap_commit(pcm_handle,offset,frames);
if(commitres<0)
	{
		printf("\nFrames not committed to memory\n");
	}



int dest =  open("sample3.wav",O_RDONLY);

if(dest<0)
	{
		perror("\nFile could not be opened\n");
		return -1;
	}


//int *c = offset;
buffer = (unsigned char *)malloc(1000);



unsigned long buf = offset;
 if(read(dest,&offset,1)<0)
	{
		perror("\nNo destination for read\n");
		return -1;
	}

struct timeval tvbefore,tvafter;
gettimeofday(&tvbefore,NULL);

while(read(dest,buffer,100)!=0);
gettimeofday(&tvafter,NULL);
printf("\nTime taken by read() is %09ld \n",tvafter.tv_usec-tvbefore.tv_usec);



/*


while(1)
	{
		if(read(dest,&offset,80)!=0)
			{
				//fread(buffer,sizeof(buffer),160,f1);
				
				
					{
						if(pcmreturn = snd_pcm_mmap_writei(pcm_handle,&offset,80)<0)
							{
								snd_pcm_prepare(pcm_handle);
								printf("\n<<<<<<<Buffer Underrun>>>>>>>>\n");
								break;
							}
					}
			

		

			}
		else break;

			
	}
*/	
}


