#include <stdio.h>
#include <stdlib.h>

// registro denominado Pixel que guardar�
//tr�s vari�veis para representar um pixel
//, com as cores Red , Green e Blue
typedef struct Pixel { 
  int r;
  int g;
  int b;
}Pixel;
// Registro Imagem que alem de ter  Tamanho e Largura
//ter� uma vari�vel pixels do tipo Pixel ou seja ,
//ela ter� as vari�veis int r, g e b.
typedef struct Imagem { 
  int largura;
  int altura;
  Pixel** pixels;
}Imagem;

// fun��o que ir� criar a imagem
Imagem* criar_imagem(int largura, int altura) { 
  int i;
  Imagem* nova_imagem; // primeiramente cria-se uma var�avel do tipo Imagem , pois ela receber� posteriormente  atributos como Largura , Altura e Pixel
  nova_imagem = malloc(sizeof(Imagem));// aloca��o dinamica do tamanho da imagem
  nova_imagem -> largura = largura;// a imagem clonada receber� a largura da imagem original atrav�s do ponteiro que ir� fazer essa referencia
  nova_imagem -> altura = altura;// a  imagem clonada receber� a alturada imagem original atrav�s do ponteiro que ir� fazer essa referencia
  nova_imagem -> pixels = malloc(sizeof(Pixel* ) *largura);// Nesse trecho a imagem clonada alocar� dinamicamente o espa�o em pixels da imagem
  for (i = 0; i < nova_imagem -> largura; ++i) {// for para alocar pixel por pixel na imagem, por op��o, nesse caso ,se alocou a largura para preencher pela altura
    nova_imagem -> pixels[i] = malloc(sizeof(Pixel) * altura); // preenchimento dos pixels na altura
  }
  return nova_imagem;
}
int saturacao(int valor_com_excesso) { // ir� saturar os pixels depois de serem adicionados no filtro
  if (valor_com_excesso < 0) { // caso o valor seja negativo ele ser� setado como 0
    return 0;
  } else if (valor_com_excesso > 255) { // caso o valor seja muito grande , o valor ser� setado para 255
    return 255;
  } else {
    return valor_com_excesso; // caso o valor esteja entre 0 e 255 , o valor nao precisa ser saturado
  }
}
int media_pixel(Pixel limiarizacao) { // fun��o criada para fazer uma media  dos pixels r , g e b para a binarizacao
  int media;
  media = (limiarizacao.r + limiarizacao.g + limiarizacao.b) / 3;
  return media;
}
// o zoom deixar� pixels vazio pois a imagem ser� aumentada.
//Logo, ser� preciso colocar pixels para preencher essas lacunas ,
//sendo assim , se calcula a media de cada pixel a redor dessas lacunas e joga esse valor para o espa�o vazio
Pixel calculo_media(Pixel* a, Pixel* b, Pixel* c, Pixel* d) {
  Pixel aux;// variavel criada para receber as cores rgb
  aux.r = (a -> r + b -> r + c -> r + d -> r) / 4;// media dos pixels
  aux.g = (a -> g + b -> g + c -> g + d -> g) / 4;
  aux.b = (a -> b + b -> b + c -> b + d -> b) / 4;

  return aux;
}

//------------------------------------------------ Funcoes para a adicao de zoom na imagem ---------------------------------//

Imagem* zoom4x(Imagem* imagem_original) {// Funcao para adicionar um zoom4x na imagem
  int largura = imagem_original -> largura;
  int altura = imagem_original -> altura;
  Imagem* nova_img = criar_imagem(altura,largura);
  nova= zoom2x(zoom2x(imagem_original));// Para adicionar esse efeito � necessario apenas dar duas vezes o zoom2x na imagem, ficando assim 4x
  return nova_img;
}
Imagem* zoom8x(Imagem* imagem_original) {// Funcao para adicionar um zoom8x na imagem
  int largura = imagem_original -> largura;
  int altura = imagem_original -> altura;
  Imagem* nova_img = criar_imagem(altura,largura);
  nova= zoom4x(zoom2x(imagem_original));// Para adicionar esse efeito � necessario apenas colocar dentro da funcao zoom4x , a funcao zoom2x , tornando-a 8x
  return nova_img;
}

