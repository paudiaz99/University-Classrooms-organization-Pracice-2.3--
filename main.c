/****************************************************************************
*
* @Propòsit: Creació d'un sistema que emmagatzema a la memòria una serie de dades d'uns graus universitaris, llegides 
			 de fitxers, i seguidament mostra al usuari una serie d'opcions per treballar amb aquestes.
* @Autor/s: Pau Díaz Cuesta
* @Data de creació:  24 de Abril de 2022
* @Data de l’última modificació: 25 de Abril de 2022
*
* ***************************************************************************/

// Llibreries del sistema
#include <stdlib.h>				
#include "linkedlist.h"
#include <stdio.h>
#include <string.h>


//Tipus propis


typedef struct {
char name[MAX_STRING_LENGTH]; 
int current_capacity;
LinkedList students;
} Classroom;

typedef struct {
	char name[MAX_STRING_LENGTH]; 
	int num_classrooms; 
	Classroom *classrooms;
} Degree;

typedef struct { 
	int num_degrees;
    Degree *elements;
} Degrees;

/*********************************************** 
*
* @Finalitat: Comprovar si s'ha obert correctament un fitxer.

* @Paràmetres: in/out: correct = punter a enter on s'emmagatzema el valor que determina si el fitxe és correcte (1/0)
			   in: f = punter que conté la direcció del fitxer obert	
* @Retorn: ----
*
* **********************************************/
void correctFile(int *correct, FILE *f){
	// Comprova si el fitxer s'ha llegit correctament.
	if(f == NULL){
		*correct = 0;
	}
	else{
		*correct = 1;
	}
}

/*********************************************** 
*
* @Finalitat: Llgir el primer fitxer i a la vegada crear l'estructura desitjada a la memòria.

* @Paràmetres: in: f1 = Punter a FILE que conté la direcció del fitxer obert
			   in/out: d = Punter a Punter a Degree que permet modificar el contingut de "d" fora del main.
* @Retorn: ----
*
* **********************************************/
void readFileOne(FILE *f1, Degrees **d){
	int i = 0, j= 0;						// Variables per als bucles for.
	char trash;								// Variable de tipus caracter per netejar el buffer.

	//Llegeixo el numero de graus que hi ha al fitxer.
	fscanf(f1, "%d", &((*d)->num_degrees));
	//Netejo el buffer per evitar errors.
	fscanf(f1, "%c", &trash);
	
	// Reservo memòria per a la quantitat de graus llegida anteriorment.
	(*d)->elements = (Degree *) malloc(sizeof(Degree)*((*d)->num_degrees));

	// Faig un bucle while per a llegir tota la informació de tots els graus de fitxer.
	while(i < ((*d)->num_degrees)){
		// LLegeixo la informació del grau.
		fscanf(f1, "%d", &((*d)->elements[i].num_classrooms));
		fscanf(f1, "%c", &trash);
		fgets((*d)->elements[i].name, MAX_STRING_LENGTH, f1);
		// Elimino el \n.
		(*d)->elements[i].name[strlen((*d)->elements[i].name)-1] = '\0';

		// Reservo memòria per a la quantitat de classes llegida anteriorment.
		(*d)->elements[i].classrooms = (Classroom *) malloc(sizeof(Classroom)*((*d)->elements[i].num_classrooms));

		//Faig un bucle for per llegir la informació de les classes.
		for(j=0;j<((*d)->elements[i].num_classrooms);j++){
			fscanf(f1, "%s", (*d)->elements[i].classrooms[j].name); 

			// Creo una llista de usuaris per a cada classe.
			(*d)->elements[i].classrooms[j].students = LINKEDLIST_create();
		}
		i++;
	}
}

