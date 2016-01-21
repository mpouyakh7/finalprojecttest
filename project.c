#include<stdio.h>
#include<ctype.h>
#include<string.h>
void keyw(char *p, char *tp, char *att);
int getToken(char *tp, char *att,int *Line);
int i=0,id=0,kw=0,num=0,op=0;
char keys[12][10]={"char","float","bool",
"int","void", "if","else","while","NULL","main","return"};
char multioper[5][3]={"==","&&","||","!="};
char buff[3]="\0";

FILE *f1;
typedef struct token
{
    char typeT[20];
    char attT[20];
    int lineT;
    struct token *next;
}token;

typedef struct symbol
{
    char typeS[20];
    char attrS[20];
    float value;
    int index;
    struct symbol *next;
}symbolT;

struct symbol *firstS,*mS;
int Sindex = 0;
char idType[20]="\0";
int *line;
token *tmpT;

void mainf();
void iff();
void elsef();
void whilef();
void intf();
void boolf();
void floatf();
void charf();
void idf();
void idfIR();


void main() {
    FILE *cprogram;
    FILE *hfile;
    FILE *prepc;
    char c=1;
    char cfile_name[20];
    char hfile_name[20];
    char prepfile_name[20];
    int  i=0;
    int  k=0;

    char type[20], attr[20];

    token *mT,*firstT;

    firstS = NULL;
    firstT = NULL;
    line= (int *) malloc(sizeof(int));
    *line=1;

    printf("\n\nStarting The Preprocessor \n");

    printf("\nPlease enter the C program file name (full path): ");
    scanf("%s",cfile_name);

    if((cprogram=fopen(cfile_name,"r"))==NULL) return 1;

    while ( (c = fgetc(cprogram)) != EOF )
        {
            if(c =='"')
            {
               break;
            }
        }

        fscanf(cprogram,"%s",hfile_name);

        for(i=0;hfile_name[i]!='\0';i++)
        {
            if(hfile_name[i]=='"')
            {
               hfile_name[i]='\0';
               break;
            }
        }


    for(i=0; cfile_name[i]!='.';i++)
        prepfile_name[i]=cfile_name[i];

    prepfile_name[i]='.'; prepfile_name[i+1]='d'; prepfile_name[i+2]='a'; prepfile_name[i+3]='t';
    prepfile_name[i+4]='\0';

    printf("\nhfilename=%s , prepfilename=%s \n", hfile_name, prepfile_name);

    prepc=fopen(prepfile_name,"w");
     if( prepc == NULL)
        printf("\nWriting to file. The file, pouya.dat, can not be opened");
    else{

       printf("\nThe program after preprocessing: \n");

       if((hfile=fopen(hfile_name,"r"))!= NULL)
       while( !feof(hfile) )
       {
           c=fgetc(hfile);
           printf("%c",c);
           if (c!=-1)
           fputc(c,prepc);
       }

       while( !feof(cprogram) )
       {
           c=fgetc(cprogram);
           printf("%c",c);
           if (c!=-1)
           fputc(c,prepc);
       }

    }
    fclose(cprogram);
    fclose(hfile);
    fclose(prepc);

    printf("\n\nStarting Lexical Analysis: Tokens are written into a linked list and the symbol table is simultaneously created. \n");

    f1=fopen(prepfile_name,"r");
    if (f1==NULL) return 1;

    while ( !getToken(type, attr, line)){
      // printf("\n %s is a %s",attr, type);
       if(firstT==NULL)
       {
            tmpT= (token *) malloc( sizeof(token));
            firstT=tmpT;
            firstT->next=NULL;
            strcpy(firstT->attT,attr);
            strcpy(firstT->typeT,type);
            firstT->lineT= *line;
            printf("-> %s is a %s at line  %d\n",firstT->attT,firstT->typeT,firstT->lineT);
       }
       else
       {
            tmpT= (token *) malloc( sizeof(token));
            mT=firstT;
            while(mT->next!=NULL) mT=mT->next;

            mT->next=tmpT;
            tmpT->next=NULL;
            strcpy(tmpT->attT,attr);
            strcpy(tmpT->typeT,type);
            tmpT->lineT= *line;
            printf("-> %s is a %s at line %d\n",tmpT->attT,tmpT->typeT,tmpT->lineT);
       }

    }

    printf("\n\nStarting Syntax Analysis\n");
    tmpT=firstT;

    while( tmpT )
    {
       if (strcmp("main",tmpT->attT)==0 ) ;
       else if (strcmp("if",tmpT->attT)==0 )   ;
       else if (strcmp("else",tmpT->attT)==0 )  ;
       else if (strcmp("while",tmpT->attT)==0 ) ;
       else if (strcmp("int",tmpT->attT)==0 )   ;
       else if (strcmp("bool",tmpT->attT)==0 )  ;
       else if (strcmp("float",tmpT->attT)==0 ) ;
       else if (strcmp("char",tmpT->attT)==0 )  ;
       else if (strcmp("identifier",tmpT->typeT)==0 ) idf();
       tmpT=tmpT->next;
    }
    printf(" \n symbol table  :   \n");

    symbolT *test;
    test=firstS;
    while(test!=NULL)
    {
           printf(" %d      %s     %s     %f\n ", test->index, test->attrS, test->typeS,test->value);
           test=test->next;

    }

    printf("\n\nStarting Intermediate Representation (IR) Generation\n");
    tmpT=firstT;

    while( tmpT )
    {
 /*      if (strcmp("main",tmpT->attT)==0 ) mainf();
       else if (strcmp("if",tmpT->attT)==0 )   iff();
       else if (strcmp("else",tmpT->attT)==0 )  elsef();
       else if (strcmp("while",tmpT->attT)==0 ) whilef();
       else if (strcmp("int",tmpT->attT)==0 )   intf();
       else if (strcmp("bool",tmpT->attT)==0 )  boolf();
       else if (strcmp("float",tmpT->attT)==0 ) floatf();
       else if (strcmp("char",tmpT->attT)==0 )  charf();
       else*/ if (strcmp("identifier",tmpT->typeT)==0 ) idfIR();
       tmpT=tmpT->next;
    }


   // printf("\nKeywords: %d\nIdentifiers: %d\nOperators: %d\nNumbers: %d\n",kw,id,op,num);
}