Imagem * reduzir2x(Imagem* imagem_original) {// funcao para reduzir a matriz em 2x
  int i;
  int j;
  Pixel a, b, d, e, c, p;
  Imagem * nova_imagem;
  nova_imagem = criar_imagem(imagem_original -> largura / 2, imagem_original -> altura / 2);
  for (i = 0; i < imagem_original -> altura - 2; i += 2) {// altura -2 pra ele nao pegar as bordas, nao faz sentido diminuir algo que nao vou usar
    for (j = 0; j < imagem_original -> largura - 2; j += 2) {
      a = imagem_original -> pixels[i][j];  // adicionar pixels nos espa�os que ficaram vazio da matriz
      b = imagem_original -> pixels[i][j + 1];
      c = imagem_original -> pixels[i + 1][j];
      d = imagem_original -> pixels[i + 1][j + 1];
      p = calculo_media( &a, &b, &c, &d);// media de quatro variaveis
      nova_imagem -> pixels[i / 2][j / 2] = p;
    }
  }

  return nova_imagem;
}
Imagem* reduzir4x(Imagem* imagem_original) {// funcao para reduzir a imagem num zoom 4x
  int largura = imagem_original -> largura;
  int altura = imagem_original -> altura;
  Imagem* nova_img = criar_imagem(altura,largura);
  nova= reduzir2x(reduzir2x(imagem_original));// � necessario apenas colocar a funcao reduzir2x uma dentro da outra , fazendo assim 4x
  return nova_img;
}
Imagem* reduzir8x(Imagem* imagem_original) {// funcao para reduzir a imagem num zoom 8x
  int largura = imagem_original -> largura;
  int altura = imagem_original -> altura;
  Imagem* nova_img = criar_imagem(altura,largura);
  nova= reduzir4x(reduzir2x(imagem_original));// � necessario apenas colocar a funcao reduzir2x dentro da funcao 4x fazendo assim 8x
  return nova_img;
}
//-------------------------------------------Funcao de binarizacao----------------------------------------//

Imagem* binarizacao(Imagem* original) { // Fun��o que ir� fazer a binarizacao da imagem
  int i, j;
  int media;
  int largura = original -> largura; // variavel largura criada tendo como valor a largura da imagem clonada
  int altura = original  -> altura; //variavel largura criada tendo como valor a altura da imagem clonada
  for (i = 0; i < altura; i++) {
    for (j = 0; j < largura; j++) {
      media = media_pixel(original -> pixels[i][j]); // ser� feita uma m�dia dos r,g e b de cada pixel e o valor ser� retornado para a variavel media
		// estabelecemos aqui um limite na limiariza��o ,
		//caso a media for maior que 127 , sete ela como 255 (branco) ,
		//caso seja menor ou igual a 127 , sete ela como preto, dando assim um efeito de preto e branco na imagem.
		if (media > 127) { 
        media = 255;
        original -> pixels[i][j].r = media; // os pixels da imagem clonada receber�o  os valores da media de acordo com o if , para cada r ,g e b
        original -> pixels[i][j].g = media;// os pixels da imagem clonada receber�o  os valores da media de acordo com o if , para cada r ,g e b
        original -> pixels[i][j].b = media;// os pixels da imagem clonada receber�o  os valores da media de acordo com o if , para cada r ,g e b
      } else if (media <= 127) {
        media = 0;
        original -> pixels[i][j].r = media;// os pixels da imagem clonada receber�o  os valores da media de acordo com o if , para cada r ,g e b
        original -> pixels[i][j].g = media;// os pixels da imagem clonada receber�o  os valores da media de acordo com o if , para cada r ,g e b
        original -> pixels[i][j].b = media;// os pixels da imagem clonada receber�o  os valores da media de acordo com o if , para cada r ,g e b
      }

    }
  }
  return original;
}
//---------------------------------------------------- funcao para o calculo de filtros --------------------------------------------------------------------------------------------//

