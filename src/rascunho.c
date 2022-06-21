#include "../libs/include.h"



/*
temos uma grid de celulas, cada uma possuindo uma lista de 
modulos validos para essa posicao.
Cada modulo tem sua cor e lista de modulos adjacentes validos
*/



//cor de um modulo




int size;
struct Queue* q;


int choose_module_rand(int ceiling){
    return (int)(((float)rand()/(float)RAND_MAX)*ceiling); //escolhe de 0..ceiling
}


void collapse_cell(Cell_2d* cell, int guess){
    
    int ceiling = cell->entropy-1;

    for(int i=0; i<ceiling; i++){
        if(i!=guess){
            cell->modules[i] = NULL;
        }
    }

    cell->modules[0]=cell->modules[guess];
    cell->modules[guess]=NULL;
    cell->entropy=1;
}



/*
void propagate(Cell_2d** grid, Pos_2d root_pos){
    Pos_2d target_pos;

    if(!(root_pos.lin < 0) && !(root_pos.lin > size-1) && !(root_pos.col < 0) && !(root_pos.col > size-1)){

    }
}
*/

void checkValidity(char dir){
    switch (dir) {
    case 't' :{
        
        break;
    }
    case 'b' :{

        break;
    }
    case 'r' :{

        break;
    }
    case 'l' :{

        break;
    }
    }
}

void constrain(Cell_2d* root, Cell_2d* target, char dir){
    int start_target_entropy = target->entropy;
    int root_entropy = root->entropy;


    //loopa pelos modulos de target, checando se sao validos
    for(int i =0; i < start_target_entropy; i++){
        for(int j =0; j < root_entropy; j++){
            switch (dir) {
                case 't' :{
                    size= root->modules[j]->sizeNeigTop;

                    for(int k=0;k<size;k++){
                        if(target->modules[i] == root->modules[j]->neigTop[k]){
                            goto continue_i;
                        }
                    }
                    break;
                }
                case 'b' :{
                    size= root->modules[j]->sizeNeigBotton;

                    for(int k=0;k<size;k++){
                        if(target->modules[i] == root->modules[j]->neigBotton[k]){
                            goto continue_i;
                        }
                    }
                    break;
                }
                case 'r' :{
                    size= root->modules[j]->sizeNeigRight;

                    for(int k=0;k<size;k++){
                        if(target->modules[i] == root->modules[j]->neigRight[k]){
                            goto continue_i;
                        }
                    }
                    break;
                }
                case 'l' :{
                    size= root->modules[j]->sizeNeigLeft;

                    for(int k=0;k<size;k++){
                        if(target->modules[i] == root->modules[j]->neigLeft[k]){
                            goto continue_i;
                        }
                    }
                    break;
                }
            }
        }


        //remover modulo
        target->modules[i] = NULL;
        target->entropy--;

        continue_i:;
    }



    

    //checa se mudou alguma coisa, ordena, e coloca na queue

    int end_target_entropy = target->entropy;

    if(end_target_entropy<start_target_entropy){

        enQueue(q, target->pos);

        for(int i = 0; i<start_target_entropy;i++){
            if(target->modules[i] != NULL){
                for(int j =0; j<i;j++){
                    if(target->modules[j]==NULL){
                        target->modules[j] = target->modules[i];
                        target->modules[i] = NULL;
                    }
                }
            }
        }
    }
}


void propagate(Cell_2d** grid, Pos_2d root_pos){
    Pos_2d target_pos;

    enQueue(q, root_pos);

    while(q->front != NULL){
        target_pos = q->front->key;
        deQueue(q);


        //ajeitar nao eh pra olhar 100% tudo em volta
        if(target_pos.lin<size-1 && grid[target_pos.lin+1][target_pos.col].entropy>1){
            constrain(&grid[target_pos.lin][target_pos.col], &grid[target_pos.lin+1][target_pos.col], 't');
        }
        if(target_pos.lin>0 && grid[target_pos.lin-1][target_pos.col].entropy>1){
            constrain(&grid[target_pos.lin][target_pos.col], &grid[target_pos.lin-1][target_pos.col], 'b');
        }
        if(target_pos.col>0 && grid[target_pos.lin][target_pos.col-1].entropy>1){
            constrain(&grid[target_pos.lin][target_pos.col], &grid[target_pos.lin][target_pos.col-1], 'l');
        }
        if(target_pos.col<size-1 && grid[target_pos.lin][target_pos.col+1].entropy>1){
            constrain(&grid[target_pos.lin][target_pos.col], &grid[target_pos.lin][target_pos.col+1], 'r');
        }

    }
}




void collapse(Cell_2d** grid, Pos_2d pos){

    Cell_2d cell = grid[pos.lin][pos.col];
    


    //nao pode chamar collapse em celulas com entropia menor que 2
    if(cell.entropy <= 1) {  printf("Error: Tried to collapse cell with entropy of 1 or less"); exit(1);   }

    

    int ceiling = cell.entropy-1;
    int guess = choose_module_rand(ceiling);

    //torna nulos todas as referencias a modulos que não foram a escolhida e traz a nao nula para frente
    collapse_cell(&cell, guess);

    /*ATENCAO    -    ajeitar opcoes das celulas adjacentes*/
    propagate(grid, cell.pos);
}








int main(void){
    Cell_2d** grid;

    q = createQueue();
    //enQueue(q, 10);
    //deQueue(q);
    //queue-> front pra acessar
}