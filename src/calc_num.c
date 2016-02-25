#include<stdio.h>
#include<stdlib.h>
//expr_node type
#define OPERATOR 1
#define NUMBER 2
#define START 3
//expr_node weight
#define NOPE 0

struct expr_node{
	int type;
	int value;
	struct expr_node * left;
	struct expr_node * right;
	int weight;
};

void free_all(struct expr_node * start){
	struct expr_node *p=start->right;
	while (p->right!=NULL){
		p=p->right;
	}
	while(p!=start){
		p=p->left;
		free(p->right);
	}
}

struct expr_node * create_node(struct expr_node * start,struct expr_node tmp){
	struct expr_node * node=(struct expr_node *)malloc(sizeof(struct expr_node));
	if (node==NULL){
		printf("create failed!\n");
		free_all(start);
		exit(-1);
	}else{
		node->type=tmp.type;
        node->value=tmp.value;
        node->left=tmp.left;
        node->right=tmp.right;
        node->weight=tmp.weight;
	}
	return node;
}

int power(int base,int index){
	int tmp=1;
	while(index>0){
		tmp*=base;
		index--;
	}
	return tmp;
}

struct expr_node * part_calc(struct expr_node * start,struct expr_node * operator_node){
    if (operator_node->type!=OPERATOR){
        printf("error operator!\n");
        return NULL;
    }
    struct expr_node tmp;
    struct expr_node * node;
    int value;
    
    switch ((char)(operator_node->value)){
        case '+':value=operator_node->left->value+operator_node->right->value;break;
        case '-':value=operator_node->left->value-operator_node->right->value;break;
        case '*':value=operator_node->left->value*operator_node->right->value;break;
        case '/':value=operator_node->left->value/operator_node->right->value;break;
        case '^':value=power(operator_node->left->value,operator_node->right->value);break;
    }
    tmp.type=NUMBER;
    tmp.value=value;
    tmp.left=NULL;
    tmp.right=NULL;
    tmp.weight=-1;
    node=create_node(start,tmp);
    return node;
}

int replace_node(struct expr_node * start,struct expr_node * operator_node){
    struct expr_node * new_node=part_calc(start,operator_node);
    if (new_node==NULL)
        return -1;
    if (operator_node->right->right==NULL){  //no more in both and more in left
        operator_node->left->left->right=new_node;
        new_node->left=operator_node->left->left;
        free(operator_node->right);
        free(operator_node->left);
        free(operator_node);
        return 1;
    }else{  //more in right and both
        operator_node->left->left->right=new_node;
        new_node->left=operator_node->left->left;
        operator_node->right->right->left=new_node;
        new_node->right=operator_node->right->right;
        free(operator_node->right);
        free(operator_node->left);
        free(operator_node);
        return 1;
    }
}

int calc_expression(struct expr_node * start){
    struct expr_node * max_weight_node;
    struct expr_node * scaner;
    int finish_flag=1;
    int result;
    
    while(finish_flag==1){    
        max_weight_node=start;
        scaner=start; 
    
        while(scaner){
            if (scaner->type!=OPERATOR){
                scaner=scaner->right;
                continue;
            }
            if (scaner->weight>max_weight_node->weight){
                max_weight_node=scaner;
            }
            scaner=scaner->right;
        }
        if (max_weight_node==start){
            finish_flag=0;
        }else{
            finish_flag=replace_node(start,max_weight_node);
        }
    }
    if (finish_flag==-1)
        return 0;
    else{
        result=start->right->value;
        free(start->right);
        start->right=NULL;
        return result;
    }
}

int calc_num(char * formula){
	int baseweight=0;
    struct expr_node init={START,0,NULL,NULL,0};
    struct expr_node * start=(struct expr_node *)&init;
	struct expr_node * last=start;
	char * expression=formula;
	char * slice_start=expression;
	char * slice_end=NULL;
	struct expr_node parameter;
	int numoff=0;
	int number=0;

	while (1){
		if (*expression=='('){
			baseweight+=4;
			*expression=' ';
			expression++;
			continue;
		}
		if (*expression==')'){
			baseweight-=4;
			*expression=' ';
			expression++;
			continue;
		}
		if (*expression=='+' || *expression=='-' ||*expression=='*' ||*expression=='/' ||*expression=='^' || *expression=='\0'){
			slice_end=expression-1;
			while (*slice_end==' ' && slice_end!=slice_start){
				slice_end--;
			}
			while (*slice_start==' ' && slice_start!=slice_end){
				slice_start++;
			}
			if (*slice_start==' '){
				printf("expression error!\n");
				exit(-1);
			}
			while(slice_start<=slice_end-numoff){
				number=number+(*(slice_end-numoff)-48)*power(10,numoff);
				numoff++;
			}
			parameter.type=NUMBER;
            parameter.value=number;
            parameter.left=last;
            parameter.right=NULL;
            parameter.weight=-1;
            last->right=create_node(start,parameter);
			last=last->right;
			number=0;
			numoff=0;
			slice_start=expression+1;
			if (*expression=='\0')
				break;
		}

		if (*expression=='+' || *expression=='-'){
			parameter.type=OPERATOR;
			parameter.value=(int)*expression;
			parameter.left=last;
			parameter.right=NULL;
			parameter.weight=baseweight+1;
            last->right=create_node(start,parameter);
            last=last->right;
			slice_start=expression+1;
		}
        if (*expression=='*' || *expression=='/'){
            parameter.type=OPERATOR;
            parameter.value=(int)*expression;
            parameter.left=last;
            parameter.right=NULL;
            parameter.weight=baseweight+2;
            last->right=create_node(start,parameter);
            last=last->right;
			slice_start=expression+1;
        }
        if (*expression=='^'){
            parameter.type=OPERATOR;
            parameter.value=(int)*expression;
            parameter.left=last;
            parameter.right=NULL;
            parameter.weight=baseweight+3;
            last->right=create_node(start,parameter);
            last=last->right;
			slice_start=expression+1;
        }
		expression++;
	}
    return(calc_expression(start));
}