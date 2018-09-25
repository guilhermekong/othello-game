#include <stdio.h>   //para as funções de ler do teclado e escrever no terminal
#include <math.h>    //para se usar funções matemáticas
#include <stdlib.h>  //para se usar funções matemáticas e outras
# include <string.h> //para se usar funções de tratamento de cadeias de caracteres
#include <time.h>    //para se usar funções sobre contagem de tempo
#include <limits.h>  //para sabermos as constantes de domínio dos tipos C
#include <ctype.h>   //para se usar funções de tratamento de caracteres 
#include <stdbool.h> //para usar os valores booleanos - os literais C true e false
#include "pg/graphics.h"


typedef struct circulo {
	int x, y;
	int jogada;
} circulo;


///-------- configuração de valores iniciais de operação na "consola grafica" --------
#define LARGURA 8 /// largura do array bidimensional tab
#define ALTURA  8 /// altura  do array bidimensional tab
#define RAIO_GRANDES 23  ///raio das bolas que vao ser jogadas
#define RAIO_PEQS 7  ///raio das bolas que sugerem as jogadas

circulo tab [ALTURA][LARGURA];


///===================== COMPOSIÇÃO DAS BOLAS ==========================
void bola_grandes(int x, int y, RGB estado){
	
	graph_circle( 235 + (x * 50) + 25, 75 + (y * 50) + 25, RAIO_GRANDES, estado, true);	}

void bola_peqs(int x, int y, RGB estado){
	
	graph_circle( 235 + (x * 50) + 25, 75 + (y * 50) + 25, RAIO_PEQS, estado, true);}

///===================== ASSINATURAS das funções ================================
void iniciar_tabuleiro ();
void desenhar_tab ();
void print_tab();

int coluna_toca();
int linha_toca();
void elimina();

int cronometro();

void moverRato();

void chamarFuncoesAoRitmoDe();
void atuarTeclado();

///=== variáveis GLOBAIS: as funções que operam eventos têm que usar variaveis globais ==

int jogador_1 = 1;
int x, y;
int segundos, minutos, horas;
int n = 0;
char tempo[9];

bool fim = false;
bool pausa=false;


///==================desenhar o tabuleiro ==========================



void desenhar_tab (){
	
	graph_rect(0, 0, 700, 500, c_black, true);      ///fundo do terminal grafico (preto)
	graph_rect(235, 75, 401, 400, c_blue, true);     ///fundo do local de jogo (azul)
   
    graph_circle(50, 400, 40, c_white, true);        ///bola inferior com fundo branco
	graph_circle(50, 400, 40, c_blue, false);        ///contorno da bola (azul)

	graph_circle(150, 400, 40, c_black, true);      ///bola inferior com fundo preto
	graph_circle(150, 400, 40, c_blue, false);       /// contorno da bola (azul)
	
	graph_text( 40, 30, c_white, "BOM JOGO AOS DOIS! ", MEDIUM_FONT);
	
	graph_text( 40, 120, c_white, "OTHELLO", LARGE_FONT);   ///escreve othello no terminal grafico (branco)
   
  
    graph_text( 40, 300, c_white, "press : ", MEDIUM_FONT);               
    graph_text( 40, 315, c_white, "p/P-pause game ", MEDIUM_FONT);     /// informa ao utilizador em que tecla clicar para pausar o tempo
    graph_text( 40, 330, c_white, "q/Q - finish game ", MEDIUM_FONT);  ///informa o utilizador em que teclaclicar para terminar o jogo
    
    int a = 235;
	for(int i=0; i<9; i++){
		graph_line(a, 75, a , 475, c_white);
		a+=50;}
	
	int b=75;
	for(int i=0; i<9; i++){
		graph_line(235, b, 635 , b, c_white);
		b+=50;}
	
///............ INICIA LOGO COM AS BOLAS GRANDES NO MEIO DO TABULEIRO...............
	
              graph_circle( 410, 300, RAIO_GRANDES, c_black,true);               
              graph_circle( 410, 250, RAIO_GRANDES, c_white,true);
              graph_circle( 460, 300, RAIO_GRANDES, c_white,true);
              graph_circle( 460, 250, RAIO_GRANDES, c_black,true);
    
/// ........................INICIAR COM AS BOLAS PEQUENAS ............................... 

                graph_circle( 460, 350, RAIO_PEQS, c_black,true);
                graph_circle( 510, 300, RAIO_PEQS, c_black,true);
                graph_circle( 410, 200, RAIO_PEQS, c_black,true);
                graph_circle( 360, 250, RAIO_PEQS, c_black,true);
}

///============= Sobre a matriz do standart output ============

void iniciar_tabuleiro (){
	
  tab[3][4].jogada=1;          /// os 1 indicam as bolas pretas
  tab[4][3].jogada=1;            
  tab[3][3].jogada=2;
  tab[4][4].jogada=2;            /// os 2 indicam as bolas brancas
  tab[3][2].jogada=3;
  tab[2][3].jogada=3;             
  tab[4][5].jogada=3;           /// os 3 indicam  as bolas de sugestao de jogadas
  tab[5][4].jogada=3;		
}

