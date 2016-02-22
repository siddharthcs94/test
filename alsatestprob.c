//i<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<USING MMAP>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.

#include<stdio.h>
#include<alsa/asoundlib.h>
#include<fcntl.h>
#include<stdlib.h>
#include<time.h>
//#include<unistd.h>
//#include<sys/time.h>



//////////////////////
//////////////////







int main()
{


const snd_pcm_channel_area_t *areas;
//unsigned char *buffer;
char *pcm_name;
unsigned int rate = 48000;
int chan;
int rc;
unsigned int  val;
char check;
snd_pcm_sframes_t pcmreturn;
snd_pcm_sframes_t avail;
snd_pcm_uframes_t thresh;



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
//int err;

/*snd_pcm_sframes_t avail = snd_pcm_avail_update(pcm_handle);
printf("\nNumber of frames available is : %d \n",(int)avail);
*/

//-------------------------------------------------------------------------------------------------------
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



////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//<<<<<<<<<<<<<mmap_begin>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/*
err=snd_pcm_mmap_begin(pcm_handle,&areas,&offset,&frames);




if(err<0)
{
printf("\n\nMMAP ERROR\n\n");
return -1;
}


*/


/*

  commitres = snd_pcm_mmap_commit(pcm_handle,offset,frames);
                                   if(commitres<0)
                                           {
                                                   printf("\n\nMemory cannot be committed\n\n");
                                                 return -1;

						}

*/






//Periods between buffer

snd_pcm_hw_params_get_periods(hwparams,&val,0);
printf("\n Periods between buffer is : %d \n",val);








//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<               >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<,OPENING FILE>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>....



int dest =  open("sample6.wav",O_RDONLY);

if(dest<0)
	{
		perror("\nFile could not be opened\n");
		return -1;
	}




unsigned char *ptr[2];



int i;
//unsigned int steps=0;


/*
for(i=0;i<2;i++)
 ptr[i] = (unsigned char*)(areas[i].addr  + (areas[i].first/8) + offset*(areas[i].step/8));
*/



/*



printf("\nOffset of the first sample : %u \n",areas[0].first);
printf("\n\nOffset of the memory map is : %u \n",areas[0].step/8);
printf("\nAreas start address is : %u \n",areas[0].addr);
printf("\nNumber of frames ; %u \n",frames);

printf("\n\nPointer before ; %u \n",ptr[0]);



if(ptr[1]==NULL)
printf("\nNull pointer 1  allocated\n");
if(ptr[0]==NULL)
printf("\nNull pointer 0 allocated \n");


buffer = (unsigned char*)malloc(1000);
*/


int check2=1;




//////////////////////////////////////////////

snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;




//////////////////////////////////////////////////////////

long count2=0;
long j=0;

long size=0;
snd_pcm_sframes_t size2=512;
long int temp ;
//while(1)
int err,sil,err2;

unsigned char *buffer = (unsigned char*)malloc(3000);

long k=0;









/*
     err=snd_pcm_mmap_begin(pcm_handle,&areas,&offset,&frames);
      
                        printf("\nOFFSET VALUE %ld \n",offset);
                        printf("\nNUMBER OF FRAMES = %ld \n",frames);
      
      
      
                        if(err<0)
                               {
                                      printf("\n\nMMAP ERROR\n\n");
                                       return -1;
                              }
      
                      printf("\n2. Frames %ld\n",frames);
      
      
      
      
      
      
                              size=frames;
                              //printf("\nCECK2\n");
                      for(i=0;i<2;i++)
                      ptr[i] = (unsigned char*)(areas[i].addr  + (areas[i].first/8) + offset*(areas[i].step/8));

*/

/*
while(1)
	{	
		for(int i=0;i<2;i++)
		{
		if(read(dest,buffer,10)>0)
			{
				printf("\n\n\n%d\n\n",dest);

				continue;

			}
		else exit(1);
		}

	}

*/




//<<<<<<<<<<<SETTING SOFTWARE PARAMETERS>>>>>>>>>>>>>>>

	
snd_pcm_sw_params_t *swparams;

snd_pcm_sw_params_malloc(&swparams);
err = snd_pcm_sw_params_current(pcm_handle,swparams);
if(err<0)
perror("\nUnable fo initialise SW Parameters\n");


//SETTING START THRESHOLD
int threshtime; 

threshtime = snd_pcm_sw_params_set_start_threshold(pcm_handle, swparams, (buffer_size/period_size)*period_size);


err = snd_pcm_sw_params_get_start_threshold(swparams, &thresh);


if(err<0)
	{
		perror("\nUnable to get start threshold time\n");

		exit(-1);
	}
else
	{	
		printf("\nStart threshold is %u \n",(unsigned int)thresh);
	}


err = snd_pcm_sw_params(pcm_handle,swparams);
if(err<0)
{
perror("\nUnable to set SW parameters\n");
exit(-1);
}

//if(err<0)
//perror("\nUnable to set software params\n");












//<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>..

snd_pcm_state_t state;


clock_t goal ;

while(1)
	{
		
		
		
		size=period_size;		
	
	
	
	
		state = snd_pcm_state(pcm_handle);
		printf("\nPCM state : %s \n",snd_pcm_state_name(state));

	
	
		if(state == SND_PCM_STATE_SUSPENDED)
		{
		perror("\nDevice suspended\n");
		exit(-1);
		}
		
		frames=size;


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

				
				printf("\n\nNEW STATE %s \n\n",snd_pcm_state_name(state));

	
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

		
				

		for(i=0;i<2;i++)
		{

		ptr[i] = (unsigned char*)(areas[i].addr  + (areas[i].first/8) + offset*(areas[i].step/8));
	
			if(ptr[i]==NULL)
				{
					perror("\nPointer has NULL value\n");
					return -1;
		
				}		
		}//FOR LOOP ENDS HERE

					

		
			     for(int i=0;i<2;i++)
			                      {k++;

			                      if(read(dest,buffer,2048)>0)
					      
			                              {
			                                   
							  goal = 2500 + clock();
							  while(goal>clock());

							   // printf("\n\n\n%d\n\n",dest);
			     				//	continue;


			                                      
			      
			                              }
			                      else 
					      
					      	{
							perror("\nEnd of file\n");
							printf("\nNUMBER OF WRITES %ld\n",k);

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

									
								break;



							}


							else
							size -= frames;
							printf("\n COMMITED %ld \n",commitres);

	
	
	
	
	}//"SIZE>0" WHILE LOOP ENDS

	}//OUTER MOST WHILE LOOP ENDS








}

	





