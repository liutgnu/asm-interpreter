#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

extern int power(int,int);
extern int calc_num(char *);
struct varible{
	char name;
	int value;
};
#define ALPHABET 26
#define ALPHABET_LEN ALPHABET*2
struct varible varible_table[ALPHABET_LEN]={{'\0',0},};
#define OPCODE_LEN 10
#define OPERAND_LEN 32

struct instruction{
	char opcode[OPCODE_LEN];
	char operand_l[OPERAND_LEN];
	char operand_r[OPERAND_LEN];
};
#define LABEL_LEN 10
struct line{
	int line_num;
	char label[LABEL_LEN];
	struct instruction instruct;
};
#define MAX_LINE_LEN 100

#define EQUAL 0
#define UNEQUAL 1
#define STACK_LEN 10
int je_flag=UNEQUAL;
struct line * source_struct_table;
int total_line;
int simu_stack[STACK_LEN]={0,};
int stack_head=0;
int eip;  //eip==line_num

#define SUCCESS 0
#define FAIL -1

/*
 * functions below are used for preprocess source file
 */

int string_shrink(char * string_orig,char * string_dest){
    char * string_left=string_orig;
    char * string_right=string_dest;
    int i=0;
    if (*string_left=='\0'){
        *string_right='\0';
        return 0;
    }
    while(*string_left==' ' || *string_left=='\t')
        string_left++;
    while(*string_left!=' ' && *string_left!='\t' && *string_left!='\0'){
        *string_right=*string_left;
        string_left++;
        string_right++;
        i++;
    }
    *string_right='\0';
    return i;
}

void string_copy(char * string_orig, char * string_dest){
    char * string_left=string_orig;
    char * string_right=string_dest;
    while(*string_left!='\0'){
        *string_right=*string_left;
        string_left++;
        string_right++;
    }
    *string_right='\0';
}

int string_cmp(char * string_orig,char * string_dest){
    int i=0;
    while (*(string_orig+i)!='\0'){
        if (*(string_orig+i)!=*(string_dest+i))
            return 1;
        i++;
    }
    if (*(string_dest+i)!='\0')
        return 1;
    return 0;
}

int process_line(char * line_source,int line_num,struct line *instructor){
    char * string_left=line_source;
    char * string_right=line_source;
    int copy_length;
    while (*string_right!='\0'){
        if (*string_right==':')
            break;
        string_right++;
    }
    if (*string_right==':'){  //there is a label
        *string_right='\0';  //setup a string-ending
        if ((copy_length=string_shrink(string_left,instructor->label))>LABEL_LEN){  //copy label
            printf("label too long!\n");
            return FAIL;
        }
        string_right++;
        string_left=string_right;
    }else{                      //there is no label
        string_right=string_left;
    }
    
    while (*string_right==' ' || *string_right=='\t'){  //skip SPACES locating in the beginning
        string_right++;
    }
   
    string_left=string_right;
    while (*string_right!=' ' && *string_right!='\t' && *string_right!='\0'){
        string_right++;
    }
    if (*string_right=='\0'){  //just one opcode, no operand. so copy and exit
        instructor->line_num=line_num;
        string_shrink(string_left,instructor->instruct.opcode);
        return SUCCESS;
    }
    
    *string_right='\0';
    string_shrink(string_left,instructor->instruct.opcode);  //copy opcode
    string_right++;
    string_left=string_right;
    
    while ((*string_right==' ' || *string_right=='\t') && *string_right!='\0')
        string_right++;
    
    if (*string_right=='\0'){  //only opcode, no operand, like "mov  ". false line
        printf("error!no operand at line %d\n",line_num);
        return FAIL;
    }
    
    string_left=string_right;
    while (*string_right!=',' && *string_right!='\0')
        string_right++;
    if (*string_right=='\0'){  //no ',' ,source line like "print asdasd" 
        if ((copy_length=string_shrink(string_left,instructor->instruct.operand_l))>OPERAND_LEN){
            printf("left operand too long!\n");
            return FAIL;
        }
        instructor->line_num=line_num;
        return SUCCESS;
    }else{  //*string_right==','
        *string_right='\0';
        if ((copy_length=string_shrink(string_left,instructor->instruct.operand_l))>OPERAND_LEN){
            printf("left operand too long!\n");
            return FAIL;
        }
        string_right++;
        string_left=string_right;
    }
    if ((copy_length=string_shrink(string_left,instructor->instruct.operand_r))>OPERAND_LEN){
        printf("right operand too long!\n");
        return FAIL;
    }
    instructor->line_num=line_num;
    return SUCCESS;
}