// fun��o para o o calculo do filtro de detec��o de bordas que
//ter� como parametros uma imagem que ser� o clone de outra imagem clonada
//primeria que receber� o nome de originale os i e j dos respectivos for's
//que mais tarde ser�o necessarios para a implementa��o dos pixels na imagem
void DetecBordCalculo(Imagem* imagem_clonada, Imagem* imagem_original, int i, int j) { 
  float Bordas[3][3] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
  int ii, jj, x, y, r_ss = 0, g_ss = 0, b_ss = 0;
  x = i - 1;// fazer com que ele nao fique menos pra nao pegar a borda preta
  ii = 0;
  // while para controlar o tamanho da matriz do filtro
  //afim de que ele sempre seja zerado , quando o segundo while chega
  //ao fim esse while continua  at� a matriz chegar a posi��o 3x3 onde ir� ser resetada
  while (x <= i + 1){
    y = j - 1;
    jj = 0;
    while (y <= j + 1){
    		// o valor de cada r,g e b sao multiplicados pelos numeros do filtros e incrementados com os valores posteriores de cada pixel
      r_ss += imagem_original -> pixels[x][y].r * Bordas[ii][jj];
      g_ss += imagem_original -> pixels[x][y].g * Bordas[ii][jj];
      b_ss += imagem_original -> pixels[x][y].b * Bordas[ii][jj];
      y++;
      jj++;
    }
    x++;
    ii++;
  }
  imagem_clonada -> pixels[i][j].r = saturacao(r_ss);// Saturacao dos pixels
  imagem_clonada -> pixels[i][j].g = saturacao(g_ss);
  imagem_clonada -> pixels[i][j].b = saturacao(b_ss);
}
// fun��o para o o calculo do filtro de Blur
//que ter� como parametros uma imagem que ser� o clone
//de outra imagem clonada primeria que receber� o nome de original
//e os i e j dos respectivos for's que mais tarde ser�o necessarios para a implementa��o dos pixels na imagem
void BlurCalculo(Imagem * imagem_clonada, Imagem * imagem_original, int i, int j) {
  float blur[3][3] = {1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0};
  int ii, jj, x, y, r_ss = 0, g_ss = 0, b_ss = 0;
  x = i - 1;
  ii = 0;
  // while para controlar o tamanho da matriz do filtro a
  //fim de que ele sempre seja zerado , quando o segundo while
  //chega ao fim esse while continua  at� a matriz chegar a posi��o 3x3 onde ir� ser resetada
  while (x <= i + 1) {
    y = j - 1;
    jj = 0;
    while (y <= j + 1) {
    		// o valor de cada r,g e b sao multiplicados pelos numeros do filtros e incrementados com os valores posteriores de cada pixel
      r_ss += imagem_original -> pixels[x][y].r * blur[ii][jj];
      g_ss += imagem_original -> pixels[x][y].g * blur[ii][jj];
      b_ss += imagem_original -> pixels[x][y].b * blur[ii][jj];
      y++;
      jj++;
    }
    x++;
    ii++;
  }
  imagem_clonada -> pixels[i][j].r = saturacao(r_ss);// apos tudo isso os pixels ser�o saturados
  imagem_clonada -> pixels[i][j].g = saturacao(g_ss);
  imagem_clonada -> pixels[i][j].b = saturacao(b_ss);
}
// fun��o para o o calculo do filtro de Sharppening que
//ter� como parametros uma imagem que ser� o clone de outra imagem
//clonada primeria que receber� o nome de originale os i e j dos respectivos
//for's que mais tarde ser�o necessarios para a implementa��o dos pixels na imagem
void sharpeningCalculo(Imagem* imagem_clonada, Imagem* imagem_original, int i, int j) {
  int sharp[3][3] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
  int ii, jj, x, y, r_ss = 0, g_ss = 0, b_ss = 0;
  x = i - 1;
  ii = 0;
// while para controlar o tamanho da matriz do filtro
//afim de que ele sempre seja zerado , quando o segundo while
//chega ao fim esse while continua  at� a matriz chegar a posi��o 3x3 onde ir� ser resetada
  while (x <= i + 1) { 
    y = j - 1;
    jj = 0;
    while (y <= j + 1) {
    	// o valor de cada r,g e b sao multiplicados pelos numeros do filtros e incrementados com os valores posteriores de cada pixel
      r_ss += imagem_original -> pixels[x][y].r * sharp[ii][jj];
      g_ss += imagem_original -> pixels[x][y].g * sharp[ii][jj];
      b_ss += imagem_original -> pixels[x][y].b * sharp[ii][jj];
      y++;
      jj++;
    }
    x++;
    ii++;
  }
  imagem_clonada -> pixels[i][j].r = saturacao(r_ss);// apos tudo isso os pixels ser�o saturados
  imagem_clonada -> pixels[i][j].g = saturacao(g_ss);
  imagem_clonada -> pixels[i][j].b = saturacao(b_ss);
}
// ----------------------------------------------Funcoes para adicionar pixels ja saturados na imagem----------------------------------------------------------//