void print_tab(){
	for( int i = 0; i < 8; i++){
	 for( int j = 0; j < 8; j++ ){
		 printf("%d ", tab[i][j].jogada);}
		   printf("\n");}
		   printf("\n");
}

void Mover(){         ///nesta funçao vamos ver as jogadas possiveis
	int i,j;
	int indice_i,indice_j;
	int find1=0;
	int guarda_i,guarda_j;             /// guarda o indice i ou j de uma jogada possivel
    int check = 0;                     /// ve os limites 
    int inv_i,inv_j;
    
	for( i = 0; i < 8; i++ ) 
	for ( j=0; j<8; j++){
		
	  if (jogador_1 == 1 && tab[i][j].jogada == 2)
		for(indice_i=i-1;indice_i<=i+1;indice_i++)
		for(indice_j=j-1;indice_j<=j+1;indice_j++){
			
			if(tab[indice_i][indice_j].jogada==1){
			 find1=1;}
			if (find1){
			    find1=0;
				   
				inv_i=i-indice_i;
				inv_j=j-indice_j;
				guarda_i=i+inv_i;
			    guarda_j=j+inv_j;
				   
			while(tab[guarda_i][guarda_j].jogada!=0 && tab[guarda_i][guarda_j].jogada!=1){
				if ((guarda_i==0 || guarda_i== 7) && inv_i!=0){
					check=1;
					break;}
							
			else 
				if ( (guarda_j==0  || guarda_j==7) && inv_j!=0){
					 check=1;
					 break;}
								  
					 guarda_i=guarda_i+inv_i;
					 guarda_j=guarda_j+inv_j;}	
						
				if (check==0 && tab[guarda_i][guarda_j].jogada==0){
					tab[guarda_i][guarda_j].jogada=3;
					bola_peqs(guarda_i,guarda_j,c_black);}              ///faz referencia a funcao bolas_peqs
					check=0;}
					}
			else if (jogador_1==2 && tab[i][j].jogada==1){
				for(indice_i=i-1;indice_i<=i+1;indice_i++)
				for(indice_j=j-1;indice_j<=j+1;indice_j++){
					
				  if(tab[indice_i][indice_j].jogada==2) find1=1;
				  if (find1){
					  find1=0;
							
					  inv_i=i-indice_i;
				   	  inv_j=j-indice_j;
					  guarda_i=i+inv_i;
					  guarda_j=j+inv_j;
							
			while(tab[guarda_i][guarda_j].jogada!=0 && tab[guarda_i][guarda_j].jogada!=2){
				if ((guarda_i==0 || guarda_i== 7) && inv_i!=0){
					check=1;
					break;}
					
				else 
				    if ( (guarda_j==0  || guarda_j==7) && inv_j!=0){
					    check=1;
					    break;}
								
					    guarda_i=guarda_i+inv_i;
					    guarda_j=guarda_j+inv_j;}
					
				 if(check ==0 && tab[guarda_i][guarda_j].jogada==0){
					tab[guarda_i][guarda_j].jogada=3;
				    bola_peqs(guarda_i,guarda_j,c_white);}  ///Faz referencia a funçao bolas pequenas
					
					 check=0;}
					}
		}
	}
}

void chamarFuncoesAoRitmoDe() {
  if( pausa ) return;  }              /// a variavel pausa a true/false pára/avança de chamar as funções seguintes 

///====== quando uma tecla do teclado é pressionada=======

void atuarTeclado(KeyEvent ke){
	if(ke.state==KEY_PRESSED){
		
///............termina o jogo ..................
		
		if(ke.keysym == 'Q' || ke.keysym == 'q'){
			graph_exit();
		}
		
///............. faz pausa ao jogo ................
		
		if(ke.keysym == 'P' || ke.keysym == 'p'){
			
			 pausa=!pausa;}
		}
	}
	
void elimina(){
	for( int i = 0; i < 8; i++ )
		for( int j = 0; j < 8; j++)
			if (tab[i][j].jogada==3) {
				tab[i][j].jogada=0;
				bola_grandes (i,j,c_blue);}
}

int linha_toca(int y){               ///serve para obter a linha em queroto clica
	return 	((y - 75) / 50);
}

int coluna_toca(int x){              ///serve para obter a coluna em que o rato clica
	return  ((x - 235) / 50);
}

int mouse_check=1;      ///ve os se o clique e valido

/// ============operações sobre os eventos de rato ================

