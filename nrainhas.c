// Autor: Gabriel Luiz F. Souto
// Local: São Carlos - SP, Brasil. USP São Carlos, ICMC
// Titulo: Algoritmo genetico pra N-Rainhas (melhor para 4 ou 8)
// Descricao: Algoritmo genetico que resolve o problema das n-rainhas dada a dimensao do tabuleiro

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>

#define NUMPOPU 15


int **populacao;
int total, n, melhorpopu;		

int recebeDimensao(){
	int n;

	printf("Digite a dimensao do tabuleiro (quantidade de rainhas): \n");
	scanf("%d", &n);

	if(n%4 != 0){
		printf("Impossive de resolver com essa dimensao\n");
		exit(0);
	}

	return n;
}

int **iniciaPopulacao(){
	int i, j;

	//cria 4 vetores para a populacao
	int **populacao = (int **) malloc(sizeof(int *) *NUMPOPU);
	for (i = 0; i < NUMPOPU; i++){
		populacao[i] = (int *) malloc(sizeof(int) *n);
	}

	//atribui valore aleatorios para a populacao
	srand( time(NULL) );
	for (i = 0; i < NUMPOPU; i++){
		for (j = 0; j < n; j++){
			populacao[i][j] = (rand()%n);
		}
	}

	return populacao;
}

void mostraPopulacao(int **populacao){
	int i, j;

	for (i = 0; i < NUMPOPU; i++){
		printf("pop[%d]:\n", i);
		for (j = 0; j < n; j++){
			printf("%d ", populacao[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


int detectarHits(int *populacao, int pos){		//pos eh posicao que vai ser analisada
	int i, hits = 0;

	//verifica se tem na linha
	i = pos;
	for (i; i < n; i++){
		//verifica se nao eh a propria peca que esta sendo analisada
		if (i != pos){
			if (populacao[pos] == populacao[i]){
				hits = hits+2;
			}
		}
	}

	//verifica se tem na diagonal
	i = pos;
	for (i; i < n; i++){
		if (i != pos){
			//calculo pra ver se ta na diagonal. Ja faz da ida e da volta pra só verificar os que estao na frente e depois n precisar verificar pra tras da posicao que ta no momento
			if (abs(populacao[pos]-populacao[i]) == abs(pos-i)){
				hits = hits+2;	
			}

		}
	}

	return hits;
}

void mostrarFitness(int **populacao){
	int i;

	for (i = 0; i < NUMPOPU; i++){
		printf("fitness da populacao %d: %d\n", i, fitness(populacao[i]));
	}
}

int fitness(int *populacao){
	int i, fit, hits=0;

	//total de pontos = n*(n-1)
	int total = n*(n-1);

	for (i = 0; i < n; i++){
		//detectar numero de hits
		hits += detectarHits(populacao, i);
	}

	//fitness = total de pontos - numero de hits
	fit = total - hits;

	return fit;
}

int selecionaMelhor(int **populacao){
	int i, j;
	int melhorindice = 0;

	for (i = 0; i < NUMPOPU; i++){
		if (fitness(populacao[i]) > fitness(populacao[melhorindice])){
			melhorindice = i;
		}
	}

	return melhorindice;
}

//Cruzamento Aritmético - é realizada uma operação aritmética para obter a nova geração Cruzamento Aritmético - é realizada uma operação aritmética para obter a nova geração 
//melhorpopu eh o indice da melhor populacao do vetor de vetores
void crossovereMutacao(int **populacao){
	int i, j, k, ponto;
	


	for (i = 0; i < NUMPOPU; i++){
		for (j = 0; j < n; j++){

			if (i != melhorpopu){
				//crossover aritmetico
				populacao[i][j] = (int) ( (populacao[melhorpopu][j] + populacao[i][j])/2 );
			}
		}
	}

	
	// mutar populacao
	for (k = 0; k < NUMPOPU; k++){				
		//se o indice aleatorio nao for o melhor de todos entao muta 1/4 dos genes
		if (k != melhorpopu){
			for (j = 0; j < n/4; j++){		
				int numaleatorio = rand()%n;
				populacao[k][numaleatorio] = rand()%n;
			}
		}
	}
}


void draw(void){
	int i;

    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(30.0f); //point size
    glLineWidth(5.0f); //line width

    //linhas verticais
	glBegin(GL_LINES);
		float medida = 100.0/n;
        glColor3f(1.0f, 1.0f, 1.0f);
        for (i = 0; i < n; i++){
        	glVertex2f(medida*i, 0);
        	glVertex2f(medida*i, 100);
        }
	glEnd();

	//linhas horizontais
	glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        for (i = 0; i < n; i++){
        	glVertex2f(0, medida*i);
        	glVertex2f(100, medida*i);
        }
	glEnd();  

	//desenha rainhas
    glBegin(GL_POINTS);
    	glColor3f(0.8f, 0.0f, 0.0f); //define red
        for (i = 0; i < n; i++){
        	int x = medida*(i+1) - medida/2;
        	int y = medida*(populacao[melhorpopu][i]+1)- medida/2 ;
        	glVertex2f(x , y);
        }

    glEnd();
    glFlush();
}

void mutaTudo(int **populacao){
	int k, j;

	for (k = 0; k < NUMPOPU; k++){				
		if (k != melhorpopu){
			for (j = 0; j < n; j++){		
				int numaleatorio = rand()%n;
				populacao[k][numaleatorio] = rand()%n;
			}
		}
	}
}

int main(int argc, char *argv[]){
	int tecla, eh_fim = 0;
	int count=0, rep = 1000; //variaveis usadas pra verificar se ta repetindo muito a melhor de todas
	int i, ger=0;

	n = recebeDimensao(); //fazer verificação se é potencia de 4

	total = n*(n-1);

	populacao = iniciaPopulacao();
	mostraPopulacao(populacao);


	while(eh_fim != 1){
		//para iteracoes para ver como andam os resultados
		if (ger%1000 == 0){
				// Ideia de mutar tudo se estiver repetindo muito o melhor de todos
				// if (melhorpopu == rep){
				// 	count++;
				// } else {
				// 	count = 0;
				// }			

				// if (count == 10){
				// 	mutaTudo(populacao);
				// 	count = 0;
				// 	mostraPopulacao(populacao);				
				// }
				//rep = melhorpopu;
			printf("melhor: %d\n", melhorpopu);
			mostraPopulacao(populacao);
			//scanf("%d", &tecla);
		}
		printf("geracao %d\n", ger);

		//seleciona melhor individuo da populacao
		melhorpopu = selecionaMelhor(populacao);
		// printf("melhor: %d\n", melhorpopu);
		crossovereMutacao(populacao);

		//verifica criterio de parada
		if (fitness(populacao[melhorpopu]) == total){
			eh_fim = 1;
			mostraPopulacao(populacao);
			mostrarFitness(populacao);
		}

		ger++;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(500, 500);
	
	glutCreateWindow("N-Rainhas");
	glutDisplayFunc(draw);

	gluOrtho2D(0, 100, 0, 100);

	glutMainLoop();


}

// passos
// 1 - inicia população
// 2 - seleciona o melhor da populacao
// 3 - crossover
// 4 - mutacao
// 5 - gera nova populacao  e volta pro 2