struct line * process_file(char * source,int * line_total){
    char * string_left=source;
    char * string_right=source;
    int max_line=1;
    int line_num=0;
    int tmp;
    char line_buf[MAX_LINE_LEN]={'\0',};
    
    while (*string_right!='\0'){
        if (*string_right=='\n')
            max_line++;
        string_right++;
    }
    *line_total=max_line;
    
    struct line *source_block=(struct line *)malloc(max_line*sizeof(struct line));
    if (source_block==NULL){
        printf("create source_block failed!\n");
        return NULL;
    }
    
    bzero(source_block,max_line*sizeof(struct line));
    string_right=string_left;
    while (line_num<max_line){
        while (*string_right!='\n' && *string_right!='\0')
            string_right++;
        *string_right='\0';
        if ((tmp=process_line(string_left,line_num+1,source_block+line_num))==FAIL){
            free(source_block);
            return NULL;
        }
        string_right++;
        string_left=string_right;
        line_num++;
    }
    return source_block;
}

/*
 * now the initial function
 */

void init_varible(){
    int i;
    for (i=0;i<ALPHABET;i++){
        varible_table[i].name=(char)(i+97);
    }
    for (i=0;i<ALPHABET;i++){
        varible_table[ALPHABET+i].name=(char)(i+65);
    }
}

/*
 * OK, now we deal with run-time functions
 */

//scan varible table, return varible's value pointer
int * get_varible(char var_name){
    struct varible * var_item=varible_table;
    int i=0;
    for (;i<ALPHABET_LEN;i++){
        if (var_name==(var_item+i)->name){
            return &((var_item+i)->value);
        }
    }
    if (i==ALPHABET_LEN){  //it's not gonna happen
        return NULL;
    }
}

//replace varibles in numeric expression, like "x+2-y"
char * relace_varible(char * expression){
    char * string_left=expression;
    char * string_right=expression;
    char * real_expression=(char *)malloc(MAX_LINE_LEN);
    char * real_expression_start=real_expression;
    bzero(real_expression,MAX_LINE_LEN);
    int * value_addr;
    int value,i;
    if (real_expression==NULL){
        printf("create real_expression failed!\n");
        return NULL;
    }
    
    while (*string_right!='\0'){
        if ((*string_right>='a' && *string_right<='z') || (*string_right>='A' && *string_right<='Z')){
            //copy non-varible part
            while (string_left<string_right){
                *real_expression=*string_left;
                real_expression++;
                string_left++;
            }
            value_addr=get_varible(*string_right);
            value=*value_addr;  //get the value
            
            //now we convert int to string
            if (value<0){
                *(real_expression++)='-';
                value=0-value;
            }
            
            i=0;
            while(value/power(10,i))
                i++;
            i--;
            
            while(i>=0){
                *real_expression=value/power(10,i)+48;
                value=value%power(10,i);
                i--;
                real_expression++;
            }
            string_right++;
            string_left=string_right;
        }else{
            string_right++;
        }
    }
    string_copy(string_left,real_expression);  //deal with the last part
    return real_expression_start;
}

int simu_push(int line_num){
    if (stack_head==STACK_LEN-1){
        printf("error!stack full\n");
        return FAIL;
    }
    simu_stack[stack_head]=line_num;
    stack_head++;
    return SUCCESS;
}

int simu_pop(){
    if (stack_head==0){
        printf("error!stack empty\n");
        return FAIL;
    }
    return (simu_stack[--stack_head]);
}

//sub functions:
    //mov x,23  case:A
int func_mov(char * operand_l,char * operand_r){
    char *real_operand_r=relace_varible(operand_r);
    int value=calc_num(real_operand_r);
    int *dest=get_varible(*operand_l);
    *dest=value;
    free(real_operand_r);
    return SUCCESS;
}

    //cmp x,y   case:B
int func_cmp(char * operand_l,char * operand_r){
    char *real_operand_r=relace_varible(operand_r);
    int value_r=calc_num(real_operand_r);
    char *real_operand_l=relace_varible(operand_l);
    int value_l=calc_num(real_operand_l);
    free(real_operand_r);
    free(real_operand_l);
    if (value_r==value_l)
        je_flag=EQUAL;
    else
        je_flag=UNEQUAL;
    return SUCCESS;
}

    //je label  case:C