/*********************************************** 
*
* @Finalitat: Cerca la posició en la que es troba el grau que introdueix l'usuari
			  i a part també retorna de forma indirecta quina és la primera classe 
			  sense omplir i quina posició és la que ha d'ocupar l'estudiant.

* @Paràmetres: in: degree = cadena on està el nom del grau.
			   in: d = Punter a Degrees on està emmagatzemada tota la informació.
			   in/out: pos_classroom = Punter a enter on s'emmagatzema la direcció
			   	   de la posició de la classe que ha d'ocupar l'estudiant.
			   in/out: current_pos = Punter a enter on s'emmagatzema la direcció
			   	   de la posició que ha d'ocupar l'estudiant dintre la classe.

* @Retorn: pos_degree = posició de l'array dinàmica on es troba el grau del estudiant.
*
* **********************************************/
int searchPosition(char degree[], Degrees **d, int *current_pos){
	int i = 0;							// Variable per al bucle for.
	int pos_degree = 0;					// Posició de l'array dinàmica on està el grau introduit.
	int continua = 1;					// Variavle flag per al bucle while.

	// Faig un bucle for per trobar la posició del grau a l'array dinàmica.
	for(i=0;i<(*d)->num_degrees;i++){
		if(strcmp(degree, (*d)->elements[i].name) == 0){
			pos_degree = i;
		}
	}
	// Resetejo la i.
	



	return(pos_degree);
}

/*********************************************** 
*
* @Finalitat: Llegir el segon fitxer amb els estudiants i emmagatzemar-lo a la memòria de forma ordenada.

* @Paràmetres: in: f2 = punter a FILE que conté la direcció del fitxer obert.
			   in/out: d = punter a Punter a Degree que permet modificar el contingut de "d" fora del main.
* @Retorn: ----
*
* **********************************************/
void readFileTwo(FILE *f2, Degrees **d){
    char degree[MAX_STRING_LENGTH];
	char trash;										// Variable per netejar el buffer.
	// int class_position = 0;							// Variable que es farà servir per emmagatzemar el estudiant a la classe disponible.
	int pos_degree = 0;								// Variable on es guardrà la posició del grau.
	int current_pos = 0;							// Variable que es farà servir per emmagatzemar el estudiant a la possició disponible.
	Student aux_student;							// Variable auxiliar per a llegir els estudiants de la llista

	// Llegeixo la informació de fitxer eliminant els \n.
	fscanf(f2, "%s", aux_student.name);
	aux_student.name[strlen(aux_student.name)-1] = '\0';
	fscanf(f2, "%c", &trash);
	fgets(degree, MAX_STRING_LENGTH, f2);
	degree[strlen(degree)-1] = '\0';
	fscanf(f2, "%s", aux_student.login);
	
	// Crido a la funció search position per trobar la posició del grau, la classe disponible i la posició on es guardarà l'estudiant. 
	pos_degree = searchPosition(degree, d, &current_pos);

	// Afegeixo a la lliste el estudiant llegit del arxiu.
    (*d)->elements[pos_degree].classrooms[0].current_capacity++;
	LINKEDLIST_add((*d)->elements[pos_degree].classrooms[0].students, aux_student);
	
	// Creo un bucle while per llegir el fitxer fins que aquest acabi.
	while(!feof(f2)){
	
	// Llegeixo la informació de fitxer eliminant els \n.
	fscanf(f2, "%s", aux_student.name);
	aux_student.name[strlen(aux_student.name)-1] = '\0';
	fscanf(f2, "%c", &trash);
	fgets(degree,MAX_STRING_LENGTH, f2);
	degree[strlen(degree)-1] = '\0';
	fscanf(f2, "%s", aux_student.login);

	// Crido a la funció search position per trobar la posició del grau, la classe disponible i la posició on es guardarà l'estudiant. 
	pos_degree = searchPosition(degree, d, &current_pos);

	// Afegeixo a la lliste el estudiant llegit del arxiu.
    (*d)->elements[pos_degree].classrooms[0].current_capacity++;
	LINKEDLIST_add((*d)->elements[pos_degree].classrooms[0].students, aux_student);
	}
}