void idf(){
    token *id1ptr, *id2ptr, *id3ptr, *opptr;

    id1ptr = tmpT;

    if ( (tmpT=tmpT->next)==NULL) {printf("\nidf208: NULL pointer encountered! Exiting."); return;}

    if ( tmpT->attT[0] == ';') return;
    else if (tmpT->attT[0] != '=') {printf("\nidf211: At line %d : unexpected operator %s, '=' was expected!\n",tmpT->lineT,tmpT->attT); return;}

    if ( (tmpT=tmpT->next)==NULL) {printf("\nidf213: NULL pointer encountered! Exiting."); return;}

    if ( strcmp(tmpT->typeT,"identifier")==0 || strcmp(tmpT->typeT,"number")==0 ) id2ptr= tmpT;
    else
        {printf("\nidf217: At line %d : unexpected operand %s, a number or identifier was expected!\n",tmpT->lineT,tmpT->attT); return;}

    if ( (tmpT=tmpT->next)==NULL) {printf("\nidf219: NULL pointer encountered! Exiting."); return;}

    int flag1=0;
    if ( tmpT->attT[0] == ';')
    {
        flag1=1;  //there is only one operand after assignment (=)
    }
    else
    {
      if ( strcmp(tmpT->typeT,"operator")!=0 )
        {printf("\nidf229: At line %d : unexpected token %s, an operator was expected!\n",tmpT->lineT,tmpT->attT); return;}

      opptr = tmpT;

      if ( (tmpT=tmpT->next)==NULL) {printf("idf233: NULL pointer encountered! Exiting."); return;}

      if ( strcmp(tmpT->typeT,"identifier")==0 || strcmp(tmpT->typeT,"number")==0 ) id3ptr= tmpT;
      else
        {printf("\nidf237: At line %d : unexpected operand %s, a number or identifier was expected!\n",tmpT->lineT,tmpT->attT); return;}


      if ( (tmpT=tmpT->next)==NULL) {printf("idf240: NULL pointer encountered! Exiting."); return;}

      if ( tmpT->attT[0] != ';')
        {printf("\nidf243: At line %d : unexpected operand %s, a ';' was expected!\n",tmpT->lineT,tmpT->attT); return;}
    }

    symbolT *id1Sptr, *id2Sptr, *id3Sptr;    //these are id pointers in the symbol table
    id1Sptr=id2Sptr=id3Sptr= firstS;
    int flag2=0;
    float value1, value2;
    do
    {
        if( strcmp(id1Sptr->attrS, id1ptr->attT) == 0 ) {flag2 =1; break;}
    }
    while( (id1Sptr=id1Sptr->next) != NULL );

    if (flag2 == 0) { printf("\nidf256: ERROR- identifier %s not in the symbol table", id1ptr->attT); return;}

    if ( strcmp(id2ptr->typeT,"identifier")==0 )
    {
        flag2=0;
        do
        {
           printf("\nidf: id2Sptr->attrs= %s , id2ptr->attT=%s",id2Sptr->attrS, id2ptr->attT );
           if( strcmp(id2Sptr->attrS, id2ptr->attT) == 0 ) {flag2 =1; break;}
        }
        while( (id2Sptr=id2Sptr->next) != NULL );

        if (flag2 == 0) { printf("\nidf267: ERROR- identifier %s not in the symbol table", id1ptr->attT); return;}
        value1= id2Sptr->value;
    }
    else
    {
        value1= atoi(id2ptr->attT);
                printf("\nidf273: %s , %f",id2ptr->attT,value1 );

    }

    if( flag1==1 )    //when there is only one opernad
    {
        id1Sptr->value = value1;
        printf("\nSyntax Processing: value %f is assigned to identifier %s", value1, id1ptr->attT);
    }
    else              // when ther are two opernads and an opoerator
    {

      if ( strcmp(id3ptr->typeT,"identifier")==0 )
      {
        flag2=0;
        do
        {
           if( strcmp(id3Sptr->attrS, id3ptr->attT) == 0 ) {flag2 =1; break;}
        }
        while( (id3Sptr=id3Sptr->next) != NULL );

        if (flag2 == 0) { printf("\nidf292: ERROR- identifier %s not in the symbol table", id1ptr->attT); return;}
        value1= id3Sptr->value;
      }
      else
      {
        value2= atoi(id3ptr->attT);
      }

      if ( opptr->attT[0] != '+' )
      {
        id1Sptr->value = value1 + value2;
        printf("\nSyntax Processing: %f %s %f is assigned to identifier %s", value1, opptr->attT, value2, id1ptr->attT);
      }
      else if ( opptr->attT[0] != '-' )
      {
        id1Sptr->value = value1 - value2;
        printf("\nSyntax Processing: %f %s %f is assigned to identifier %s", value1, opptr->attT, value2, id1ptr->attT);
      }
      else if ( opptr->attT[0] != '*' )
      {
        id1Sptr->value = value1*value2;
        printf("\nSyntax Processing: %f %s %f is assigned to identifier %s", value1, opptr->attT, value2, id1ptr->attT);
      }
      else if ( opptr->attT[0] != '/' )
      {
         if( value2 != 0 )
         {
            id1Sptr->value = (value1 / value2);
            printf("\nSyntax Processing: %f %s %f is assigned to identifier %s", value1, opptr->attT, value2, id1ptr->attT);
         }
         else printf("\nidf: error :  division by zero ");
      }
      else
        {printf("\nidf325: At line %d : unexpected operator %s, a + - * / was expected!\n",opptr->lineT,opptr->attT); return;}
    }
}

