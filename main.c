#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <imageprocessing.h>

#define N_PROCESSOS  3


void avaliar_vizinhanca_red(int k, int z, float *r, imagem img, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img.r[j*(img.width) + i];
    }
  }
  soma /= ( (2*(N+1)) * (2*(N+1) ) -1 );
  r[z*(img.width) + k] = soma;
}

void avaliar_vizinhanca_blue(int k, int z, float *b, imagem img, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img.b[j*(img.width) + i];
    }
  }
  soma /= ( (2*(N+1)) * (2*(N+1) ) -1 );
  b[z*(img.width) + k] = soma;
}

void avaliar_vizinhanca_green(int k, int z, float *g, imagem img, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img.g[j*(img.width) + i];
    }
  }
  soma /= ( (2*(N+1)) * (2*(N+1) ) -1 );
  g[z*(img.width) + k] = soma;
}

int main(){

    imagem img;
    float *r, *g, *b;

    
    int nucleo;

    pid_t filho[N_PROCESSOS];

    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANON;

    img = abrir_imagem("data/cachorro.jpg");

    r = (float*) mmap(NULL, (img.width * img.height)*sizeof(float), protection, visibility, 0, 0);
    g = (float*) mmap(NULL, (img.width * img.height)*sizeof(float), protection, visibility, 0, 0);
    b = (float*) mmap(NULL, (img.width * img.height)*sizeof(float), protection, visibility, 0, 0);

    for(int i = 0; i < img.width; i++){
      for(int j = 0; j < img.height; j++){
        r[j*img.width + i] = img.r[j*img.width + i];
        g[j*img.width + i] = img.g[j*img.width + i];
        b[j*img.width + i] = img.b[j*img.width + i];
      }
    }

    if( (img.width) > 600){
        nucleo = 10;
    }
    else nucleo = 3; 

    printf("Nucleo é: %d\n",nucleo);


    /*Criar processos filhos*/

    for(int k = 0; k < N_PROCESSOS ; k++){
        filho[k] = fork();

        if (filho[k]==0) {

            /* Processo filho(k) Opera */ 
            printf("Filho[%d]\n", k);
            int i, j;

            for (int i = k; i<(img.width); i += 3) {
                for (int j = 0; j<(img.height); j++) {
                    if( (i >= nucleo) && (j >= nucleo) && ( (img.width) - i > nucleo) && ( (img.height) - j > nucleo) ){
                        avaliar_vizinhanca_red(i, j, r, img, nucleo);
                        avaliar_vizinhanca_blue(i, j, b, img, nucleo);
                        avaliar_vizinhanca_green(i, j, g, img, nucleo);
                    }
                }
            }
            exit(0);
        }
    }


    for (int k = 0; k < N_PROCESSOS ; k++){
        waitpid (filho[k], NULL, 0);
    }

    for(int i = 0; i < img.width; i++){
      for(int j = 0; j < img.height; j++){
        img.r[j*img.width + i] = r[j*img.width + i];
        img.g[j*img.width + i] = g[j*img.width + i];
        img.b[j*img.width + i] = b[j*img.width + i];
      }
    }

    salvar_imagem("cachorro-out.jpg", &img);
    liberar_imagem(&img);
    
    return 0;
}