int func_je(char * operand_l){
    int i=0;
    while (i<total_line){
        if (!string_cmp((source_struct_table+i)->label,operand_l))
            break;
        i++;
    }
    if (i==total_line){
        printf("error!no label %s\n",operand_l);
        return FAIL;
    }
    
    if (je_flag==EQUAL){  //OK,we are authorized to jump
        eip=i;
        je_flag=UNEQUAL;
    }  //else do nothing
    return SUCCESS;  
}

    //jmp label case:D
int func_jmp(char * operand_l){
    int i=0;
    while (i<total_line){
        if (!string_cmp((source_struct_table+i)->label,operand_l))
            break;
        i++;
    }
    if (i==total_line){
        printf("error!no label %s\n",operand_l);
        return FAIL;
    }
    eip=i;
    return SUCCESS;   
}

    //print "hello,world"  print x+y+z  case:E
int func_print(char * operand_l){
    char * string_right=operand_l;
    char * tmp;
    while (*string_right!='\0')
        string_right++;
    if (*operand_l=='"'){  //print string
        if (*(string_right-1)!='"'){
            printf("error!\" without pair!\n");
            return FAIL;
        }else{
            *(string_right-1)='\0';
            printf("%s\n",operand_l+1);
            *(string_right-1)='"';
            return SUCCESS;
        }
    }
    
    tmp=relace_varible(operand_l);
    printf("%d\n",calc_num(tmp));
    free(tmp);
    return SUCCESS;
}

    //call print_num    case:F
int func_call(char * operand_l){
    int i=0;
    while (i<total_line){
        if (!string_cmp((source_struct_table+i)->label,operand_l))
            break;
        i++;
    }
    if (i==total_line){
        printf("error!no function %s\n",operand_l);
        return FAIL;
    }

    if (!simu_push(eip+1)){
        eip=(source_struct_table+i)->line_num-1;  //remember eip++ in line 492
        return SUCCESS;
    }else
        return FAIL;
}

    //ret   case:G
int func_ret(){
    int line_num=simu_pop();
    if (line_num==FAIL)
        return FAIL;
    eip=line_num-1;  //remember eip++ in line 492
    return SUCCESS;
}

    //halt  case:H
int func_halt(){
    printf("program finished normally\n");
    return SUCCESS;
}

    //other case:I
int func_other(){
    return SUCCESS;
}
    
/*
 * now the real interpreter
 */

int interpreter(struct line * source_table){
    eip=1;
    int (* function)();
    int tmp;
    while(eip<=total_line){
        if (!string_cmp((source_table+eip-1)->instruct.opcode,"mov"))
            function=func_mov;
        else if (!string_cmp((source_table+eip-1)->instruct.opcode,"cmp"))
            function=func_cmp;
        else if (!string_cmp((source_table+eip-1)->instruct.opcode,"je"))
            function=func_je;
        else if (!string_cmp((source_table+eip-1)->instruct.opcode,"jmp"))
            function=func_jmp;
        else if (!string_cmp((source_table+eip-1)->instruct.opcode,"print"))
            function=func_print;
        else if (!string_cmp((source_table+eip-1)->instruct.opcode,"call"))
            function=func_call;
        else if (!string_cmp((source_table+eip-1)->instruct.opcode,"ret"))
            function=func_ret;
        else if (!string_cmp((source_table+eip-1)->instruct.opcode,"halt"))
            function=func_halt;
        else 
            function=func_other;
        tmp=(*function)((source_table+eip-1)->instruct.operand_l,(source_table+eip-1)->instruct.operand_r);
        if (function==func_halt)
                break;
        if (tmp){
            printf("execution failed at line %d\n",(source_table+eip-1)->line_num);
            return FAIL;
        }
        eip++;
    }
    return SUCCESS;
}

void main(int argc,char ** argv){
    struct stat buf;
    char source_buf[2048]={'\0',};
    if (argc!=2){
        printf("Useage:./a.out file\n");
        exit(-1);
    }
    int fd=open(argv[1],O_RDONLY);
    if (fd<0){
        printf("open %s error!\n",argv[1]);
        return;
    }
    fstat(fd,&buf);
    read(fd,source_buf,buf.st_size);
    
    init_varible();
    if ((source_struct_table=process_file(source_buf,&total_line))!=NULL){
        interpreter(source_struct_table);
        free(source_struct_table);
        printf("total line %d executed\n",total_line);
    }

    close(fd);
}