void idfIR(){
    token *id1ptr, *id2ptr, *id3ptr, *opptr;

    id1ptr = tmpT;

    if ( (tmpT=tmpT->next)==NULL) {printf("idf: NULL pointer encountered! Exiting."); return;}

    if ( tmpT->attT[0] == ';') return;
    else if (tmpT->attT[0] != '=') {printf("At line %d : unexpected operator %s, '=' was expected!\n",tmpT->lineT,tmpT->attT); return;}

    if ( (tmpT=tmpT->next)==NULL) {printf("idf: NULL pointer encountered! Exiting."); return;}

    if ( strcmp(tmpT->typeT,"identifier")==0 || strcmp(tmpT->typeT,"number")==0 ) id2ptr= tmpT;
    else
        {printf("At line %d : unexpected operand %s, a number or identifier was expected!\n",tmpT->lineT,tmpT->attT); return;}

    if ( (tmpT=tmpT->next)==NULL) {printf("idf: NULL pointer encountered! Exiting."); return;}

    int flag1=0;
    if ( tmpT->attT[0] == ';')
    {
        flag1=1;  //there is only one operand after assignment (=)
    }
    else
    {
      if ( strcmp(tmpT->typeT,"operator")!=0 )
        {printf("At line %d : unexpected token %s, an operator was expected!\n",tmpT->lineT,tmpT->attT); return;}

      opptr = tmpT;

      if ( (tmpT=tmpT->next)==NULL) {printf("idf: NULL pointer encountered! Exiting."); return;}

      if ( strcmp(tmpT->typeT,"identifier")==0 || strcmp(tmpT->typeT,"number")==0 ) id3ptr= tmpT;
      else
        {printf("At line %d : unexpected operand %s, a number or identifier was expected!\n",tmpT->lineT,tmpT->attT); return;}


      if ( (tmpT=tmpT->next)==NULL) {printf("idf: NULL pointer encountered! Exiting."); return;}

      if ( tmpT->attT[0] != ';')
        {printf("At line %d : unexpected operand %s, a ';' was expected!\n",tmpT->lineT,tmpT->attT); return;}
    }

    symbolT *id1Sptr, *id2Sptr, *id3Sptr;    //these are id pointers in the symbol table
    id1Sptr=id2Sptr=id3Sptr=firstS;
    int flag2=0;
    float value1, value2;
    do
    {
        if( strcmp(id1Sptr->attrS, id1ptr->attT) == 0 ) {flag2 =1; break;}
    }
    while( (id1Sptr=id1Sptr->next) != NULL );

    if (flag2 == 0) { printf("idf: ERROR- identifier %s not in the symbol table", id1ptr->attT); return;}

    if ( strcmp(id2ptr->typeT,"identifier")==0 )
    {
        flag2=0;
        do
        {
           if( strcmp(id2Sptr->attrS, id2ptr->attT) == 0 ) {flag2 =1; break;}
        }
        while( (id2Sptr=id2Sptr->next) != NULL );

        if (flag2 == 0) { printf("idf: ERROR- identifier %s not in the symbol table", id1ptr->attT); return;}
        value1= id2Sptr->value;
        printf("\nFetch(%s)",id2Sptr->attrS);
    }
    else
    {
        value1= atoi(id2ptr->attT);
        printf("\nLoad(%f)",value1);
    }

    if( flag1==1 )    //when there is only one opernad
    {
        id1Sptr->value = value1;
        printf("\n%s = %f", id1ptr->attT,value1);
    }
    else              // when ther are two opernads and an opoerator
    {

      if ( strcmp(id3ptr->typeT,"identifier")==0 )
      {
        flag2=0;
        do
        {
           if( strcmp(id3Sptr->attrS, id3ptr->attT) == 0 ) {flag2 =1; break;}
        }
        while( (id3Sptr=id3Sptr->next) != NULL );

        if (flag2 == 0) { printf("idf: ERROR- identifier %s not in the symbol table", id1ptr->attT); return;}
        value2= id3Sptr->value;
        printf("\nFetch(%s)",id3Sptr->attrS);
      }
      else
      {
        value2= atoi(id3ptr->attT);
        printf("\nLoad(%f)",value2);
      }

      if ( opptr->attT[0] != '+' )
      {
        id1Sptr->value = value1 + value2;
        printf("\nR1=%f %s %f", value1, opptr->attT, value2);
        printf("\n%s = R1", id1ptr->attT);
      }
      else if ( opptr->attT[0] != '-' )
      {
        id1Sptr->value = value1 - value2;
        printf("\nR1=%f %s %f", value1, opptr->attT, value2);
        printf("\n%s = R1", id1ptr->attT);
      }
      else if ( opptr->attT[0] != '*' )
      {
        id1Sptr->value = value1*value2;
        printf("\nR1=%f %s %f", value1, opptr->attT, value2);
        printf("\n%s = R1", id1ptr->attT);
      }
      else if ( opptr->attT[0] != '/' )
      {
         if( value2 !=0 )
         {
            id1Sptr->value = value1 / value2;
            printf("\nR1=%f %s %f", value1, opptr->attT, value2);
            printf("\n%s = R1", id1ptr->attT);
         }
         else printf(" error :  division by zero ");
      }
      else
        {printf("At line %d : unexpected operator %s, a + - * / was expected!\n",opptr->lineT,opptr->attT); return;}
    }
}

