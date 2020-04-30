#include <stdio.h>
#include <string.h>
#include "libbt/btree.h"
#include "libfdr/jrb.h"
#include "libfdr/fields.h"

#define MAX_WORD 40
#define MAX_DATA 100000

void addString(char *des,char *source){
    int i,j = 0;
    for(i = strlen(des); i <= strlen(des) + strlen(source); i++){
        des[i] = source[j];
        j++;
    }
}
/* divide English and Vietnamese */
void divideString(char *source,char *des){
    int i,j,k = 0,check = 0;
    for(i=0; i < strlen(source); i++){
        if(source[i] == '/'){
            des[0] = '\0';
            check = 1;
            break;
        }
    }

    if(check == 0){
        source[strlen(source) - 1] = '\0';
        return;
    }

    for(j=i;j <= strlen(source); j++){
        des[k] = source[j];
        k++;
    }

    source[i-1] = '\0';
}

void fixString(char *str){
    int i;
    for(i = 1;i <= strlen(str); i++){
        str[i -1] = str[i];
    }
}

void readData(BTA **dic,char *filename){
    FILE *f;
    int count = 0;
    char word[MAX_WORD],data[MAX_DATA],tmp[100];
    data[0] = '\0';         //  data is empty
    
    if(!(f = fopen(filename,"r"))){
        printf("Error file \n");
        return;
    }

    *dic = btcrt("dictionary",0,0);
    
    /* Read file anhviet.txt */
    while(!feof(f)){
        fgets(tmp,100,f);
        while(tmp[0] != '@'){
            addString(data,tmp);
            if(feof(f)) break;
            fgets(tmp,100,f);
        }
    
        /* Insert into B Tree named dic*/
        int k = btins(*dic,word,data,500*sizeof(char));
        if(k == 0)  count++;
        if(tmp[0] == '@'){
            strcpy(word,tmp);
            data[0] = '\0';
            divideString(word,data);
            fixString(word);
        }
    }
    fclose(f);
}

void menu(){
    printf("\n===========MeNu=============\n");
    printf("1.Print\n2.Add\n3.Search\n4.Delete\n5.Exit");
    printf(".....Your choice: ");
}
int main()
{
    char en[100];
    char vn[100000];
    char tmp[100];
    int size;
    int scan,choice;
   /* init a new dictionay in BTree */
   BTA * dic;
   btinit();
   readData(&dic,"demo.txt");
   do{    
        menu();
        scanf("%d",&scan);
        switch (scan)
        {
        case 1:
            btsel(dic,"",vn,sizeof(char),&size);
            while(btseln(dic,en,vn,100000*sizeof(char),&size) == 0){
                printf("%s\n%s\n",en,vn);
            }
            break;
        case 2:
            printf("English: ");
            __fpurge(stdin);
            gets(en);
            printf("Vietnamese: ");
            __fpurge(stdin);    
            gets(vn);
            
            if(btsel(dic,en,vn,sizeof(char),&size) == 0)
            {
                printf("\nMESSAGE:want to update press 1 if not press 0");
                scanf("%d",&choice);
                if(choice == 1)  btupd(dic,en,vn,sizeof(char));
                else break;
            }
            else 
            {
                btins(dic,en,vn,sizeof(char));
            }
            
            break;         
        case 3:
            printf("Word: ");
            __fpurge(stdin);
            gets(tmp);
            if(btsel(dic,en,vn,sizeof(char),&size)) printf("Not found\n");
            else printf("%s\n%s\n",en,vn);
            break;
        case 4:
            printf("Word: ");
            __fpurge(stdin);
            gets(tmp);
            if(btsel(dic,tmp,vn,sizeof(char),&size)) printf("Not found\n");
            else btdel(dic,tmp);
            break;
        case 5:
            printf("\nBye Bye");
            btcls(dic);
            break;
        default:
            break;
        }
   }while(scan != 5);

    

    
}