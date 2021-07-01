#define psm_unit 2048

ps* psm;
size_t ps_n = 0;

struct parser_state {
	ps* next[256];
	ps_event event[256];

	size_t id;
	size_t tag_id;

	char name[256];
};

ps* ps_new()
{
	if (ps_n % psm_unit == 0)
	{
		ps* psm_tmp;

		// Realloc Doesn't work... That's buggy and I don'y know why...
		// and it is slow maybe because an array must be serialized order on memory space...
		// so manual assignment of some bytes by malloc and do memcpy is much faster and safe

		// psm_tmp = (ps*)realloc(psm, sizeof(ps) * ((int)ps_n / psm_unit + 1) * psm_unit);
		psm_tmp = (ps*)malloc(sizeof(ps) * ((size_t)ps_n / psm_unit + 1) * psm_unit);

		if (psm_tmp != NULL)
		{
			if (ps_n) memcpy(psm_tmp, psm, sizeof(ps) * (ps_n / psm_unit) * psm_unit);
			psm = psm_tmp;
		}
	}

	psm[ps_n].id = ps_n;

	return &psm[ps_n++];
}

ps* push_ps_html_tag_name(ps* const pso, ps* const ps_source, unsigned char const c, const ps_event E)
{
	ps* pso_next;

	pso_next = ps_new();
	*pso_next = *ps_source;
#ifdef PS_NAME
	sprintf(pso_next->name, "ps_%llu", ps_n);
#endif
	ps_set_ci(pso, c, pso_next, E);

	//printf("<%c:%d>\n", c, pso->next[c]);

	return pso_next;
}

void set_ps_html_tag_name_out_event(ps* const pso, const ps_event E)
{
	ps_set_event(pso, '>', E);
	ps_set_event(pso, '\n', E);
	ps_set_event(pso, '\r', E);
	ps_set_event(pso, '\t', E);
	ps_set_event(pso, ' ', E);
	ps_set_event(pso, '=', E);
}

void set_ps_html_close_tag_name_out(ps* const cpso, const ps_event E)
{
	ps_set(cpso, '>', ps_html_close_tag_out, E);
	ps_set(cpso, '\n', ps_html_close_tag_name_out, E);
	ps_set(cpso, '\r', ps_html_close_tag_name_out, E);
	ps_set(cpso, '\t', ps_html_close_tag_name_out, E);
	ps_set(cpso, ' ', ps_html_close_tag_name_out, E);
	ps_set(cpso, '=', ps_html_close_tag_name_out, E);
}

void init_ps(ps* X, ps* Y, ps_event E)
{
	for (register unsigned char c = 0x00; ++c;)
	{
		X->next[c] = Y;
		X->event[c] = E;
	}
}

void init_ps_none(ps* X, ps_event E)
{
	for (register unsigned char c = 0x00; ++c;)
	{
		X->event[c] = E;
	}
}

void ps_set(ps* X, unsigned const char C, ps* Y, ps_event E)
{
	X->next[C] = Y;
	X->event[C] = E;
}

void ps_set_event(ps* X, unsigned const char C, ps_event E)
{
	X->event[C] = E;
}

void ps_set_ci(ps* X, unsigned const char C, ps* Y, ps_event E)
{
	ps_set(X, tolower(C), Y, E);
	ps_set(X, toupper(C), Y, E);
}