int getToken (char tp[], char att[],int *Line)
{
    char ch,ch_str[25],str[25],seps[15]=" \t\n,'\"<>",oper[]="*-;+={}()|<>/";
    int j, ll;

    	if(f1==NULL)
        {
        	 printf("file not found");
        	 exit(0);
        }

        while( 1 )
        {
            if( buff[0] != '\0')
            {
                ch=buff[0];
                buff[0]='\0';
            }
            else if ( (ch=fgetc(f1)) == EOF)
                return 1;

            for(j=0;j<=13;j++)
            {
                if(ch==oper[j])
                    if( i>0 )
                {
                        buff[0]=ch;
                        str[i]='\0';
                        keyw(str, tp ,att);
                }
                else
                {
                    ch_str[0]=ch;
                    if ( (ch= fgetc(f1))==ch_str[0] )
                        {ch_str[1]=ch; ch_str[2]='\0';}
                    else if ( ch_str[0]=='!' && ch=='=')
                        {ch_str[1]=ch; ch_str[2]='\0';}
                    else
                        {ch_str[1]='\0'; buff[0]=ch;}

                    if( ch_str[0]=='{' || ch_str[0]=='}' || ch_str[0]==';'|| ch_str[0]=='('|| ch_str[0]==')') strcpy(tp,"punctuator");
                    else strcpy(tp,"operator");
                    strcpy(att,ch_str);
                    op++;
                    i=-1;
                 }
            }

            for(j=0;j<=14;j++)
            {
                if(i==-1) break;

                if(ch==seps[j])
                {
                    if(ch=='#')
                    {
                        ll=0;
                        while(ch!='>')
                        {
                                ch=fgetc(f1);
                                ch_str[ll]=ch;
                                ll++;
                        }
                        printf("%s is a header file\n",ch_str);
                        strcpy(tp,"header");
                        strcpy(att,ch_str);
                        break;
                    }
                    if(ch=='\n')
                    {
                        (*Line)++;
                    }

                    if(ch=='\'')
                    {
                           ll=0;
                           do
                           {
                                ch=fgetc(f1);
                                ch_str[ll]=ch;
                                ll++;
                           }    while(ch!='\'');

                           ch_str[ll-1]='\0';
                           strcpy(tp,"charactor");
                           strcpy(att,ch_str);
                           i=-1;
                           break;
                    }

                    str[i]='\0';
                    keyw(str, tp ,att);
                }
            }

            if(i==-1)
            {
                i=0;
                break;
            }
            else if (i==-2)
            {
                i=0;
            }
            else {
                str[i]=ch;
                i++;
            }

        }
            //getch();

        return 0;
}

void keyw(char *p, char tp[], char att[])
{
            int k,flag=0;
            for(k=0;k<=11;k++)
            {
                if(strcmp(keys[k],p)==0)
                    {
                     strcpy(tp,"keyword");
                     strcpy(att,p);
                     kw++;
                     flag=1;

                     if (idType[0] != '\0')
                     {
                         if( strcmp(p,"main") !=0 ) printf("\nKeyw-215: Warning Unexpected idType at line %d, an ID after a Type is expected\n",*line);
                         idType[0]='\0';
                     }
                     else if(k<4) strcpy(idType,p);
                     break;
                }
            }
            if(flag==0)
            {
                if(isdigit(p[0]))
                    {
                     strcpy(tp,"number");
                     strcpy(att,p);
                     num++;
                }
                else
                {
                    //if(p[0]!=13&&p[0]!=10)
                    if(p[0]!='\0')
                    {
                        strcpy(tp,"identifier");
                        strcpy(att,p);

                        symbolT *st_ptr;
                        st_ptr= firstS;
                        int sym_index=-1;
                        while (st_ptr != NULL)
                        {
                          if ( strcmp(st_ptr->attrS,att)==0 ) sym_index = st_ptr->index;
                                st_ptr= st_ptr->next;
                        }

                        if ( sym_index != -1 && idType[0]!='\0' )
                            printf("\nKeyw-246: Unexpected idType at line %d, Redefinition of %s!\n",*line, att);
                        else if( sym_index == -1 && idType[0]=='\0' )
                            printf("\nKeyw-247: Unexpected idType at line %d, Undeclared identifier %s!\n",*line, att);
                        else if( sym_index == -1 )
                        {
                           Sindex ++;
                           symbolT *tmpS;
                           if(firstS==NULL)
                           {
                             tmpS= (symbolT *) malloc( sizeof(symbolT));
                             firstS=tmpS;
                             firstS->next=NULL;
                             strcpy(firstS->attrS,att);
                             strcpy(firstS->typeS,idType);
                             firstS->index= Sindex;
                             firstS->value=0;
                             printf("A new identifier, %s, inserted into the symbol table with index %d\n",firstS->attrS,firstS->index);
                           }
                           else
                           {
                             st_ptr=firstS;
                             while(st_ptr->next!=NULL)st_ptr=st_ptr->next;

                             tmpS= (symbolT *) malloc( sizeof(symbolT));
                             st_ptr->next=tmpS;
                             tmpS->next=NULL;
                             strcpy(tmpS->attrS,att);
                             strcpy(tmpS->typeS,idType);
                             tmpS->index= Sindex;
                             printf("A new identifier, %s, inserted into the symbol table with index %d\n",tmpS->attrS,tmpS->index);
                           }
                           idType[0]='\0';
                        }
                    }
                }
            }

            if ( p[0] =='\0' ) i=-2;
             else i=-1;
}