/*********************************************** 
*
* @Finalitat: Mostrar les dades llegides anteriorment de forma ordenada per graus.
* @Paràmetres: in: d = Punter a degrees on es troba la direcció de tota la estructura creada previament.
* @Retorn: ----
*
* **********************************************/
void summaryOption(Degrees *d){
	int j = 0, k = 0;					// Variables per als bucles for.

	// Faig dos bucles for per printar tota la informació del grau.
	for(j=0;j<d->num_degrees;j++){
		printf("\n%s\n", d->elements[j].name);

		for(k = 0;k<d->elements[j].num_classrooms;k++){
			printf("%s %d/inf\n", d->elements[j].classrooms[k].name,d->elements[j].classrooms[k].current_capacity);
		}
	}
}

/*********************************************** 
*
* @Finalitat: Comprovar si el grau introduit per l'usuari està a la memòria i axtualitzar la seva posició.

* @Paràmetres: in: d = Punter a degrees on es troba la direcció de tota la estructura creada previament.
			   in: degree = cadena on està el nom del grau.
			   in/out: degree_pos = Punter a enter on s'emmagatzema la direcció de la variable 
			       que determina la posició on es situa el grau introduit per l'usuari.

* @Retorn: correct = variable de tipus enter que valdrà 1 o 0 depenent si el grau introduit existeix a la memòria.
*
* **********************************************/
int findDegree(Degrees *d, char degree[], int *degree_pos){
	int i = 0;					// Variable per al bucle for.
	int correct = 0;			// Variable que valdrà 1 o 0 depenent si el grau introduit existeix a la memòria.
	
	// Faig un bucle for per comprovar que existeix el grau a la memòria i trobar la seva posició.
	for(i=0;i<d->num_degrees;i++){
		if(strcmp(degree, d->elements[i].name) == 0){
			correct = 1;
			*degree_pos = i;
		}
	}
	return(correct);
}
/*********************************************** 
*
* @Finalitat: Preguntar al usuari quin grau vol veure la seva informació 
			  i seguidament mostrar-la en cas que aquest existeixi.

* @Paràmetres: in: d = Punter a degrees on es troba la direcció de tota la estructura creada previament.
* @Retorn: ----
*
* **********************************************/
void showOption(Degrees *d){
	char degree[MAX_STRING_LENGTH];				// Cadena on es guardarà el nom del grau.
	int degree_pos = 0;							// Variable on es guardarà la posició del grau.
	int i = 0, j = 0;							// Variables per als bucles for.
    Student aux_student;						// Variable auxiliar per a llegir els estudiants de la llista

	// Obtinc el nom del grau sense \n.
	printf("\nDegree to show? ");
	fgets(degree, MAX_STRING_LENGTH-1, stdin);
	degree[strlen(degree)-1] = '\0';
	
	//Comprovo si existeix el grau amb la funció findDegree.
	if(findDegree(d, degree, &degree_pos)){
		printf("\n");

		for(i=0;i<d->elements[degree_pos].num_classrooms;i++){
			// Vaig al principi de la llista amb la funció LINKEDLIST_goToHead.
            LINKEDLIST_goToHead(d->elements[degree_pos].classrooms[i].students);

            for(j=0;j<d->elements[degree_pos].classrooms[i].current_capacity;j++){
			// Extrec el estudiant i l'asigno a la variable auxiliar.
            aux_student = LINKEDLIST_get(d->elements[degree_pos].classrooms[i].students);
            
            printf("%s (%s): %s\n", aux_student.name, aux_student.login, d->elements[degree_pos].classrooms[i].name);
            
			// Avanço el POV de la llista per poder seguir llegint els estudiants d'aquesta.
            LINKEDLIST_next(d->elements[degree_pos].classrooms[i].students);
            }
			
		}
	}
	else{
		printf("\nERROR: Can't find degree\n");
	}
}
/*********************************************** 
*
* @Finalitat: Comprova que hi ha algu estudiant amb el login introduit i actualitza
			  les variables de la seva posició a la classe i la possició de la classe.

* @Paràmetres: in: login = cadena on està el login a cercar.
			   in: d = Punter a Degrees on es troba la direcció de tota la estructura creada previament.
			   in/out: classroom_pos = Punter a enter on s'emmagatzema la direcció de la variable 
			       que determina la posició on es situa la classe del estudiant.
			   in/out: student_pos = Punter a enter on s'emmagatzema la direcció de la variable 
			       que determina la posició on es situa l'estudiant dintre la classe.	
			   in: degree_pos = posició de l'array dinàmica on està el grau on es vol cercar el login.

* @Retorn: Retorna una variable de tipus int que val 1 o 0 depenent si s'ha trobat un estudiant amb el login introduït o no.
*
* **********************************************/
int findLogin(char login[], Degrees *d, int *classroom_pos, int *student_pos, int degree_pos){
	int i = 0, j = 0;				// Variables per als bucles for.
	int correct = 0;				// Variable que valdrà 1 o 0 depenent si el login és correcte.
    Student aux_student;

	// Faig dos bucles for per comrovar si existeix el login.  
	for(i = 0; i < d->elements[degree_pos].num_classrooms; i++){
		// Vaig al principi de la llista amb la funció LINKEDLIST_goToHead.
        LINKEDLIST_goToHead(d->elements[degree_pos].classrooms[i].students);
		for(j = 0; j < d->elements[degree_pos].classrooms[i].current_capacity; j++){
			// Extrec el estudiant i l'asigno a la variable auxiliar.
            aux_student = LINKEDLIST_get(d->elements[degree_pos].classrooms[i].students);
			if(strcmp(login, aux_student.login) == 0){
				// Actulitzo la posició de la seva classe desreferenciant el punter.
				*classroom_pos = i;
				// Actualitzo la posició dintre de la classe desreferenciant el punter.
				*student_pos = j;

				correct = 1;
			}
			// Avanço el POV de la llista per poder seguir llegint els estudiants d'aquesta.
            LINKEDLIST_next(d->elements[degree_pos].classrooms[i].students);
		}
	}
	return(correct);
}
/*********************************************** 
*
* @Finalitat: Preguntar al usuari un grau, login del estudiant i index
			  de la classe a la que és vol moure, i sempre que la informació 
			  sigui correcte és mou a l'estudiant.
* @Paràmetres: in: d = Punter a Degrees on es troba la direcció de tota la estructura creada previament.
* @Retorn: ----
*
* **********************************************/
void moveOption(Degrees *d){
	char degree[MAX_STRING_LENGTH];						// Cadena on es guardarà el nom del grau.
	int degree_pos = 0;									// Variable on s'emmagatzemarà la posició del grau a l'array dinàmica
	int i = 0, j = 0;									// Variables for per als bucles while.
	char login[MAX_STRING_LENGTH];						// Cadena on es guardarà el login el estudiant.
	int index = 0;										// Variable on es guardarà el index que introdueix l'usuari
	int classroom_pos = 0, student_pos_copy = 0;		// Variables on s'emmagatzemarà la posició destí i origen de la classe respectivament.
	int error = 0;										// Variable flag per si alguna de les condicions no es compleix.
	int student_position = 0;							// Posició on s'haura de traspassar el usuari.
    Student student_copy;

	// Llegeixo el nom del grau que introdueix l'usuari sense \n.
	printf("\nDegree? ");
	fgets(degree, MAX_STRING_LENGTH, stdin);
	degree[strlen(degree)-1] = '\0';

	// Comprovo que el grau existeix amb la funció findDegree.
	if(findDegree(d, degree, &degree_pos)){
		printf("\n");
		// Mostro quina és la distribució actual del grau.
		printf("Classrooms and capacity:\n");
		for(i=0;i<d->elements[degree_pos].num_classrooms;i++){
			printf("%d. %s %d/inf\n",i+1, d->elements[degree_pos].classrooms[i].name, d->elements[degree_pos].classrooms[i].current_capacity);
			
			// Vaig al principi de la llista amb la funció LINKEDLIST_goToHead.
            LINKEDLIST_goToHead(d->elements[degree_pos].classrooms[i].students);

			for(j=0;j<d->elements[degree_pos].classrooms[i].current_capacity;j++){

				// Extrec el estudiant i el printo
				printf("%s\n", (LINKEDLIST_get(d->elements[degree_pos].classrooms[i].students)).login);

				// Avanço el POV de la llista per poder seguir llegint els estudiants d'aquesta.
                LINKEDLIST_next(d->elements[degree_pos].classrooms[i].students);
			}
		}
		
		//Llegeixo el nom del grau que introdueix l'usuari sense \n.
		printf("\nWho do you want to move (login)? ");
		scanf("%s", login);
		
		//Comprovo que existeix un estudiant amb el login introduit.
		if(findLogin(login, d, &classroom_pos, &student_pos_copy, degree_pos)){

			// Llegeixo l'index de la classe a la que s'ha de moure l'usuari.
			printf("\nTo which classroom (index)? ");
			scanf("%d", &index);

			// Comprovo que aquest compleix les condicions.
			if(index > 0 && index<=d->elements[degree_pos].num_classrooms && index-1 != classroom_pos){
				
				// Vaig al principi de la llista amb la funció LINKEDLIST_goToHead.
                LINKEDLIST_goToHead(d->elements[degree_pos].classrooms[classroom_pos].students);
                for(i=0;i<student_pos_copy;i++){

					// Avanço el POV de la llista per trobar la posicio del estudiant a copiar.
                    LINKEDLIST_next(d->elements[degree_pos].classrooms[classroom_pos].students);
                }
				// Extrec el estudiant i l'asigno a la variable auxiliar.
                student_copy = LINKEDLIST_get(d->elements[degree_pos].classrooms[classroom_pos].students);
                
                
                // Afegeixo l'estudiant a la nova posició de la llista
				LINKEDLIST_add(d->elements[degree_pos].classrooms[index-1].students, student_copy);

				// Actualitzo la capacitat.
				d->elements[degree_pos].classrooms[index-1].current_capacity++;
				
				// Resetejo i i la j.
				i = 0;
				j = 0;

				// Faig un bucle while per eliminar el estudiant que ha estat mogut.
				while(i<d->elements[degree_pos].classrooms[classroom_pos].current_capacity){

					// Comprovo si la posició original del estudiant es diferent de i per actualitzar la llista d'estudiants de la classe.
					if(i == student_pos_copy){
						// Elimino el estudiant amb la funció LINKEDLIST_remove.
                        LINKEDLIST_remove(d->elements[degree_pos].classrooms[classroom_pos].students);
					}
					// Avanço el POV de la llista per trobar la posicio del estudiant a eliminar.
                    LINKEDLIST_next(d->elements[degree_pos].classrooms[classroom_pos].students);
					i++;
				}
				// Resto 1 a la capacitat de la classe original del estudiant.
				d->elements[degree_pos].classrooms[classroom_pos].current_capacity--;
			}
			else{
				error = 1;
			}
		}
		else{
			error = 1;
		}
	}
	else{
		error = 1;
	}
	// En cas de que les dades introduides no siguin correctes es mostra l'error.
	if (error){
		printf("\nERROR: Can't move student\n");

	}
}
/*********************************************** 
*
* @Finalitat: Neteja la memòria on estava guardada la informació dels fitxers.
* @Paràmetres: in/out: d = Punter a Punter a Degree que permet modificar el contingut de "d" fora del main.
* @Retorn: ----
*
* **********************************************/
void dealocation(Degrees **d){
	int i = 0, j = 0;					// Variables per als bucles for.

	// Faig dos bucles for per alliberar la memòria on estaven emmagatzemats els studiants.
	for(i=0;i<(*d)->num_degrees;i++){
		for(j=0;j<(*d)->elements[i].num_classrooms;j++){
			//Allibero la memoria de la llista destruintla amb la funció LINKEDLIST_destroy.
			LINKEDLIST_destroy(&((*d)->elements[i].classrooms[j].students));
		}
	}
	// Faig un bucle for per alliberar la memòria on estaven emmagatzemades les classes.
	for(i=0;i<(*d)->num_degrees;i++){
		free((*d)->elements[i].classrooms);
	}
	// Allibero la memòria on estava emmagatzemada la informació dels graus.
	free((*d)->elements);
	// Allibero la memòria on estaven emmagatzemats els graus.
	free((*d));
}
/*********************************************** 
*
* @Finalitat: Executar el sistema (Funció Principal).
* @Paràmetres: ----
* @Retorn: ----
*
* **********************************************/
int main(){
	int correct_class = 0, correct_student = 0;								// Variables flag que determinaràn si els fitxers son correctes.
	char class_name[MAX_STRING_LENGTH], students_name[MAX_STRING_LENGTH];	// Cadenes on es guardarà el nom dels fitxers.
	FILE *f1, *f2;															// Punters on es guardaran les direccions dels fitxers.
	int continua = 1;														// Variable flag per al bucle while.
	int op = 0;																// Variable que determinarà quina opció realitzar
	Degrees *d;																// Punter a Degree on guardarà la direcció de tota l'structura d'arrays dinàmiques.
	char trash;																// Variable per netejar el buffer.

	// Reservo memòria per a d;
	d = (Degrees *) malloc(sizeof(Degrees));
	
	printf("Welcome!\n");

	// Faig un bucle while per llegir el primer fitxer
	while(!correct_class){
		// Llegeixo el nom del fitxer que introdueix l'usuari.
		printf("\nType the name of the 'classrooms' file: ");
		scanf("%s", class_name);

		// Obro el fitxer.
		f1 = fopen(class_name, "r");

		//Crido la funció correctFile per comprovar si el fitxer s'ha obert correctament.		
		correctFile(&correct_class, f1);
		
		// Comprovo que el fitxer s'ha obert correctament.
		if(correct_class){

			// Crido la funció readFileOne per llegir el fitxer e inicialitzar la memòria.
			readFileOne(f1, &d);
			// Tanco el fitxer
			fclose(f1);

			//Faig un bucle while per llegir el segon fitxer
			while(!correct_student){
				// Llegeixo el nom del fitxer que introdueix l'usuari.
				printf("\nType the name of the 'students' file: ");
				scanf("%s", students_name);

				// Obro el fitxer.
				f2 = fopen(students_name, "r");		

				//Crido la funció correctFile per comprovar si el fitxer s'ha obert correctament.
				correctFile(&correct_student, f2);

				// Comprovo que el fitxer s'ha obert correctament.
				if(!correct_student){
					printf("\nERROR: Can't open file '%s'\n", students_name);
				}
				else{
					// Crido la funció readFileTwo per llegir el fitxer.
					readFileTwo(f2, &d);
					// Tanco el fitxer
					fclose(f2);
				}
			}	
		}
		else{
			// En cas de que no es pugui obrir el fitxer es printa el missatge d'error.
			printf("\nERROR: Can't open file '%s'\n",class_name);
		}	
	}
	
	// Faig un bucle while per a demanar la opció al usuari.
	while(continua){

		// Demano la opció al usuari.
		printf("\n1. Summary | 2. Show degree students | 3. Move student | 4. Exit\nSelect option: ");
		scanf("%d", &op);
		// Netejo el buffer per evitar errors.
		scanf("%c", &trash);
		
		//Comprovo que la opció és correcta.
		if(op>0 && op<5){
			// Faig un switch amb op per realitzar la opció que introdueix l'usuari.
			switch(op){
				case 1:
					// Crido la funció summaryOption per executar la opció 1.
					summaryOption(d);
				break;

				case 2:
					// Crido la funció showOption per executar la opció 2.
					showOption(d);
				break;
			
				case 3:
					// Crido la funció moveOption per executar la opció 3.
					moveOption(d);
				break;

				case 4:
					printf("\nBye!\n");
					// Finalitzo el bucle.
					continua = 0;
			}
		}
		else{
			// En el cas de que la opció no sigui correcta és printa el missatge d'error.
			printf("\nERROR: Wrong option number\n");
		}

	}

	// Allibero tota la memòria reservada anteriorment amb la funció dealocation.
	dealocation(&d);

	return(0);
}
œ