Imagem* DeteccaoBordas(Imagem* imagem_original) { // funcao para alocar os pixels ja saturados na imagem
  Imagem* imagem_clonada = criar_imagem(imagem_original -> largura, imagem_original -> altura);// ser� criada uma imagem clonada com os parametros de largura e altura da imagem original
  int largura = imagem_original -> largura;
  int altura = imagem_original -> altura;
  int i,j;
  for (i =1; i < altura -1 ; ++i) {// for para colocar os pixels saturados em seu respectivos lugares
    for (j =1; j < largura -1 ; ++j) {
      DetecBordCalculo(imagem_clonada, imagem_original, i, j);// funcao que ser� chamada para se fazer todo o calculo que levar� a imagem ter o filtro de detec��o de bordas
    }
  }
  return imagem_clonada;
}

Imagem* blurring(Imagem* imagem_original) {// funcao para alocar os pixels ja saturados na imagem
  Imagem* imagem_clonada = criar_imagem(imagem_original -> largura, imagem_original -> altura);
  int largura = imagem_original -> largura;
  int altura = imagem_original -> altura;
  int i,j;
  for (i = 1; i < altura - 1; ++i) {// for para colocar os pixels saturados em seu respectivos lugares
    for (j = 1; j < largura - 1; ++j) {
      BlurCalculo(imagem_clonada, imagem_original, i, j);// funcao que ser� chamada para se fazer todo o calculo que levar� a imagem ter o filtro
    }
  }
  return imagem_clonada;
}

Imagem* sharpen(Imagem* imagem_original) {// funcao para alocar os pixels ja saturados na imagem
  Imagem* imagem_clonada = criar_imagem(imagem_original -> largura, imagem_original -> altura);
  int largura = imagem_original -> largura;
  int altura = imagem_original -> altura;
  int i,j;
  for (i = 1; i < altura - 1; ++i) {// for para colocar os pixels saturados em seu respectivos lugares
    for (j = 1; j < largura - 1; ++j) {
      sharpeningCalculo(imagem_clonada, imagem_original, i, j);// funcao que ser� chamada para se fazer todo o calculo que levar� a imagem ter o filtro
    }
  }
  return imagem_clonada;
}
//---------------------------------------------------------------------funcoes para rotacao  de imagem----------------------------------------------//
Imagem* rotacionar90(Imagem* imagem_original) { // fun��o criada para rotacionar  a imagem 90 graus
  int largura = imagem_original -> largura;
  int altura = imagem_original -> altura;
  int i, j;
  Imagem * novaimg = criar_imagem(altura, largura);// uma nova imagem � criada , sendo clone da original
  for (i = 0; i < altura; ++i) {
    for (j = 0; j < largura; ++j) {
    	// nesse for os pixels serao colocados com a linha
		//come�ando do final e a colunar normal , com isso vai se dar o efeito de 90 graus na imagem
      novaimg -> pixels[j][(altura - 1) - i] = imagem_original -> pixels[i][j];
    }
  }
  return novaimg;
}
// funcao para rotacionar a imagem 180graus ,
//que n�o � muito misterio, pois so � necessario aplicar
//o rotacionar90 duas vezes para que se consiga os 180 graus
Imagem* Rotacionar180(Imagem* imagem_original) { 
  int largura = imagem_original -> largura;
  int altura = imagem_original -> altura;
  Imagem* nova = criar_imagem(altura,largura);
  nova= rotacionar90(rotacionar90(imagem_original)); // aplica se a funcao rotacionar90 duas vezes para que a imagem gire 180 graus
  return nova;
}
Imagem* Rotacionar270(Imagem* imagem_original) {// funcao para rotacionar 270 graus , onde so e preciso aplicar um rotacionar90 dentro de um rotacionar 180
  int largura = imagem_original -> largura;
  int altura = imagem_original -> altura;
  Imagem* nova = criar_imagem(altura,largura);
  nova= Rotacionar180(rotacionar90(imagem_original));// aplicasse o rotacionar 90 dentro do rotacionar180
  return nova;
}