/*
void   next(){
        if(strcmp("/",firstT->typeT)==0)
                firstT=firstT->next;
        if(search(firstT->typeT)->value==0)
           {
            printf("divide on 0");
            endf(); return 0;}
        else firstT=firstT->next;

    }
firstS *search(char a[]){


    do{

        if(strcmp(a,firstS->atttrS)==0)
            return firstS;
    }
    while(firstS->next!=NULL);
    }
void endf(){
  while(strcmp(firstT->arrT,";")!=0||strcmp("keyword",firstT->typeT)!=0){
    next();
  }
     return 0;
  }
void mainff(){
    next();
   if(strcmp(firstT->arrT,"(")==0)
    next();
     else{printf("line is%d i read %s corect is(\n",firstT->lineT,firstT->arrT);
                      next(); endf(); return 0;}
        if(strcmp(firstT->arrT,")")==0)
           next();
            else{printf("line is%d i read %s corect is)\n",firstT->lineT,firstT->arrT);
                   next(); endf(); return 0;}
                   if(strcmp(firstT->arrT,"{")==0)
                           next();
                             else{printf("line is%d i read %s corect is {",firstT->lineT,firstT->arrT);
                                      next(); }

 }
void elsef(){
                next();
               if(strcmp(firstT->arrT,"{")==0)
                  next();
                        else {printf("line is%d i read %s corect is{\n",firstT->lineT,firstT->arrT);
                          next(); }
                             mainf();
                              if (strcmp(firstT->arrT,"}")==0)
                                    next();
                                else{printf("line is%d i read %s corect }is)\n",firstT->lineT,firstT->arrT);
                                   next();}

}
void iff(){
    next();
    if(strcmp(firstT->arrT,"(")==0)
    next();
     else{printf("line is%d i read %s corect is(\n",firstT->lineT,firstT->arrT);
                      next(); }
       if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
           next();
        else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT);
                next(); }
         if(strcmp(firstT->arrT,"==")==0||strcmp(firstT->arrT,">")==0||strcmp(firstT->arrT,"<")==0||strcmp(firstT->arrT,"!=")==0||strcmp(firstT->arrT,"<=")==0||strcmp(firstT->arrT,">=")==0)
                     next();
           else {printf("line is%d i read %s corect is ==,<,>,!=,>=,<=\n",firstT->lineT,firstT->arrT);
                 next();
           }
             if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
               next();
                else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT); next();}
                 if(strcmp(firstT->arrT,"||")==0||strcmp(firstT->arrT,"&&")==0){goto *51;}
                    if(strcmp(firstT->arrT,")")==0) next();
                     else{printf("line is%d i read %s corect is)\n",firstT->lineT,firstT->arrT);
                         next();}
                       if(strcmp(firstT->arrT,"{")==0){next();}
                        else {printf("line is%d i read %s corect is{\n",firstT->lineT,firstT->arrT);
                          next(); }
                               mainf();
                            if (strcmp(firstT->arrT,"}"==0))next();
                              else{printf("line is%d i read %s corect }is)\n",firstT->lineT,firstT->arrT);
                                  next();}}
void whilef(){
    next();
    if(strcmp(firstT->arrT,"(")==0)
    next();
     else{printf("line is%d i read %s corect is(\n",firstT->lineT,firstT->arrT);
                      next(); }
       if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
           next();
        else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT);
                next(); }
         if(strcmp(firstT->arrT,"==")==0||strcmp(firstT->arrT,">")==0||strcmp(firstT->arrT,"<")==0||strcmp(firstT->arrT,"!=")==0||strcmp(firstT->arrT,"<=")==0||strcmp(firstT->arrT,">=")==0)
                     next();
           else {printf("line is%d i read %s corect is ==,<,>,!=,>=,<=\n",firstT->lineT,firstT->arrT);
                 next();
           }
             if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
               next();
                else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT);next();}
                 if(strcmp(firstT->arrT,"||")==0||strcmp(firstT->arrT,"&&")==0){goto *80;}
                    if(strcmp(firstT->arrT,")")==0) next();
                     else{printf("line is%d i read %s corect is)\n",firstT->lineT,firstT->arrT);
                         next();}
                       if(strcmp(firstT->arrT,"{")==0){next();}
                        else {printf("line is%d i read %s corect is{\n",firstT->lineT,firstT->arrT);
                          next(); }
                               mainf();
                            if (strcmp(firstT->arrT,"}")==0)next();
                              else{printf("line is%d i read %s corect }is)\n",firstT->lineT,firstT->arrT);
                                  next();}
}
void intf(){
    next();


           if(strcmp("id",firstT->typeT)==0)
              {
            firstS *q=search(firstT->arrT);
            next();



            }
                else{printf("line is%d i read %s corect is id \n",firstT->lineT,firstT->arrT);
                   next(); endf(); return 0;}
                  if(strcmp("max",firstT->arrT)==0||strcmp("min",firstT->arrT)==0||strcmp("=",firstT->arrT)==0||strcmp(";",firstT->arrT)==0)
                    else {printf("line is%d i read %s corect is max min = ; \n",firstT->lineT,firstT->arrT);
                      endf(); return 0;}
                     //max or min
                      if(strcmp("max",firstT->arrT)==0||strcmp("min",firstT->arrT)==0)
                              {if(strcmp("(",firstT->arrT)==0)
                                     next();
                                        else{printf("line is%d i read %s corect is(\n",firstT->lineT,firstT->arrT);next();}
                                                   if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
                                                      next();
                                                         else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT); next();}
                                                              if(strcmp(",",firstT->arrT)==0)
                                                                   next();
                                                                  else{printf("line is%d i read %s corect is ,\n",firstT->lineT,firstT->arrT);
                                                                         next();}
                                                                           if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
                                                                                next();
                                                                                else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT); next();}
                                                                                    if(strcmp(")",firstT->arrT)==0) next();
                                                                                        else{printf("line is%d i read %s corect is)\n",firstT->lineT,firstT->arrT);
                                                                                             next();}
                                                                                              if(token()==';') {next();                                                           }
                                                                                                 else{printf("line is%d i read %s corect is;\n",firstT->lineT,firstT->arrT);
                                                                                                 next(); return 0; }}

                     //end of max or min
                     //;
                  if(strcmp(";",firstT->arrT)==0)
                    {next();
                     if(strcmp(";",firstT->arrT)!=0){printf("line is%d i read %s corect is;\n",firstT->lineT,firstT->arrT);
                         next();}
                           return 0;

                         }

                                     // end ;
                                     //=

                  if(strcmp("=",firstT->arrT)==0){next();
                        else {printf("line is%d i read %s corect is=\n",firstT->lineT,firstT->arrT);
                          next();}
                           if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
                           {


                            if(strcmp("id",firstT->typeT)==0){
                                 firstS *w=search(firstT->arrT);
                                        q->value=w->value;}
                                         else q->value=firstT->arrT;
                                  next();}
                                 else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT);
                                  next();}
                                 if(strcmp(";",firstT->arrT)==0||strcmp("+",firstT->arrT)==0||strcmp("/",firstT->arrT)==0||strcmp("*",firstT->arrT)==0||strcmp("-",firstT->arrT)==0)
                                    else {printf("line is%d i read %s corect is; + - / *\n",firstT->lineT,firstT->arrT);
                                       next();  endf(); return 0:}
                                      if(strcmp(";",firstT->arrT)==0) {next(); return 0;}

                                            if(strcmp("+",firstT->arrT)==0||strcmp("/",firstT->arrT)==0||strcmp("*",firstT->arrT)==0||strcmp("-",firstT->arrT)==0){int c;
                                                if (strcmp("+",firstT->arrT)==0)c=0;
                                                if (strcmp("/",firstT->arrT)==0)c=1;
                                                if (strcmp("*",firstT->arrT)==0)c=2;
                                                if (strcmp("-",firstT->arrT)==0)c=3;

                                                              next();}

                                                 if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
                           {


                            if(strcmp("id",firstT->typeT)==0){int b;
                                 firstS *z=search(firstT->arrT);
                                 if (z->value==NULL) {endf();return 0;}
                                 switch(c){
                                 case'0':q->value=q->value+z->value; break;
                                 case'1':q->value=q->value/z->value; break;
                                 case'2':q->value=q->value*z->value; break;
                                 case'3':q->value=q->value-z->value; break;



                                 }




                                        }
                                         else b=firstT->arrT;
                                         switch(c){
                                 case'0':q->value=q->value+b; break;
                                 case'1':q->value=q->value/ b;  break;
                                 case'2':q->value=q->value*b; break;
                                 case'3':q->value=q->value-b; break;



                                 }

                                  next();}
                                 else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT);
                                  next();}
                                                           if(strcmp(";",firstT->arrT)==0)
                                                                 next();
                                                                  else{printf("line is%d i read %s corect is;\n",firstT->lineT,firstT->arrT);
                                                                      next();}
                                                                          }


}
void floatf(){
    next();


           if(strcmp("id",firstT->typeT)==0)
              {
            firstS *q=search(firstT->arrT);
            next();



            }
                else{printf("line is%d i read %s corect is id \n",firstT->lineT,firstT->arrT);
                   next(); endf(); return 0;}
                  if(strcmp("max",firstT->arrT)==0||strcmp("min",firstT->arrT)==0||strcmp("=",firstT->arrT)==0||strcmp(";",firstT->arrT)==0)
                    else {printf("line is%d i read %s corect is max min = ; \n",firstT->lineT,firstT->arrT);
                      endf(); return 0;}
                     //max or min
                      if(strcmp("max",firstT->arrT)==0||strcmp("min",firstT->arrT)==0)
                              {if(strcmp("(",firstT->arrT)==0)
                                     next();
                                        else{printf("line is%d i read %s corect is(\n",firstT->lineT,firstT->arrT);next();}
                                                   if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
                                                      next();
                                                         else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT); next();}
                                                              if(strcmp(",",firstT->arrT)==0)
                                                                   next();
                                                                  else{printf("line is%d i read %s corect is ,\n",firstT->lineT,firstT->arrT);
                                                                         next();}
                                                                           if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
                                                                                next();
                                                                                else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT); next();}
                                                                                    if(strcmp(")",firstT->arrT)==0) next();
                                                                                        else{printf("line is%d i read %s corect is)\n",firstT->lineT,firstT->arrT);
                                                                                             next();}
                                                                                              if(token()==';') {next();                                                           }
                                                                                                 else{printf("line is%d i read %s corect is;\n",firstT->lineT,firstT->arrT);
                                                                                                 next(); return 0; }}

                     //end of max or min
                     //;
                  if(strcmp(";",firstT->arrT)==0)
                    {next();
                     if(strcmp(";",firstT->arrT)!=0){printf("line is%d i read %s corect is;\n",firstT->lineT,firstT->arrT);
                         next();}
                           return 0;

                         }

                                     // end ;
                                     //=

                  if(strcmp("=",firstT->arrT)==0){next();
                        else {printf("line is%d i read %s corect is=\n",firstT->lineT,firstT->arrT);
                          next();}
                           if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
                           {


                            if(strcmp("id",firstT->typeT)==0){
                                 firstS *w=search(firstT->arrT);
                                        q->value=w->value;}
                                         else q->value=firstT->arrT;
                                  next();}
                                 else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT);
                                  next();}
                                 if(strcmp(";",firstT->arrT)==0||strcmp("+",firstT->arrT)==0||strcmp("/",firstT->arrT)==0||strcmp("*",firstT->arrT)==0||strcmp("-",firstT->arrT)==0)
                                    else {printf("line is%d i read %s corect is; + - / *\n",firstT->lineT,firstT->arrT);
                                       next();  endf(); return 0:}
                                      if(strcmp(";",firstT->arrT)==0) {next(); return 0;}

                                            if(strcmp("+",firstT->arrT)==0||strcmp("/",firstT->arrT)==0||strcmp("*",firstT->arrT)==0||strcmp("-",firstT->arrT)==0){int c;
                                                if (strcmp("+",firstT->arrT)==0)c=0;
                                                if (strcmp("/",firstT->arrT)==0)c=1;
                                                if (strcmp("*",firstT->arrT)==0)c=2;
                                                if (strcmp("-",firstT->arrT)==0)c=3;

                                                              next();}

                                                 if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
                           {


                            if(strcmp("id",firstT->typeT)==0){int b;
                                 firstS *z=search(firstT->arrT);
                                 if (z->value==NULL) {endf();return 0;}
                                 switch(c){
                                 case'0':q->value=q->value+z->value; break;
                                 case'1':q->value=q->value/z->value; break;
                                 case'2':q->value=q->value*z->value; break;
                                 case'3':q->value=q->value-z->value; break;



                                 }




                                        }
                                         else b=firstT->arrT;
                                         switch(c){
                                 case'0':q->value=q->value+b; break;
                                 case'1':q->value=q->value/ b;  break;
                                 case'2':q->value=q->value*b; break;
                                 case'3':q->value=q->value-b; break;



                                 }

                                  next();}
                                 else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT);
                                  next();}
                                                           if(strcmp(";",firstT->arrT)==0)
                                                                 next();
                                                                  else{printf("line is%d i read %s corect is;\n",firstT->lineT,firstT->arrT);
                                                                      next();}
                                                                          }


}
void boolf(){
                 next();
    if(strcmp("id",firstT->typeT)==0){
        firstS *q =search(firstT->arrT);
                   next();}
                else{printf("line is%d i read %s corect is id \n",firstT->lineT,firstT->arrT);
                next();}
                     //;
                  if(strcmp(";",firstT->arrT)==0)
                    {next();
                     if(strcmp(";",firstT->arrT)!=0){printf("line is%d i read %s corect is;\n",firstT->lineT,firstT->arrT);
                         next();}
                           return 0;

                         }
                             if(strcmp("=",firstT->arrT)==0){next();}
                                else {printf("line is%d i read %s corect is=\n",firstT->lineT,firstT->arrT);
                                    next();}
                                     if(strcmp("true",firstT->arrT)==0||strcmp("false",firstT->arrT)==0)
                                                    next();
                                                      else{printf("line is%d i read %s corect is true or false\n",firstT->lineT,firstT->arrT);
                                                        next();}
                                               if(strcmp(";",firstT->arrT)==0)
                                                                 {next();
                                                        if(strcmp(";",firstT->arrT)!=0){printf("line is%d i read %s corect is;\n",firstT->lineT,firstT->arrT);
                                                                                                                   next();}
                                                                               return 0;

                                                                             }
                  }
void idf(){

           if(strcmp("id",firstT->typeT)==0)
              {
            firstS *q=search(firstT->arrT);
            next();



            }
                else{printf("line is%d i read %s corect is id \n",firstT->lineT,firstT->arrT);
                   next(); endf(); return 0;}
                  if(strcmp("max",firstT->arrT)==0||strcmp("min",firstT->arrT)==0||strcmp("=",firstT->arrT)==0||strcmp(";",firstT->arrT)==0)
                    else {printf("line is%d i read %s corect is max min = ; \n",firstT->lineT,firstT->arrT);
                      endf(); return 0;}
                     //max or min
                      if(strcmp("max",firstT->arrT)==0||strcmp("min",firstT->arrT)==0)
                              {if(strcmp("(",firstT->arrT)==0)
                                     next();
                                        else{printf("line is%d i read %s corect is(\n",firstT->lineT,firstT->arrT);next();}
                                                   if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
                                                      next();
                                                         else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT); next();}
                                                              if(strcmp(",",firstT->arrT)==0)
                                                                   next();
                                                                  else{printf("line is%d i read %s corect is ,\n",firstT->lineT,firstT->arrT);
                                                                         next();}
                                                                           if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
                                                                                next();
                                                                                else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT); next();}
                                                                                    if(strcmp(")",firstT->arrT)==0) next();
                                                                                        else{printf("line is%d i read %s corect is)\n",firstT->lineT,firstT->arrT);
                                                                                             next();}
                                                                                              if(token()==';') {next();                                                           }
                                                                                                 else{printf("line is%d i read %s corect is;\n",firstT->lineT,firstT->arrT);
                                                                                                 next(); return 0; }}

                     //end of max or min
                     //;
                  if(strcmp(";",firstT->arrT)==0)
                    {next();
                     if(strcmp(";",firstT->arrT)!=0){printf("line is%d i read %s corect is;\n",firstT->lineT,firstT->arrT);
                         next();}
                           return 0;

                         }

                                     // end ;
                                     //=

                  if(strcmp("=",firstT->arrT)==0){next();
                        else {printf("line is%d i read %s corect is=\n",firstT->lineT,firstT->arrT);
                          next();}
                           if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
                           {


                            if(strcmp("id",firstT->typeT)==0){
                                 firstS *w=search(firstT->arrT);
                                        q->value=w->value;}
                                         else q->value=firstT->arrT;
                                  next();}
                                 else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT);
                                  next();}
                                 if(strcmp(";",firstT->arrT)==0||strcmp("+",firstT->arrT)==0||strcmp("/",firstT->arrT)==0||strcmp("*",firstT->arrT)==0||strcmp("-",firstT->arrT)==0)
                                    else {printf("line is%d i read %s corect is; + - / *\n",firstT->lineT,firstT->arrT);
                                       next();  endf(); return 0:}
                                      if(strcmp(";",firstT->arrT)==0) {next(); return 0;}

                                            if(strcmp("+",firstT->arrT)==0||strcmp("/",firstT->arrT)==0||strcmp("*",firstT->arrT)==0||strcmp("-",firstT->arrT)==0){int c;
                                                if (strcmp("+",firstT->arrT)==0)c=0;
                                                if (strcmp("/",firstT->arrT)==0)c=1;
                                                if (strcmp("*",firstT->arrT)==0)c=2;
                                                if (strcmp("-",firstT->arrT)==0)c=3;

                                                              next();}

                                                 if(strcmp("id",firstT->typeT)==0||strcmp("number",firstT->typeT)==0)
                           {


                            if(strcmp("id",firstT->typeT)==0){int b;
                                 firstS *z=search(firstT->arrT);
                                 if (z->value==NULL) {endf();return 0;}
                                 switch(c){
                                 case'0':q->value=q->value+z->value; break;
                                 case'1':q->value=q->value/z->value; break;
                                 case'2':q->value=q->value*z->value; break;
                                 case'3':q->value=q->value-z->value; break;



                                 }




                                        }
                                         else b=firstT->arrT;
                                         switch(c){
                                 case'0':q->value=q->value+b; break;
                                 case'1':q->value=q->value/ b;  break;
                                 case'2':q->value=q->value*b; break;
                                 case'3':q->value=q->value-b; break;



                                 }

                                  next();}
                                 else{printf("line is%d i read %s corect is id or number\n",firstT->lineT,firstT->arrT);
                                  next();}
                                                           if(strcmp(";",firstT->arrT)==0)
                                                                 next();
                                                                  else{printf("line is%d i read %s corect is;\n",firstT->lineT,firstT->arrT);
                                                                      next();}
                                                                          }


}
void charf(){
    next();
    if(strcmp("id",firstT->typeT)==0)
               next();
                else{printf("line is%d i read %s corect is id \n",firstT->lineT,firstT->arrT); next();}
                   if(strcmp("=",firstT->arrT)==0 )next();
                        else {printf("line is%d i read %s corect is=\n",firstT->lineT,firstT->arrT);
                          next();}
                             if(strcmp("'",firstT->arrT)==0){next();}
                        else {printf("line is%d i read %s corect is'\n",firstT->lineT,firstT->arrT);
                          next();}








                           if(strcmp("'",firstT->arrT)==0){next();}
                        else {printf("line is%d i read %s corect is'\n",firstT->lineT,firstT->arrT);
                          next();}
                        if(strcmp(";",firstT->arrT)==0)
                    {next();
                     if(strcmp(";",firstT->arrT)!=0){printf("line is%d i read %s corect is;\n",firstT->lineT,firstT->arrT);
                         next();}
                           return 0;

                         }
}
void mainf(){
   switch(firstT->arrT)
   case"main":  mainff();break;
   case"if":    iff();break;
   case"else":  elsef();break;
   case"while": whilef();break;
   case"int":   intf();break;
   case"bool":  boolf();break;
   case"float": floatf();break;
   case"char": charf();break;
   default:
       {if(strcmp("id",firstT->typeT)==0){idf();}
    }
 }
int  main(){

   do{
        mainf();


    }
     while(firstT->next!=NULL)
    return 0;
}

    Status API Training Shop Blog About Pricing

    © 2016 GitHub, Inc. Terms Privacy Security Contact Help
*/
