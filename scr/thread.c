
#include <imageprocessing.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WORKERS 3

imagem img;
imagem img2;

int nucleo;

void avaliar_vizinhanca_red(int k, int z, imagem img, imagem img2, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img2.r[j*img.width + i];
    }
  }
  soma /= 2*N*2*N;
  img.r[z*img.width + k] = soma;
}

void avaliar_vizinhanca_blue(int k, int z, imagem img, imagem img2, int N){
  float soma= 0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img2.b[j*img.width + i];
    }
  }
  soma /= 2*N*2*N;
  img.b[z*img.width + k] = soma;
}

void avaliar_vizinhanca_green(int k, int z, imagem img, imagem img2, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img2.g[j*img.width + i];
    }
  }
  soma /= 2*N*2*N;
  img.g[z*img.width + k] = soma;
}




void* worker(void *arg) {
  /* Inicializacao: guardar o proprio numero da thread */
  int *N = (int*)(arg);
  int M = (*N);

  while (1) {

    float soma = 0; 
    int i, j;
    for (int i = M; i<(img.width); i += 3) {
        for (int j = 0; j<(img.height); j++) {
            if( (i >= nucleo) && (j >= nucleo) && (img.width - i > nucleo) && (img.height - j > nucleo) ){
                avaliar_vizinhanca_red(i, j, img, img2, nucleo);
                avaliar_vizinhanca_blue(i, j, img, img2, nucleo);
                avaliar_vizinhanca_green(i, j, img, img2, nucleo);
            }
        }
    }
    return NULL;
  }
    return NULL;
}


int main(){
    
    pthread_t workers[WORKERS];
    int thread_id[WORKERS];

    img = abrir_imagem("data/cachorro.jpg");
    img2 = abrir_imagem("data/cachorro.jpg");

/* Identificadores de thread */
    for (int i = 0; i < WORKERS; i++) {
        thread_id[i] = i;
        }


    if(img.width > 600){
        nucleo = 10;
    }
    else nucleo = 3; 


      /* Disparando threads */
    for (int i=0; i < WORKERS; i++) {
        pthread_create(&(workers[i]), NULL, worker, (void*) (&thread_id[i]));
    }

        /* Esperando threads */
    for (int i=0; i < WORKERS; i++) {
        pthread_join(workers[i], NULL);
    }

    salvar_imagem("cachorro-out.jpg", &img);
    liberar_imagem(&img2);
    return 0;
}
