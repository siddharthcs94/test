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
//frames =32;


//SETTING SOME MORE PARAMETERS

//size = frames*4;
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



////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//<<<<<<<<<<<<<mmap_begin>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


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


/*
commitres = snd_pcm_mmap_commit(pcm_handle,offset,frames);
if(commitres<0)
	{
		printf("\nFrames not committed to memory\n");
	}

*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<               >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<,OPENING FILE>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>....



int dest =  open("sample3.wav",O_RDONLY);

if(dest<0)
	{
		perror("\nFile could not be opened\n");
		return -1;
	}



//buffer = (unsigned char *)malloc(1000);

/*

unsigned long buf = offset;
 if(read(dest,&offset,1)<0)
	{
		perror("\nNo destination for read\n");
		return -1;
	}
*/


/*
avail=snd_pcm_avail_update(pcm_handle);
printf("\nAvailable frames = %d \n\n",(int)avail);
struct timeval tvbefore,tvafter;
gettimeofday(&tvbefore,NULL);
*/
unsigned char *ptr[2];



int i;
//unsigned int steps=0;

for(i=0;i<2;i++)
 ptr[i] = (unsigned char*)areas[i].addr  + (areas[i].first/8) + offset*(areas[i].step/8);

printf("\nOffset of the first sample : %u \n",areas[0].first);
printf("\n\nOffset of the memory map is : %u \n",areas[0].step/8);
printf("\nAreas start address is : %u \n",areas[0].addr);
printf("\nNumber of frames ; %u \n",frames);

printf("\n\nPointer before ; %u \n",ptr[0]);


long pl;
for(pl=0;pl<100000;pl++);

if(ptr[1]==NULL)
printf("\nNull pointer 1  allocated\n");
if(ptr[0]==NULL)
printf("\nNull pointer 0 allocated \n");




int check2=1;

snd_pcm_sframes_t size2 =30;
while(1)
	{
		for(i=0;i<2;i++)
		{
		//ptr[i] = (unsigned char*)areas[i].addr + (areas[i].first/8) + offset*(areas[i].step/8);
		
		if(read(dest,ptr[i],120)!=0)
			{
				
				
					if(ptr[i]!=NULL)
					{
						 	
						if(pcmreturn = snd_pcm_mmap_writei(pcm_handle,ptr[i],size2)<0)
							{
								snd_pcm_prepare(pcm_handle);
								printf("\n<<<<<<<Buffer Underrun>>>>>>>>\n");
								break;
							}
					
			

						//	printf("\n%d\n",pcmreturn);
						ptr[i]+=0;
						printf("\n%lu\n",ptr[i]);


					}
											

			}
			else {
				check2=0;
				break;
				}
		}
		
		

		if(check2==0)
		break;
		

			
		
	//	l:break;

	}

	commitres = snd_pcm_mmap_commit(pcm_handle,offset,frames);
	 if(commitres<0)
	         {
	                 printf("\nFrames not committed to memory\n");
	         }

	
}


