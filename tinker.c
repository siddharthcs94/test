
#include<stdio.h>
#include<stdlib.h>
#include "sndfile.h"

#include<alsa/asoundlib.h>

int main()
{
char *filename = "sample6.wav";
 SF_INFO sfinfo;
  SNDFILE *f;
  f = sf_open(filename,SFM_READ, &sfinfo);
 printf("\nCHECK RATE %d \n",sfinfo.channels);
 
 


snd_pcm_t *pcm_handle;
char *pcm_name;
unsigned int err;
//pcm_name = "default";
pcm_name = "plughw:0,0";
snd_pcm_hw_params_t *hwparams;
unsigned int rate = sfinfo.samplerate;
unsigned int channels = sfinfo.channels;
unsigned int exact_rate;


 snd_pcm_uframes_t frames, offset;
 snd_pcm_sframes_t commitres, avail,size=170;
 const snd_pcm_channel_area_t *areas;
 unsigned char *ptr[2];
 
snd_pcm_format_t stream = SND_PCM_FORMAT_S16_LE;


err=snd_pcm_open(&pcm_handle,pcm_name,SND_PCM_STREAM_PLAYBACK,0);
         
         if(err<0)       
                 {       
                         perror("\nCannot open PCM device\n");
                         exit(0);
           	}


if(snd_pcm_hw_params_malloc(&hwparams)<0)
	{
		perror("\nMemory cannot be allocated\n");
		exit(0);
	}


if(snd_pcm_hw_params_any(pcm_handle,hwparams)<0)
	{
		perror("\nUnable to set any HW configuration to device\n");
		exit(0);
	}

exact_rate = rate;

if(snd_pcm_hw_params_set_rate_near(pcm_handle,  hwparams,  &exact_rate, 0)<0)
	{
		printf("\nUnable to set rate %u instead rate used is %u \n",rate,exact_rate);
	}
rate = exact_rate;
printf("\nExact rate is %u \n",rate);


if(snd_pcm_hw_params_set_channels(pcm_handle,  hwparams,  channels),0)
	{
		perror("\nUnable to set channels\n");
		exit(0);
	}
if(snd_pcm_hw_params_set_format(pcm_handle,  hwparams,  stream)<0)
	{
		perror("\nUnable to set format\n");
		exit(0);
	}

if(snd_pcm_hw_params_set_access(pcm_handle,  hwparams,  SND_PCM_ACCESS_MMAP_INTERLEAVED)<0)
	{
		perror("\nUnable to set access\n");
		exit(0);
	}



snd_pcm_sframes_t period_size=170;

err = snd_pcm_hw_params_set_period_size(pcm_handle,  hwparams,  period_size,  0);
	   if(err<0)       
                 {       
                         perror("\nCannot open PCM device\n");
                         exit(0);
                 }
printf("\nPeriod size is %ld \n",period_size);



frames = 2048;
if(snd_pcm_hw_params_set_buffer_size(pcm_handle,  hwparams,  frames)<0)
	{
		perror("\nCannot set buffer size\n");
		exit(0);
	}

printf("\nBuffer size is %ld \n",frames);




err = snd_pcm_hw_params(pcm_handle,hwparams);
if(err<0)
	{
		perror("\nHW params cannot be set\n");
		exit(0);
	}


snd_pcm_hw_params_get_period_size(hwparams,&period_size,0);
printf("\nObtained Period size = %ld \n",period_size);

size = period_size;

snd_pcm_hw_params_get_buffer_size(hwparams,&frames);
printf("\nObtained buffer size is %ld \n",frames);


int ret = open(filename,O_RDONLY);
if(ret<0)
	{
		perror("\nFile cannot be opened\n");
		exit(0);
	}



int k=0;
int i;

while(1)
	{
	k++;
	if(k==2)
		{
			err = snd_pcm_start(pcm_handle);
			if(err<0)
				{
					perror("\nError starting PCM device\n");
					exit(1);
				}
		}
 
		
	avail = snd_pcm_avail_update(pcm_handle);
		if(avail<frames)
			{
				err = snd_pcm_prepare(pcm_handle);
				if(err<0)
					{
						perror("\nNo frames available\n");
						exit(0);
					}
		}
//	printf("\nFrames available %ld\n",avail);

	
	size=period_size;


	while(size>0)
		{
			

			frames = size;

			err = snd_pcm_mmap_begin(pcm_handle,  &areas,  &offset,  &frames);
						
			 if(err<0)
				{
					perror("\nMMAP cannot assign areas\n");
					exit(0);
				}
			

			for(i=0;i<1;i++)
			{
			ptr[i] = (unsigned char *)areas[i].addr + areas[i].first/8 + offset*(areas[i].step/8) ;
		//	printf("\nCheck\n");


				if(ptr[i]==NULL)
					{
						perror("\nPointer cannot point to memory\n");
						exit(0);
					}

			}


			for(i=0;i<1;i++)
			if(read(ret, ptr[i], frames*4)>0)
				{
					
				
				}
			else 
				{
					perror("\nFile cannot be opened\n");
					exit(0);
				}

				      commitres = snd_pcm_mmap_commit(pcm_handle,  offset,  frames);
 
                                         if(commitres<0)
                                                 {
                                                         err = snd_pcm_prepare(pcm_handle);
 
                                                         if(err<0)
                                                                 {
                                                                 perror("\nFrames cannot be committed\n");
                                                                exit(0);
                                                                }
						
						}

				else size-=frames;
			snd_pcm_wait(pcm_handle,1000);


		}//END OF "while(size>0)" LOOP

		}//END OF OUTER WHILE LOOP

}
		












