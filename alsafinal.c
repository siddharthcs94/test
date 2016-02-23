//i<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<USING MMAP>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.

#include<stdio.h>
#include<alsa/asoundlib.h>
#include<fcntl.h>
#include<stdlib.h>
#include<time.h>
//#include<unistd.h>
//#include<sys/time.h>


int main()
{


const snd_pcm_channel_area_t *areas;
//unsigned char *buffer;
char *pcm_name;
unsigned int rate = 8000;
int chan;
int rc;
unsigned int  val;
char check;
snd_pcm_sframes_t pcmreturn;
snd_pcm_sframes_t avail,sframes;
snd_pcm_uframes_t thresh,frames,offset;
int err;
snd_pcm_sframes_t commitres;



//Set PCM stream and handle
snd_pcm_t *pcm_handle;
snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;

//Set HARDWARE parameters
snd_pcm_hw_params_t *hwparams;


//pcm_name = (char *)malloc(10);
pcm_name = "plughw:0,0";
//pcm_name = "default";


//allocate memory for hardware
if(snd_pcm_hw_params_malloc(&hwparams)<0)
	{

		fprintf(stderr,"No memory allocated for hardware");
		return(-1);
	
	}



//<<<<<<<<<<<<<  SET PCM HANDLE  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.


if (snd_pcm_open(&pcm_handle, pcm_name, stream, 0) < 0) {
      fprintf(stderr, "Error opening PCM device %s\n", pcm_name);
            return(-1);
	        }





//<<<<<<<<<<<<<<<<<<<<<<<< INITIALIZE HARDWARE WITH CONFIGURATION OF THE SOUNDCARD >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.

	if(snd_pcm_hw_params_any(pcm_handle,hwparams)<0)
		{
			fprintf(stderr,"The hardware device cannot be configured\n");


		}





//SET FORMAT TO 16 BIT LITTLE ENDIAN
snd_pcm_hw_params_set_format(pcm_handle,hwparams,SND_PCM_FORMAT_S16_LE);


/* Two channels (stereo) */
 snd_pcm_hw_params_set_channels(pcm_handle,hwparams,2);

 
    /* (x) bits/second sampling rate (CD quality) */
      
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


//SETTING SOME MORE PARAMETERS

int first=0;;
//<<<<<<<<<<<<<<<<<<<<<<DISPLAYING INFORMATION>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//Buffer size
unsigned int buffer_size;
unsigned int period_time;

snd_pcm_sframes_t period_size;
unsigned int buffer_time;

snd_pcm_hw_params_get_period_time(hwparams,&period_time,0);
printf("\nPeriod Time : %d\n",period_time);


snd_pcm_hw_params_get_buffer_size(hwparams, (snd_pcm_uframes_t*)&buffer_size);
printf("\nThe buffer size is %d \n",buffer_size);


//Buffer time

snd_pcm_hw_params_get_buffer_time(hwparams, &buffer_time,0);
printf("\nBuffer time is : %d \n",buffer_time);


//Period size

snd_pcm_hw_params_get_period_size(hwparams,&period_size,0);
printf("\nperiod size : %d \n",(int)period_size);


//Periods between buffer

snd_pcm_hw_params_get_periods(hwparams,&val,0);
printf("\n Periods between buffer is : %d \n",val);








//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<               >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<,OPENING FILE>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>....



int dest =  open("sample3.wav",O_RDONLY);

if(dest<0)
	{
		perror("\nFile could not be opened\n");
		return -1;
	}




unsigned char *ptr[2];



int i;

int check2=1;




//FORMAT - 16 BIT LITTLE ENDIAN

snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;

snd_pcm_sframes_t size;



long k=0;



snd_pcm_state_t state;


size = 85;

while(1)
	{

		if(k==1)
			{
				err = snd_pcm_start(pcm_handle);
			}	
		k++;		
		
		size=period_size;		
	
		state = snd_pcm_state(pcm_handle);
		printf("\nPCM state : %s \n",snd_pcm_state_name(state));
	
		if(state == SND_PCM_STATE_SUSPENDED)
		{
		perror("\nDevice suspended\n");
		exit(-1);
		}
		
		
		//CHECK NUMBER OF FRAMES AVAILABLE (WHICH CAN BE ALLOCATED)
		avail=snd_pcm_avail_update(pcm_handle);
		printf(" \n1. Frames available = %ld \n",avail);
		
		if(avail<frames)
			{
				
				//PREPARE PCM DEVICE ONCE AVAILABLE FRAMES < FRAMES TO BE ALLOCATED
				err = 	snd_pcm_prepare(pcm_handle);
				if(err<0)
					{
						perror("\nFrames not prepared\n");
						exit(-1);
					}
	
			}

	

		while(size>0)
		{
		

			frames=size;

		//SET ASIDE MEMORY SPACE USING MMAP
		 err=snd_pcm_mmap_begin(pcm_handle,&areas,&offset,&frames);
			 
		
		  if(err<0)
		 	 {
		  		perror("\n\nMMAP ERROR\n\n");
		 		 return -1;
		  	}

		
				

		for(i=0;i<1;i++)
		{

		ptr[i] = (unsigned char*)(areas[i].addr  + (areas[i].first/8) + offset*(areas[i].step/8));
	
			if(ptr[i]==NULL)
				{
					perror("\nPointer has NULL value\n");
					return -1;
		
				}		
		}//FOR LOOP ENDS HERE

					

		
			     for(int i=0;i<1;i++)
			                      {
					
			                      if(read(dest,ptr[i],340)>0)
					      
			                              {
			                  
			                                      
			      
			                              }
			                      else 
					      
					      	{
							perror("\nEnd of file\n");

					      		exit(1);
						}
			                      }//FOR LOOP ENDS HERE


									
				
				commitres = snd_pcm_mmap_commit(pcm_handle,offset,frames);
					

					

							if(commitres<0)
							{	
								err = snd_pcm_prepare(pcm_handle);
								if(err<0)
									{
									perror("\n<<<<<<<MEMORY NOT COMMITTED>>>>>>>>\n");
									exit(1);
									}
							}


							else
							size -= frames;
							printf("\n COMMITED %ld \n",commitres);
							snd_pcm_wait(pcm_handle,100000);

					
				
	
	
	}//"SIZE>0" WHILE LOOP ENDS

	}//OUTER MOST WHILE LOOP ENDS








}

	





