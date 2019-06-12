
#include <imageprocessing.h>
#include <stdio.h>


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


int main() {
  imagem img;
  imagem img2;
  img = abrir_imagem("data/random-pic-14.jpg");
  img2 = abrir_imagem("data/cachorro.jpg");
  int N;

  if(img.width > 600){
    N = 10;
  }
  else N = 3; 

  printf("N é %d\n", N);

  for (int i=0; i<(img.width); i++) {
    for (int j=0; j<(img.height); j++) {
      if( (i>=N) && (j>=N) && (img.width - i > N) && (img.height - j > N) ){
        avaliar_vizinhanca_red(i, j, img, img2, N);
        avaliar_vizinhanca_blue(i, j, img, img2, N);
        avaliar_vizinhanca_green(i, j, img, img2, N);
      }
    }
  }

  salvar_imagem("cachorro-out.jpg", &img);
  liberar_imagem(&img);
  liberar_imagem(&img2);
  return 0;
}