Imagem* EspelharImagem(Imagem* imagem_original) {
  int largura = imagem_original -> largura;
  int altura = imagem_original -> altura;
  int i, j;
  Imagem* novaimg = criar_imagem(altura, largura);
  for (i = 0; i < altura; ++i) {
    for (j = 0; j < largura; ++j) {
      novaimg -> pixels[j][(altura - 1) - i] = imagem_original -> pixels[j][i];
    }
 }
 return novaimg;
}

Imagem * ler_imagem(char * nome_arquivo) {
  int largura;
  int altura;
  int maximo;
  char P3[3];
  int i,j,r,g,b;
  FILE * arquivo;
  Imagem * img;
  arquivo = fopen(nome_arquivo, "r"); // ler o arquivo, por isso foi usado a letra r de read
  fscanf(arquivo, "%s", P3);
  fscanf(arquivo, "%i %i", &largura, &altura);// far� a leitura da largura e altura da imagem, ou seja, sua resolucao
  fscanf(arquivo, "%i", &maximo);// ler� o valor maximo da qualidade da imagem , no caso 255.
  img = criar_imagem(largura, altura); // vai criar a imagem, tendo como parametros a altura e a largura da mesma
  for (i = 0; i < altura; ++i) {
    for (j = 0; j < largura; ++j) {
      fscanf(arquivo, "%i %i %i", & r, & g, & b);
      img -> pixels[i][j].r = r;
      img -> pixels[i][j].g = g;
      img -> pixels[i][j].b = b;
    }
  }
  fclose(arquivo);
  return img;
}