void moverRato(MouseEvent me){
	
	int guarda_i,guarda_j;           
	int indice_i,indice_j;
	int inv_j,inv_i;
	int find1=0;
	int check=0;
	
 if(fim == true) return;
	
	///apenas estamos interessados em eventos do rato e seus clicks de botões: LEFT, RIGHT
	
 if( me.type==MOUSE_BUTTON_EVENT && me.state==BUTTON_CLICK && me.button==BUTTON_LEFT ){
	if( me.x >= 235 && me.x <= 700 && me.y >= 75 && me.y <= 500 ){		///este if valida o click apenas dentro do tabuleiro
		x = coluna_toca(me.x);
		y = linha_toca(me.y);
		mouse_check=1;
		
	if(tab[x][y].jogada != 3) return;
	   elimina ();		
	   printf("x:%d\ny:%d\n", x, y);
	   printf("\n");
			
	if(jogador_1 == 1){
	   bola_grandes (x,y,c_black);
	   tab[x][y].jogada = jogador_1++;
				
  for(indice_i=x-1;indice_i<=x+1;indice_i++)
  for(indice_j=y-1;indice_j<=y+1;indice_j++){
	if(tab[indice_i][indice_j].jogada==2) find1=1;
	if (find1){
		find1=0;
		
		inv_i=indice_i-x;
		inv_j=indice_j-y;
		guarda_i=indice_i+inv_i;
		guarda_j=indice_j+inv_j;
							
    while(tab[guarda_i][guarda_j].jogada!=1){
	  if ((guarda_i==0 || guarda_i== 7) && inv_i!=0){
		   check=1;
		   break;}
	 else
	     if ( (guarda_j==0  || guarda_j==7) && inv_j!=0){
			   check=1;
			   break;}
			   
			   guarda_i=guarda_i+inv_i;
			   guarda_j=guarda_j+inv_j;}	
					
   while((guarda_i!=x || guarda_j!=y) && check!=1){
		  guarda_i=guarda_i-inv_i;
		  guarda_j=guarda_j-inv_j;
		  tab[guarda_i][guarda_j].jogada=1;	
		  bola_grandes(guarda_i,guarda_j,c_black);}
				
			check=0;}
				} 
		}
		else 
		     if(jogador_1 == 2){
				bola_grandes(x,y,c_white);
				tab[x][y].jogada = jogador_1--;
				
  for(indice_i=x-1;indice_i<=x+1;indice_i++)
  for(indice_j=y-1;indice_j<=y+1;indice_j++){
    if(tab[indice_i][indice_j].jogada==1) find1=1;
	if (find1){
		find1=0;
		inv_i=indice_i-x;
		inv_j=indice_j-y;
		guarda_i=indice_i+inv_i;
		guarda_j=indice_j+inv_j;
   while(tab[guarda_i][guarda_j].jogada!=2){
	if ((guarda_i==0 || guarda_i== 7) && inv_i!=0){
		 check=1;
		 break;}
	else
	    if ( (guarda_j==0  || guarda_j==7) && inv_j!=0){
			  check=1;
			  break;}
			  
		      guarda_i=guarda_i+inv_i;
		      guarda_j=guarda_j+inv_j;
							}	
						
   while((guarda_i!=x || guarda_j!=y) && check!=1){
		  guarda_i=guarda_i-inv_i;
		  guarda_j=guarda_j-inv_j;
		  tab[guarda_i][guarda_j].jogada=2;	
		  bola_grandes(guarda_i,guarda_j,c_white);}
		  
		  check=0;}
		     } 
		}
		   print_tab();
		   Mover();	
		}
	}
}



/// ============ TEMPO DE JOGO ==================

void contador(){
	
	if(fim == true) {
	  return;}
		
		segundos++;

	if (segundos == 60) {
		  segundos = 0;
		  minutos++;
	if (minutos == 60 ) 
		  horas = (horas + 1) % 60;
		}
		
		sprintf( tempo,"%02d:%02d:%02d\r", horas, minutos, segundos);
		
		fflush(stdout);          /// esta função força os caracteres a serem escritos na consola              
		
	    graph_rect(470,25,300,25,c_black,true);        
		graph_text(480,50,c_white,tempo,LARGE_FONT);    /// mostra o tempo no terminal grafico
		graph_text( 480, 22, c_white, "TEMPO DE JOGO: ", MEDIUM_FONT);
}	



int main(void){
	
	printf("Os numeros 1 representam as peças pretas\n");            ///escreve no standard output
	printf("Os numeros 2 representam as peças brancas\n");
	printf("\n");
	
	iniciar_tabuleiro();
	
///iniciar a consola gráfica =======================	
graph_init();

	desenhar_tab();
	print_tab();

	while(1){
		
	graph_regist_timer_handler(contador, 1000);
	
	graph_regist_mouse_handler( moverRato );     /// chamada quando o rato mexe e trata eventos de rato	
	
	graph_regist_key_handler( atuarTeclado );  ///trata eventos de teclado e é chamada quando o teclado mexe
	
	  if (mouse_check){
		  mouse_check=0;
		  
graph_start();}
	}
	
	return 0;}
