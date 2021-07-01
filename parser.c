#include "include/parser.h"

#ifdef STOPWATCH

double stw_load_time = -1;
double stw_look_time = -1;
double stw_read_time = -1;
double stw_parse_time = -1;
int stw_file_length = -1;

double stw_speed(int length, double time)
{
	return (double)length / 1000 / time ;
}

#endif


int main(int argc, char* argv[])
{
	char* result;

#ifdef STOPWATCH
	tick();
#endif

	load();

#ifdef STOPWATCH
	stw_load_time = tick();
#endif

	if (argc >= 2)
	{
#ifdef STOPWATCH
		fprintf(stderr, "load time : %.3f ms\n", stw_load_time);
#endif
		result = parse_file(argv[1], NULL);
		if (result != NULL)
		{
			//fprintf(stdout, "%s\n", result);
			free(result);
		}
	}
	else
	{
		char cmd[256];
		while (fgets(cmd, 256, stdin) != NULL)
		{
			strtok(cmd, "\n");
			FILE* fp = fopen(cmd, "rb");
			if (fp)
			{
#ifdef STOPWATCH
				tick();
#endif
				result = parse_file(cmd, fp);
				if (result != NULL)
				{
					//fprintf(stdout, "%s\n", result);
					free(result);
				}
			}
			else
			{
				if (!strcmp(cmd, "streaming"))
				{
					int ret;
					size_t length;
					char* result;
					char* str;
					
					ret = fread(&length, sizeof(unsigned long), 1, stdin);
					
					str = malloc(length + 1);
					ret = fread(str, sizeof(char), length, fp);
					str[length] = '\0';
					
					result = parser(str, length);
					printf("%s", result);
					free(result);
				}
#ifdef STOPWATCH
				else if (!strcmp(cmd, "load"))
				{
					printf("load time : %.3f ms\n", stw_load_time);
				}
#endif
				else if (!strcmp(cmd, "tag"))
				{
					for (size_t i = 0; i < tag_n; i++)
					{
						printf("%7llu\t%s\n", i, tags[i].name);
					}
				}
				else if (!strcmp(cmd, "psm"))
				{
					printf("Total States Generated : %llu\n", ps_n);
				}
				else if (!strcmp(cmd, "q") || !strcmp(cmd, "quit") || !strcmp(cmd, "bye") || !strcmp(cmd, "exit"))
				{
					break;
				}
			}


		}
	}

	return 0;
}

char* parse_file(char* path, FILE* fp)
{
	char* str;
	int ret;

	size_t length;

	if (!fp)
	{
#ifdef STOPWATCH
		tick();
#endif
		fp = fopen(path, "rb");
	}

	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
		fseek(fp, 0, SEEK_SET);  /* same as rewind(f); */

#ifdef STOPWATCH
		stw_look_time = tick();
#endif

		str = malloc(length + 1);
		ret = fread(str, sizeof(char), length, fp);
		str[length] = '\0';

		fclose(fp);

#ifdef STOPWATCH
		stw_file_length = length;
		stw_read_time = tick();
#endif

		return parser(str, length);
	}
	else
	{
		fprintf(stderr, "File does not exist.\n");
		return NULL;
	}
}

char* parser(unsigned char* str, const size_t length)
{
#ifdef STOPWATCH
	tick();
#endif

	register ps* pso;
	register ps* pso_fallback;
	register ps_event pse;
	ps* cpso = ps_html_close_tag_in;
	ps ps_first;

	register const unsigned char* i;
	register const unsigned char* p;

	ps_first = (*ps_html_string);
	pso = &ps_first;
	pso_fallback = pso;

	i = str;
	p = str;

	do
	{
		pse = pso->event[*i];

		//printf("%c%c\t%s:%s\n", *i == '\t' ? '\\' : *i == '\n' ? '\\' : *i == '\r' ? '\\' : *i, *i == '\t' ? 't' : *i == '\n' ? 'n' : *i == '\r' ? 'r' : ' ', pso->name, ps_event_str[pse]);
		switch (pse)
		{
		case EV_NONE:
			break;

		case EV_TAG_IN:
			break;

		case EV_TAG_IN_REJECT:
			pso = pso_fallback;
			break;

		case EV_TAG_NAME_IN_FIRST:
			p = i;
			//pso = 
			push_ps_html_tag_name(pso, ps_html_tag_name, *i, EV_TAG_NAME_IN);
			cpso = push_ps_html_tag_name(ps_html_close_tag_in, ps_html_close_tag_name, *i, EV_NONE);

			break;

		case EV_TAG_NAME_IN:
			p = i;
			cpso = ps_html_close_tag_in->next[*i];
			break;

		case EV_TAG_NAME:
			push_ps_html_tag_name(pso, ps_html_tag_name, *i, EV_NONE);
			cpso = push_ps_html_tag_name(cpso, ps_html_close_tag_name, *i, EV_NONE);
			break;

		case EV_TAG_NAME_SAVE:
			goto EV_TAG_NAME_SAVE;

		EV_TAG_NAME_SAVE:
			cpso->tag_id = pso->tag_id = tag_new(p, i - p);
			tags[pso->tag_id].is_single = 0;
			set_ps_html_tag_name_out_event(pso, EV_TAG_NAME_OUT);
			set_ps_html_tag_name_out_event(cpso, EV_CLOSE_TAG_NAME_OUT);

			goto EV_TAG_STACK_PUSH;

		case EV_TAG_NAME_OUT:
			goto EV_TAG_STACK_PUSH;

		EV_TAG_STACK_PUSH:

			// TAG STACK PUSH CODE

			//printf("%s\n", tags[pso->tag_id].name);
			break;

		case EV_TAG_NAME_OUT_SCRIPT:
			pso_fallback = ps_script;
			goto EV_TAG_STACK_PUSH;

		case EV_TAG_NAME_OUT_CSS:
			pso_fallback = ps_script;
			goto EV_TAG_STACK_PUSH;

		case EV_TAG_OUT:
			pso = pso_fallback;
			break;

		case EV_TAG_OUT_NAME_SAVE:
			pso = pso_fallback;
			goto EV_TAG_NAME_SAVE;

		case EV_CLOSE_TAG_NAME_OUT:

			// TAG STACK POP CODE

			//printf(" --> %s\n", tags[pso->tag_id].name);
			pso_fallback = ps_html_string;
			break;

		case EV_SCRIPT_TAG_IN:
			pso_fallback = pso;
			break;

		case EV_SCRIPT_TAG_OUT:
			pso = pso_fallback;
			break;
		}

		// context switching
		pso = pso->next[*i];
	} while (*++i); // EOF = \0 = zero


#ifdef STOPWATCH
	stw_parse_time = tick();
#endif
	
#ifdef STOPWATCH
	fprintf(stderr, "file size : ");
	fprintfcomma(stderr, stw_file_length);
	fprintf(stderr, " Bytes\n");
	
	fprintf(stderr, "file size lookup time : %.3f ms\n", stw_look_time);
	fprintf(stderr, "read time : %.3f ms\n", stw_read_time);
	fprintf(stderr, "read speed : %.3f MB/s\n", stw_speed(stw_file_length, stw_read_time));
	fprintf(stderr, "string processing time : %.3f ms\n", stw_parse_time);
	fprintf(stderr, "string processing speed : %.3f MB/s\n", stw_speed(stw_file_length, stw_parse_time));
	
	stw_look_time = -1;
	stw_read_time = -1;
	stw_parse_time = -1;
#endif

	return str;
}