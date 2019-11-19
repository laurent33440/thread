/**
 * probleme diner des philosophes
 *
 */

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

#define NB_PHILOSOPHE (3)
#define MAX_WAIT (3)

typedef unsigned short uint8_t;

/*  static uint8_t philosophe[NB_PHILOSOPHE]; */

typedef struct _stick_model
{
	uint8_t is_free;
} stick_model_t;

static stick_model_t stick[NB_PHILOSOPHE];

typedef struct _thread_context 
{
	unsigned long philosophe_ids[NB_PHILOSOPHE];
	pthread_mutex_t stick_mutex;
} thread_context_t;

static thread_context_t thread_context = {
	.stick_mutex = PTHREAD_MUTEX_INITIALIZER,
};

static uint8_t randomize_quantity(uint8_t max)
{
	return (uint8_t)(rand() % max+1);
}

static void think(void)
{
	uint8_t tp = randomize_quantity(MAX_WAIT);
	sleep(tp);
}

static void eat(void)
{
	uint8_t tm = randomize_quantity(MAX_WAIT);
	sleep(tm);
}

static void* fn_philosophe(void* philosophe_n)
{
	uint8_t philosophe_nb;
	uint8_t stick_left;
	uint8_t stick_right;
	int rstatus = 0;

	philosophe_nb = *((uint8_t*)philosophe_n);

	while (0 == rstatus)
	{
		printf("Philosophe #%u think \n",philosophe_nb);
		think();
		if (philosophe_nb == 0 )
		{
			stick_left = NB_PHILOSOPHE - 1;
		}
		else 
		{
			stick_left = philosophe_nb - 1;
		}
		stick_right = philosophe_nb;
		rstatus =  pthread_mutex_lock(&thread_context.stick_mutex);
		if(0 != rstatus)
		{
			printf("Error : mutex lock -- philosophe#%u \n",philosophe_nb);
		}
		if (stick[stick_left].is_free && stick[stick_right].is_free)
		{	
			printf("Philosophe #%u prend les baguettes gauche#%u et droite#%u",
					philosophe_nb, stick_left, stick_right);
			stick[stick_left].is_free = 0;
			stick[stick_right].is_free = 0;

			rstatus = pthread_mutex_unlock(&thread_context.stick_mutex);
			if(0 != rstatus)
			{
				printf("Error : mutex unlock -- philosophe#%u \n",philosophe_nb);
			}
			else
			{
				printf("Philosophe #%u eat \n",philosophe_nb);
				eat();
				rstatus =  pthread_mutex_lock(&thread_context.stick_mutex);
				if(0 != rstatus)
				{
					printf("Error : mutex lock -- philosophe#%u \n",philosophe_nb);
				}
				else
				{	
					printf("Philosophe #%u prend les baguettes gauche#%u et droite#%u\n",
							philosophe_nb, stick_left, stick_right);
					stick[stick_left].is_free = 1;
					stick[stick_right].is_free = 1;
					rstatus = pthread_mutex_unlock(&thread_context.stick_mutex);
					if(0 != rstatus)
					{
						printf("Error : mutex unlock -- philosophe#%u \n",philosophe_nb);
					}
				}
			}
		}
	}

	{
		/* handle error */
	}
	return NULL;
}

static void init_stick(void)
{
	uint8_t i;
	for (i = 0 ; i < NB_PHILOSOPHE ; i++)
	{
		stick[i].is_free = 1;
	}
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	int status;
	void* retval_thread;
	uint8_t i;
	uint8_t idx[NB_PHILOSOPHE];

	init_stick();
	status = EXIT_SUCCESS;

	printf( "--> main thread  - create philosophes : \n" );
	for(i = 0 ; i < NB_PHILOSOPHE && status == EXIT_SUCCESS ; i++ )
	{
		idx[i] = i;
		if (0 !=  pthread_create(&thread_context.philosophe_ids[i],
					NULL,
					&fn_philosophe,
					&idx[i]
					))					
		{
			printf("Erreur : impossible de créer le thread du philosopĥe %u !! \n",i);
			status  = EXIT_FAILURE;
		}
	       	else 
		{
			printf("philosope# %u id#%lx created \n",i,thread_context.philosophe_ids[i]);
		}	
	}

	for(i = 0 ; i < NB_PHILOSOPHE && status == EXIT_SUCCESS ; i++ )
	{
		if (0 !=  pthread_join(thread_context.philosophe_ids[i], &retval_thread))
		{
			printf("Erreur : impossible de joindre le thread du philosopĥe %lx !! \n",thread_context.philosophe_ids[i]);
			status  = EXIT_FAILURE;
		}
	       	else 
		{
			printf("philosope %lx joined \n",thread_context.philosophe_ids[i]);
		}	
	}
		
	return status;

}
