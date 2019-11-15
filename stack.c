#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include"stack.h"
typedef struct node {
	unsigned long long value;
	struct node *prev;
	struct node *next;
} node;
extern void free_resources(void);
node *stack = NULL;

void stack_push(node *new_node)
{
	if (!stack) {
		stack = new_node;
		new_node->next = stack;
		new_node->prev = stack;
	} else {
		new_node->next = stack;
		new_node->prev = stack->prev;
		stack->prev->next = new_node;
		stack->prev = new_node;
		stack = new_node;
	}
}

node *stack_pop(void) {
	node *tmp;
	if (!stack) {
		return NULL;
	} 
	if (stack->next == stack) {
		tmp = stack;
		stack = NULL;
		return tmp;
	} else {
		stack->next->prev = stack->prev;
		stack->prev->next = stack->next;
		tmp = stack;
		stack = stack->next;
		return tmp;
	}
}

void push_address(unsigned long long value)
{
	node *new_node = malloc(sizeof(node));
	if (!new_node) {
		printf("malloc call func error!\n");
		free_resources();
		exit(-1);
	}
	memset(new_node, 0, sizeof(node));
	new_node->value = value;
	stack_push(new_node); 
}

unsigned long long pop_address(void)
{
	unsigned long long value;
	node *node = stack_pop();
	if (!node) {
		printf("stack empty!\n");
		free_resources();
		exit(-1);
	}
	value = node->value;
	free(node);
	return value;
}
