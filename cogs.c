#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Cogs */

typedef char *str;
typedef void fn_t;
typedef void *handler_t;

typedef struct {
	str 		name;
	handler_t 	fn;
} Cog;

#define __COG_SZ__ sizeof(Cog)

typedef Cog *cog_t;		  // Cog *
typedef cog_t *cog_arr_t; // Cog **

typedef struct {
	cog_arr_t 	handlers;
	int 		idx;
} Cogs;

typedef Cogs *cogs_t;

cogs_t init_cogs()
{
	cogs_t cogs = (Cogs *)malloc(sizeof(Cogs));

	cogs->handlers = (cog_arr_t)malloc(sizeof(Cog *) * 1);
	cogs->idx = 0;

	return cogs;
}

int add_cog(cogs_t cogs, str name, handler_t fn)
{
	if(!cogs || !fn)
		return 0;

	cog_t cog = (cog_t)malloc(__COG_SZ__);
	cog->name = strdup(name);
	cog->fn = fn;

	cogs->handlers[cogs->idx] = cog;
	cogs->idx++;
	cogs->handlers = (Cog **)realloc(cogs->handlers, sizeof(Cog *) * (cogs->idx + 1));

	return 1;
}

int add_remove(cogs_t cogs, handler_t fn)
{
	if(!cogs || !fn)
		return 0;

	cog_arr_t handlers = (cog_arr_t)malloc(sizeof(Cog *) * 1);
	int idx = 0;
	for(int i = 0; i < cogs->idx; i++)
	{
		if(!cogs->handlers[i])
			break;

		handlers[idx++] = cogs->handlers[i];
	}

	handlers[idx] = NULL;

	cogs->handlers = handlers;
	cogs->idx = idx;
	return 1;
}

fn_t cog_Destruct(cog_t cog)
{
	if(!cog)
		return;

	if(cog->name)
		free(cog->name);

	free(cog);
}

fn_t cogs_Destruct(cogs_t cogs)
{
	if(cogs->handlers)
	{
		for(int i = 0; i < cogs->idx; i++)
		{
			if(!cogs->handlers[i])
				break;

			cog_Destruct(cogs->handlers[i]);
		}
	}
}

int find_cmd(cogs_t cogs, str cmd)
{
	for(int i = 0; i < cogs->idx; i++) {
		if(cogs->handlers[i]->name, cmd)
			return i;
	}

	return -1;
}

void _test_fn() { printf("NIG\n"); }

int main(int argc, char *argv[])
{
	cogs_t cogs = init_cogs();
	add_cog(cogs, "test", _test_fn);

	int pos = find_cmd(cogs, "test");
	printf("%d\n", pos);
	
	((void *(*)())cogs->handlers[pos]->fn)();
}