void Salvar_Imagem(Imagem* img, char* nome_arquivo) {// funcao feita para salvar a imagem
  int r, g, b, i, j;
  FILE * arquivo;
  arquivo = fopen(nome_arquivo, "w"); // abrir� o arquivo e o gravar�
  fprintf(arquivo, "P3\n"); // imprimir� a primeira linha
  fprintf(arquivo, "%i %i\n", img -> largura, img -> altura); // imprimir�  a segunda linha com a resolucao
  fprintf(arquivo, "255\n"); // imprimir� a terceira linha com a qualidade da imagem
  //for para percorer a matriz  de linha e coluna
  //para a impressao , cada r ,g e b receber� os
  //pixels da nova imagem e ser�o impressos posteriormente
  for (i = 0; i < img -> altura; i++) { 
    for (j = 0; j < img -> largura; j++) {
      r = img -> pixels[i][j].r;
      g = img -> pixels[i][j].g;
      b = img -> pixels[i][j].b;
      fprintf(arquivo, "%i %i %i\n", r, g, b);
    }
  }
  fclose(arquivo); // fechar o arquivo
}
// Funcao main que apenas servira para colocar o usuario
//em frente ao menu e pedir� que escolha o nome da imagem
//a ser modifica e qual modificacao voce deseja fazer nela
int main() { 
  Imagem* img;// foram criadas quatro variaveis apenas para facilitar a compreensao do codigo na hora da chamada de funcoes
  Imagem* filtro;
  Imagem* rotacionar;
  Imagem* zoom;
  char arquivo[40];// servir� como parametro para que a fun��o ler imagem saiba a qual imagem o usuario esta se referenciando , para que o programa a leia
  int opcao;
  
 do{

  printf("Digite o nome do arquivo que vc quer abrir com sua respectiva extensao\n");
  scanf("%s", &arquivo);
  img = ler_imagem(arquivo); // imagem ja clonada
  printf("Digite Qual modificao voce deseja aplicar naimagem:");
  printf("\n 1- blur ;\n 2 - deteccao de bordas;\n 3- sharpening ;\n 4-rotacionar 90 graus \n 5-rotacionar 180 graus\n 6-rotacionar 270 graus\n 7-Espelhar\n 8-reduzir2x \n 9-reduzir4x\n 10-reduzir8x\n 11-zoom2x\n 12-zoom4x\n 13-zoom8x\n 14-binarizacao\n sair-0\n");
  scanf("%i", &opcao);
// dependendo do tipo de opcao escolhida pelo usuario , a transforma��o na imagem ser� aplicada
  if (opcao == 1) {
    filtro = blurring(img);
    Salvar_Imagem(filtro, "lena3.ppm");
    printf("Imagem gerada com sucesso!\n");
  } else if (opcao == 2) {
    filtro = DeteccaoBordas(img);
    Salvar_Imagem(filtro,"lena3.ppm");
    printf("Imagem gerada com sucesso!\n");
  } else if (opcao == 3) {
    filtro = sharpen(img);
    Salvar_Imagem(filtro, "lena3.ppm");
    printf("Imagem gerada com sucesso!\n");
  } else if (opcao == 4) {
    rotacionar = rotacionar90(img);
    Salvar_Imagem(rotacionar, "lena3.ppm");
    printf("Imagem gerada com sucesso!\n");
  } else if (opcao == 5) {
    rotacionar = Rotacionar180(img);
    Salvar_Imagem(rotacionar, "lena3.ppm");
    printf("Imagem gerada com sucesso!\n");
  }else if (opcao == 6){
	 rotacionar = Rotacionar270(img);
    Salvar_Imagem(rotacionar, "lena3.ppm");
    printf("Imagem gerada com sucesso!\n");
  } else if (opcao == 7) {
    rotacionar = EspelharImagem(img);
    Salvar_Imagem(rotacionar, "lena3.ppm");
    printf("Imagem gerada com sucesso!\n");
  } else if (opcao == 8) {
    zoom= reduzir2x(img);
    Salvar_Imagem(zoom, "lena3.ppm");
    printf("Imagem gerada com sucesso!\n");
  } else if (opcao == 9) {
    zoom = reduzir4x(img);
    Salvar_Imagem(zoom, "lena3.ppm");
    printf("Imagem gerada com sucesso!\n");
  }else if (opcao == 10){
  	 zoom = reduzir8x(img);
    Salvar_Imagem(zoom, "lena3.ppm");
    printf("Imagem gerada com sucesso!\n");
  }else if (opcao == 11){
	 zoom = zoom2x(img);
    Salvar_Imagem(zoom, "lena3.ppm");
    printf("Imagem gerada com sucesso!\n");
  }else if (opcao==12){
 	 zoom = zoom4x(img);
    Salvar_Imagem(zoom, "lena3.ppm");
    printf("Imagem gerada com sucesso!\n");
  }else if (opcao==13){
	 zoom = zoom8x(img);
    Salvar_Imagem(zoom, "lena3.ppm");
    printf("Imagem gerada com sucesso!\n");
  }else if (opcao==14){
	 filtro = binarizacao(img);
    Salvar_Imagem(filtro , "lena3.ppm");
    printf("Imagem gerada com sucesso!\n");
  } else {
    printf("digite uma opcao valida\n");
  }
}while (opcao!=0);
  return 0;
}
