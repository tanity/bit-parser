// parse.c
char* parser(unsigned char* str, const size_t length);
char* parse_file(char* path, FILE* fp);

// event.h
typedef enum ps_event ps_event;

// state.h

// ps.h
typedef struct parser_state ps;
ps* ps_new();
ps* push_ps_html_tag_name(ps* const pso, ps* const ps_source, unsigned char const c, const ps_event E);
void set_ps_html_tag_name_out_event(ps* const pso, const ps_event E);
void set_ps_html_close_tag_name_out(ps* const cpso, const ps_event E);
void init_ps(ps* X, ps* Y, ps_event E);
void init_ps_none(ps* X, ps_event E);
void ps_set(ps* X, unsigned const char C, ps* Y, ps_event E);
void ps_set_event(ps* X, unsigned const char C, ps_event E);
void ps_set_ci(ps* X, unsigned const char C, ps* Y, ps_event E);


// tag.h
typedef struct parser_tag tag;
size_t tag_new(const unsigned char* p, size_t length);

// load.h
void load();
void load_ps();
void load_ps_html();
void load_ps_html_tag(const unsigned char* const tag, bool is_single);
void load_ps_html_tag_script();




// ASCII  A : 65, a : 96, therefore (A+32=a)
#define tolower(C) ('A' <= C && C <= 'Z') ? C+32 : C

#define toupper(C) ('a' <= C && C <= 'z') ? C-32 : C

#define strtolower(S) \
{ \
	register char *SP = S; \
	while (*SP) { \
		if ('A' <= *SP && *SP <= 'Z') (*SP)+=32; \
		*SP++; \
	} \
}

#define strtoupper(S) \
{ \
	register char *SP = S; \
	while (*SP) \
	{ \
		if ('a' <= *SP && *SP <= 'a') (*SP)-=32; \
		*SP++; \
	} \
}

#define CHAR_ASCII_CONTROL 0x00
#define CHAR_ASCII_ONLY 0x10
#define CHAR_ASCII_NUMERIC 0x40
#define CHAR_ASCII_ALPHABET_UPPERCASE 0x50
#define CHAR_ASCII_ALPHABET_LOWERCASE 0x51
#define CHAR_CYRILLIC_ONLY 0x81
#define CHAR_UTF8_DATA_ONLY 0x82
#define CHAR_UTF8_DATA_CYRILLIC 0x83
#define CHAR_UTF8_2BYTE_ONLY 0x84
#define CHAR_UTF8_2BYTE_CYRILLIC 0x85
#define CHAR_UTF8_3BYTE_ONLY 0x86
#define CHAR_UTF8_3BYTE_CYRILLIC 0x87
#define CHAR_UTF8_4BYTE_ONLY 0x88

unsigned char get_char_type(unsigned char c)
{
	unsigned char ret = 0;

	if (0x30 <= c && c <= 0x39)
	{
		ret = CHAR_ASCII_NUMERIC;
	}
	else if (0x41 <= c && c <= 0x5A)
	{
		ret = CHAR_ASCII_ALPHABET_UPPERCASE;
	}
	else if (0x61 <= c && c <= 0x7A)
	{
		ret = CHAR_ASCII_ALPHABET_LOWERCASE;
	}
	else if (0x80 <= c && c < 0xC0)
	{
		ret = CHAR_UTF8_DATA_ONLY;
	}
	else if (0xC0 <= c && c < 0xE0)
	{
		ret = CHAR_UTF8_2BYTE_ONLY;
	}
	else if (0xE0 <= c && c < 0xF0)
	{
		ret = CHAR_UTF8_3BYTE_ONLY;
	}
	else if (0xF0 <= c && c < 0xF8)
	{
		ret = CHAR_UTF8_4BYTE_ONLY;
	}
	else
	{
		ret = CHAR_ASCII_ONLY;
	}

	// is_cyrillic
	ret += (\
		(0x80 <= c && c <= 0xA7 && c != 0x9C && c != 0x9E) || \
		(0xB5 <= c && c <= 0xB7) || \
		(0xC6 <= c && c <= 0xC7) || \
		(0xD0 <= c && c <= 0xD8) || \
		(0xE0 <= c && c <= 0xED) \
		);

	return ret;
}


//https://stackoverflow.com/questions/1449805/how-to-format-a-number-from-1123456789-to-1-123-456-789-in-c
// printfcomma
void fprintfcomma(FILE* fp, int num)
{
	size_t n1 = (num < 0) ? -num : num;
	size_t n2 = 0;
	size_t scale = 1;

	if (num < 0)
	{
		fprintf(fp, "-");
	}

	while (n1 >= 1000)
	{
		n2 = n2 + scale * (n1 % 1000);
		n1 /= 1000;
		scale *= 1000;
	}

	fprintf(fp, "%lu", n1);

	while (scale != 1) {
		scale /= 1000;
		n1 = n2 / scale;
		n2 = n2 % scale;
		fprintf(fp, ",%03lu", n1);
	}
}
