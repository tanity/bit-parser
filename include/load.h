void load()
{
	load_ps();
	load_ps_html();
}

void load_ps()
{
	ps_vars(gen_new);

	// AND Data
	init_ps(ps_html_string, ps_html_string, EV_NONE);
	ps_set(ps_html_string, '<', ps_html_tag_in, EV_TAG_IN);

	init_ps(ps_html_tag_in, ps_html_string, EV_TAG_IN_REJECT);

	// define first tag name filter
	for (unsigned char c = 0x00; ++c;)
	{
		switch (get_char_type(c))
		{
		case CHAR_ASCII_ALPHABET_UPPERCASE:
		case CHAR_ASCII_ALPHABET_LOWERCASE:
		case CHAR_CYRILLIC_ONLY:
		case CHAR_UTF8_DATA_ONLY:
		case CHAR_UTF8_DATA_CYRILLIC:
		case CHAR_UTF8_2BYTE_ONLY:
		case CHAR_UTF8_2BYTE_CYRILLIC:
		case CHAR_UTF8_3BYTE_ONLY:
		case CHAR_UTF8_3BYTE_CYRILLIC:
		case CHAR_UTF8_4BYTE_ONLY:
			ps_set(ps_html_tag_in, c, NULL, EV_TAG_NAME_IN_FIRST);
			break;
		default:
			switch (c)
			{
			case '_':
			case '.':
			case ':':
				ps_set(ps_html_tag_in, c, NULL, EV_TAG_NAME_IN_FIRST);
				break;
			case '?':
				ps_set(ps_html_tag_in, c, NULL, EV_TAG_NAME_IN_FIRST);
				break;// XML DOCUMENT DETECTION
			case '!':
				ps_set(ps_html_tag_in, c, ps_html_tag_exclamation_in, EV_NONE); // EXCLAMATION MARKS
				break;
			case '/':
				ps_set(ps_html_tag_in, c, ps_html_close_tag_in, EV_CLOSE_TAG_IN); // CLOSE TAG DETECTED
				break;
			}
			break;
		}
	}

	init_ps(ps_html_tag_out, ps_html_string, EV_TAG_OUT); //EV_HTML_STRING
	ps_set(ps_html_tag_out, '<', ps_html_tag_in, EV_TAG_IN);


	init_ps(ps_html_close_tag_in, ps_html_close_tag_name_out, EV_NONE);
	ps_set(ps_html_close_tag_in, '>', ps_html_close_tag_out, EV_CLOSE_TAG_OUT);

	init_ps(ps_html_close_tag_name, ps_html_close_tag_name_out, EV_NONE); // undefined name, no action...
	ps_set(ps_html_close_tag_name, '>', ps_html_close_tag_out, EV_CLOSE_TAG_OUT);
	set_ps_html_tag_name_out_event(ps_html_close_tag_name, EV_CLOSE_TAG_NAME_OUT);

	init_ps(ps_html_close_tag_name_out, ps_html_close_tag_name_out, EV_NONE);
	ps_set(ps_html_close_tag_name_out, '>', ps_html_close_tag_out, EV_CLOSE_TAG_OUT);

	init_ps(ps_html_close_tag_out, ps_html_string, EV_HTML_STRING);
	ps_set(ps_html_close_tag_out, '<', ps_html_tag_in, EV_TAG_IN);


	// XML TAG NAME REGION
	// TAG NAME, ATTRIBUTE NAME, ATTRIBUTE VALUE
	init_ps(ps_html_tag_name, NULL, EV_TAG_NAME);
	for (unsigned char c = 0x00; ++c;)
	{
		switch (get_char_type(c))
		{
		case CHAR_ASCII_NUMERIC:
		case CHAR_ASCII_ALPHABET_UPPERCASE:
		case CHAR_ASCII_ALPHABET_LOWERCASE:
		case CHAR_CYRILLIC_ONLY:
		case CHAR_UTF8_DATA_ONLY:
		case CHAR_UTF8_DATA_CYRILLIC:
		case CHAR_UTF8_2BYTE_ONLY:
		case CHAR_UTF8_2BYTE_CYRILLIC:
		case CHAR_UTF8_3BYTE_ONLY:
		case CHAR_UTF8_3BYTE_CYRILLIC:
		case CHAR_UTF8_4BYTE_ONLY:
			ps_set(ps_html_tag_name, c, NULL, EV_TAG_NAME);
			break;
		default:
			switch (c)
			{
			case '_':
			case '-':
			case '+':
			case '.':
			case ':':
				ps_set(ps_html_tag_name, c, NULL, EV_TAG_NAME);
				break;
			case '>':
				ps_set(ps_html_tag_name, c, ps_html_tag_out, EV_TAG_OUT_NAME_SAVE);
				break;
			case '\n':
			case '\r':
			case '\t':
			case ' ':
				ps_set(ps_html_tag_name, c, ps_html_tag_attribute_name_in, EV_TAG_NAME_SAVE);
				break;
			case '=':
				ps_set(ps_html_tag_name, c, ps_html_tag_attribute_value_in, EV_TAG_NAME_SAVE);
				break;
			}
			break;
		}

	}

	set_ps_html_tag_name_out_event(ps_html_tag_name, EV_TAG_NAME_SAVE);

	// XML ELEMENT REGION
	// TAG NAME, ATTRIBUTE NAME, ATTRIBUTE VALUE

	init_ps(ps_html_tag_attribute_name_in, ps_html_tag_attribute_name, EV_TAG_ATTRIBUTE_NAME_IN);
	ps_set(ps_html_tag_attribute_name_in, '>', ps_html_tag_out, EV_TAG_OUT);
	ps_set(ps_html_tag_attribute_name_in, ' ', ps_html_tag_attribute_name_in, EV_NONE);
	ps_set(ps_html_tag_attribute_name_in, '\r', ps_html_tag_attribute_name_in, EV_NONE);
	ps_set(ps_html_tag_attribute_name_in, '\n', ps_html_tag_attribute_name_in, EV_NONE);
	ps_set(ps_html_tag_attribute_name_in, '\t', ps_html_tag_attribute_name_in, EV_NONE);

	init_ps(ps_html_tag_attribute_name, ps_html_tag_attribute_name, EV_TAG_ATTRIBUTE_NAME_ING);
	ps_set(ps_html_tag_attribute_name, '>', ps_html_tag_out, EV_TAG_ATTRIBUTE_NAME_OUT);
	ps_set(ps_html_tag_attribute_name, '=', ps_html_tag_attribute_value_in, EV_TAG_ATTRIBUTE_NAME_OUT);
	ps_set(ps_html_tag_attribute_name, ' ', ps_html_tag_attribute_name_out, EV_TAG_ATTRIBUTE_NAME_OUT);
	ps_set(ps_html_tag_attribute_name, '\r', ps_html_tag_attribute_name_out, EV_TAG_ATTRIBUTE_NAME_OUT);
	ps_set(ps_html_tag_attribute_name, '\n', ps_html_tag_attribute_name_out, EV_TAG_ATTRIBUTE_NAME_OUT);
	ps_set(ps_html_tag_attribute_name, '\t', ps_html_tag_attribute_name_out, EV_TAG_ATTRIBUTE_NAME_OUT);

	init_ps(ps_html_tag_attribute_name_out, ps_html_tag_attribute_name, EV_TAG_ATTRIBUTE_NAME_IN);
	ps_set(ps_html_tag_attribute_name_out, '>', ps_html_tag_out, EV_NONE);
	ps_set(ps_html_tag_attribute_name_out, '=', ps_html_tag_attribute_value_in, EV_NONE);
	ps_set(ps_html_tag_attribute_name_out, ' ', ps_html_tag_attribute_name_out, EV_NONE);
	ps_set(ps_html_tag_attribute_name_out, '\r', ps_html_tag_attribute_name_out, EV_NONE);
	ps_set(ps_html_tag_attribute_name_out, '\n', ps_html_tag_attribute_name_out, EV_NONE);
	ps_set(ps_html_tag_attribute_name_out, '\t', ps_html_tag_attribute_name_out, EV_NONE);

	init_ps(ps_html_tag_attribute_value_in, ps_html_tag_attribute_value_raw, EV_TAG_ATTRIBUTE_VALUE_IN);
	ps_set(ps_html_tag_attribute_value_in, '>', ps_html_tag_out, EV_NONE);
	ps_set(ps_html_tag_attribute_value_in, '\'', ps_html_tag_attribute_value_single_quoted_in, EV_NONE);
	ps_set(ps_html_tag_attribute_value_in, '"', ps_html_tag_attribute_value_double_quoted_in, EV_NONE);
	ps_set(ps_html_tag_attribute_value_in, ' ', ps_html_tag_attribute_value_in, EV_NONE);
	ps_set(ps_html_tag_attribute_value_in, '\r', ps_html_tag_attribute_value_in, EV_NONE);
	ps_set(ps_html_tag_attribute_value_in, '\n', ps_html_tag_attribute_value_in, EV_NONE);
	ps_set(ps_html_tag_attribute_value_in, '\t', ps_html_tag_attribute_value_in, EV_NONE);


	init_ps(ps_html_tag_attribute_value_single_quoted_in, ps_html_tag_attribute_value_single_quoted, EV_TAG_ATTRIBUTE_VALUE_IN);
	ps_set(ps_html_tag_attribute_value_single_quoted, '\'', ps_html_tag_attribute_name_in, EV_TAG_ATTRIBUTE_VALUE_OUT);

	init_ps(ps_html_tag_attribute_value_single_quoted, ps_html_tag_attribute_value_single_quoted, EV_TAG_ATTRIBUTE_VALUE_ING);
	ps_set(ps_html_tag_attribute_value_single_quoted, '\'', ps_html_tag_attribute_name_in, EV_TAG_ATTRIBUTE_VALUE_OUT);

	init_ps(ps_html_tag_attribute_value_double_quoted_in, ps_html_tag_attribute_value_double_quoted, EV_TAG_ATTRIBUTE_VALUE_IN);
	ps_set(ps_html_tag_attribute_value_double_quoted, '"', ps_html_tag_attribute_name_in, EV_TAG_ATTRIBUTE_VALUE_OUT);

	init_ps(ps_html_tag_attribute_value_double_quoted, ps_html_tag_attribute_value_double_quoted, EV_TAG_ATTRIBUTE_VALUE_ING);
	ps_set(ps_html_tag_attribute_value_double_quoted, '"', ps_html_tag_attribute_name_in, EV_TAG_ATTRIBUTE_VALUE_OUT);

	init_ps(ps_html_tag_attribute_value_raw, ps_html_tag_attribute_value_raw, EV_TAG_ATTRIBUTE_VALUE_ING);
	ps_set(ps_html_tag_attribute_value_raw, '>', ps_html_tag_out, EV_TAG_ATTRIBUTE_VALUE_OUT);
	ps_set(ps_html_tag_attribute_value_raw, '"', ps_html_tag_attribute_name_in, EV_TAG_ATTRIBUTE_VALUE_OUT);
	ps_set(ps_html_tag_attribute_value_raw, ' ', ps_html_tag_attribute_name_in, EV_TAG_ATTRIBUTE_VALUE_OUT);
	ps_set(ps_html_tag_attribute_value_raw, '\r', ps_html_tag_attribute_name_in, EV_TAG_ATTRIBUTE_VALUE_OUT);
	ps_set(ps_html_tag_attribute_value_raw, '\n', ps_html_tag_attribute_name_in, EV_TAG_ATTRIBUTE_VALUE_OUT);
	ps_set(ps_html_tag_attribute_value_raw, '\t', ps_html_tag_attribute_name_in, EV_TAG_ATTRIBUTE_VALUE_OUT);




	// XML ELEMENT EXCLAMATION MARK REGION
	// - DOCTYPE, CDATA, COMMENT
	init_ps(ps_html_tag_exclamation_in, ps_html_tag_exclamation_name, EV_NONE);
	ps_set(ps_html_tag_exclamation_in, '-', ps_html_comment_in, EV_NONE);

	init_ps(ps_html_tag_exclamation_name, ps_html_tag_exclamation_name, EV_NONE);
	ps_set(ps_html_tag_exclamation_name, '>', ps_html_tag_out, EV_NONE);
	ps_set(ps_html_tag_exclamation_name, ' ', ps_html_tag_exclamation_body, EV_NONE);

	init_ps(ps_html_tag_exclamation_body, ps_html_tag_exclamation_body, EV_NONE);
	ps_set(ps_html_tag_exclamation_body, '>', ps_html_tag_out, EV_NONE);
	ps_set(ps_html_tag_exclamation_body, '\'', ps_html_tag_exclamation_single_quoted, EV_NONE);
	ps_set(ps_html_tag_exclamation_body, '"', ps_html_tag_exclamation_double_quoted, EV_NONE);



	init_ps(ps_html_tag_exclamation_single_quoted_in, ps_html_tag_exclamation_single_quoted, EV_NONE);
	ps_set(ps_html_tag_exclamation_single_quoted_in, '\'', ps_html_tag_exclamation_body, EV_NONE);

	init_ps(ps_html_tag_exclamation_single_quoted, ps_html_tag_exclamation_single_quoted, EV_NONE);
	ps_set(ps_html_tag_exclamation_single_quoted, '\'', ps_html_tag_exclamation_body, EV_NONE);


	init_ps(ps_html_tag_exclamation_double_quoted_in, ps_html_tag_exclamation_double_quoted, EV_NONE);
	ps_set(ps_html_tag_exclamation_double_quoted_in, '"', ps_html_tag_exclamation_body, EV_NONE);

	init_ps(ps_html_tag_exclamation_double_quoted, ps_html_tag_exclamation_double_quoted, EV_NONE);
	ps_set(ps_html_tag_exclamation_double_quoted, '"', ps_html_tag_exclamation_body, EV_NONE);

	// COMMENT
	init_ps(ps_html_comment_in, ps_html_tag_exclamation_name, EV_NONE);
	ps_set(ps_html_comment_in, '-', ps_html_comment_body, EV_NONE);

	init_ps(ps_html_comment_body, ps_html_comment_body, EV_NONE);
	ps_set(ps_html_comment_body, '-', ps_html_comment_out1, EV_NONE);

	init_ps(ps_html_comment_out1, ps_html_comment_body, EV_NONE);
	ps_set(ps_html_comment_out1, '-', ps_html_comment_out2, EV_NONE);

	init_ps(ps_html_comment_out2, ps_html_comment_body, EV_NONE);
	ps_set(ps_html_comment_out2, '>', ps_html_tag_out, EV_NONE);




	//////////////////////////////
	// SCRIPT REGION
	////

	init_ps(ps_script, ps_script, EV_NONE);
	ps_set(ps_script, '<', ps_script_tag_in, EV_CLOSE_TAG_IN_SCRIPT);
	ps_set(ps_script, '\'', ps_script_string_literal_single_quoted_in, EV_SCRIPT_STRING_LITERAL_IN);
	ps_set(ps_script, '"', ps_script_string_literal_double_quoted_in, EV_SCRIPT_STRING_LITERAL_IN);
	ps_set(ps_script, '/', ps_script_comment_in, EV_NONE);


	// SCRIPT SINGLE LITERAL
	init_ps(ps_script_string_literal_single_quoted_in, ps_script_string_literal_single_quoted, EV_NONE);
	ps_set(ps_script_string_literal_single_quoted_in, '\'', ps_script, EV_SCRIPT_STRING_LITERAL_OUT);
	ps_set(ps_script_string_literal_single_quoted_in, '\\', ps_script_string_literal_single_quoted_backslash, EV_NONE);

	init_ps(ps_script_string_literal_single_quoted, ps_script_string_literal_single_quoted, EV_NONE);
	ps_set(ps_script_string_literal_single_quoted, '\'', ps_script, EV_SCRIPT_STRING_LITERAL_OUT);
	ps_set(ps_script_string_literal_single_quoted, '\\', ps_script_string_literal_single_quoted_backslash, EV_NONE);

	init_ps(ps_script_string_literal_single_quoted_backslash, ps_script_string_literal_single_quoted, EV_NONE);


	// SCRIPT DOUBLE LITERAL
	init_ps(ps_script_string_literal_double_quoted_in, ps_script_string_literal_double_quoted, EV_NONE);
	ps_set(ps_script_string_literal_double_quoted_in, '"', ps_script, EV_SCRIPT_STRING_LITERAL_OUT);
	ps_set(ps_script_string_literal_double_quoted_in, '\\', ps_script_string_literal_double_quoted_backslash, EV_NONE);

	init_ps(ps_script_string_literal_double_quoted, ps_script_string_literal_double_quoted, EV_NONE);
	ps_set(ps_script_string_literal_double_quoted, '"', ps_script, EV_SCRIPT_STRING_LITERAL_OUT);
	ps_set(ps_script_string_literal_double_quoted, '\\', ps_script_string_literal_double_quoted_backslash, EV_NONE);

	init_ps(ps_script_string_literal_double_quoted_backslash, ps_script_string_literal_double_quoted, EV_NONE);

	init_ps(ps_script_comment_in, ps_script, EV_NONE);
	ps_set(ps_script_comment_in, '<', ps_script_tag_in, EV_SCRIPT_TAG_IN);
	ps_set(ps_script_comment_in, '/', ps_script_comment_single_line, EV_NONE);
	ps_set(ps_script_comment_in, '*', ps_script_comment_multi_line, EV_NONE);

	init_ps(ps_script_comment_single_line, ps_script_comment_single_line, EV_NONE);
	ps_set(ps_script_comment_single_line, '<', ps_script_tag_in, EV_SCRIPT_TAG_IN);
	ps_set(ps_script_comment_single_line, '\\', ps_script_comment_single_line_backslash, EV_NONE);
	ps_set(ps_script_comment_single_line, '\n', ps_script_comment_out, EV_NONE);

	init_ps(ps_script_comment_single_line_backslash, ps_script_comment_single_line, EV_NONE);
	ps_set(ps_script_comment_single_line_backslash, '<', ps_script_tag_in, EV_SCRIPT_TAG_IN);

	init_ps(ps_script_comment_multi_line, ps_script_comment_multi_line, EV_NONE);
	ps_set(ps_script_comment_multi_line, '<', ps_script_tag_in, EV_SCRIPT_TAG_IN);
	ps_set(ps_script_comment_multi_line, '*', ps_script_comment_multi_line_out, EV_NONE);

	init_ps(ps_script_comment_multi_line_out, ps_script_comment_multi_line, EV_NONE);
	ps_set(ps_script_comment_multi_line, '<', ps_script_tag_in, EV_SCRIPT_TAG_IN);
	ps_set(ps_script_comment_multi_line, '/', ps_script_comment_out, EV_NONE);

	init_ps(ps_script_comment_out, ps_script, EV_NONE);
	ps_set(ps_script_comment_out, '<', ps_script_tag_in, EV_SCRIPT_TAG_IN);

	init_ps(ps_script_tag_in, NULL, EV_SCRIPT_TAG_OUT);
	ps_set(ps_script_tag_in, '/', ps_script_close_tag_in, EV_NONE);
	init_ps(ps_script_close_tag_in, NULL, EV_SCRIPT_TAG_OUT);
	init_ps(ps_script_close_tag_name, NULL, EV_SCRIPT_TAG_OUT);
	load_ps_html_tag_script();

	// STYLESHEET REGION

}

void load_ps_html()
{
	//Reference : https://developer.mozilla.org/en-US/docs/Web/HTML/Element

	// Main root
	load_ps_html_tag("html", 0);

	// Document metadata
	load_ps_html_tag("base", 1);
	load_ps_html_tag("head", 0);
	load_ps_html_tag("link", 1);
	load_ps_html_tag("meta", 1);
	load_ps_html_tag("style", 0);
	load_ps_html_tag("title", 0);

	// Sectioning root
	load_ps_html_tag("body", 0);

	//Content sectioning
	load_ps_html_tag("address", 0);
	load_ps_html_tag("article", 0);
	load_ps_html_tag("aside", 0);
	load_ps_html_tag("footer", 0);
	load_ps_html_tag("header", 0);
	load_ps_html_tag("h1", 0);
	load_ps_html_tag("h2", 0);
	load_ps_html_tag("h3", 0);
	load_ps_html_tag("h4", 0);
	load_ps_html_tag("h5", 0);
	load_ps_html_tag("h6", 0);
	load_ps_html_tag("hgroup", 0);
	load_ps_html_tag("main", 0);
	load_ps_html_tag("nav", 0);
	load_ps_html_tag("section", 0);

	// Text content
	load_ps_html_tag("blockquote", 0);
	load_ps_html_tag("dd", 0);
	load_ps_html_tag("div", 0);
	load_ps_html_tag("dl", 0);
	load_ps_html_tag("dt", 0);
	load_ps_html_tag("figcaption", 0);
	load_ps_html_tag("figure", 0);
	load_ps_html_tag("hr", 1);
	load_ps_html_tag("li", 0);
	load_ps_html_tag("ol", 0);
	load_ps_html_tag("p", 0);
	load_ps_html_tag("pre", 0);
	load_ps_html_tag("ul", 0);

	// Inline text semantics
	load_ps_html_tag("a", 0);
	load_ps_html_tag("abbr", 0);
	load_ps_html_tag("b", 0);
	load_ps_html_tag("bdi", 0);
	load_ps_html_tag("bdo", 0);
	load_ps_html_tag("br", 1);
	load_ps_html_tag("cite", 0);
	load_ps_html_tag("code", 0);
	load_ps_html_tag("data", 0);
	load_ps_html_tag("dfn", 0);
	load_ps_html_tag("em", 0);
	load_ps_html_tag("i", 0);
	load_ps_html_tag("kbd", 0);
	load_ps_html_tag("mark", 0);
	load_ps_html_tag("q", 0);
	load_ps_html_tag("rb", 0);
	load_ps_html_tag("rp", 0);
	load_ps_html_tag("rt", 0);
	load_ps_html_tag("rtc", 0);
	load_ps_html_tag("ruby", 0);
	load_ps_html_tag("s", 0);
	load_ps_html_tag("samp", 0);
	load_ps_html_tag("small", 0);
	load_ps_html_tag("span", 0);
	load_ps_html_tag("strong", 0);
	load_ps_html_tag("sub", 0);
	load_ps_html_tag("sup", 0);
	load_ps_html_tag("time", 0);
	load_ps_html_tag("u", 0);
	load_ps_html_tag("var", 0);
	load_ps_html_tag("wbr", 1);

	// Image and multimedia
	load_ps_html_tag("area", 1);
	load_ps_html_tag("audio", 0);
	load_ps_html_tag("img", 1);
	load_ps_html_tag("map", 0);
	load_ps_html_tag("track", 1);
	load_ps_html_tag("video", 0);

	// Embedded content
	load_ps_html_tag("embed", 1);
	load_ps_html_tag("iframe", 0);
	load_ps_html_tag("object", 0);
	load_ps_html_tag("param", 1);
	load_ps_html_tag("picture", 0);
	load_ps_html_tag("source", 1);

	// Scripting
	load_ps_html_tag("canvas", 0);
	load_ps_html_tag("noscript", 0);
	//load_ps_html_tag("script", 0);

	// Demarcating edits
	load_ps_html_tag("del", 0);
	load_ps_html_tag("ins", 0);

	// Table content
	load_ps_html_tag("caption", 0);
	load_ps_html_tag("col", 1);
	load_ps_html_tag("colgroup", 0);
	load_ps_html_tag("table", 0);
	load_ps_html_tag("tbody", 0);
	load_ps_html_tag("td", 0);
	load_ps_html_tag("tfoot", 0);
	load_ps_html_tag("th", 0);
	load_ps_html_tag("thead", 0);
	load_ps_html_tag("tr", 0);

	// Forms
	load_ps_html_tag("button", 0);
	load_ps_html_tag("datalist", 0);
	load_ps_html_tag("fieldset", 0);
	load_ps_html_tag("form", 0);
	load_ps_html_tag("input", 1);
	load_ps_html_tag("label", 0);
	load_ps_html_tag("legend", 0);
	load_ps_html_tag("meter", 0);
	load_ps_html_tag("optgroup", 0);
	load_ps_html_tag("option", 0);
	load_ps_html_tag("output", 0);
	load_ps_html_tag("progress", 0);
	load_ps_html_tag("select", 0);
	load_ps_html_tag("textarea", 0);

	// Interactive elements
	load_ps_html_tag("details", 0);
	load_ps_html_tag("dialog", 0);
	load_ps_html_tag("menu", 0);
	load_ps_html_tag("summary", 0);

	// Web Components
	load_ps_html_tag("slot", 0);
	load_ps_html_tag("template", 0);

	// Obsolete and deprecated elements
	load_ps_html_tag("acronym", 0);
	load_ps_html_tag("applet", 0);
	load_ps_html_tag("basefont", 0);
	load_ps_html_tag("bgsound", 0);
	load_ps_html_tag("big", 0);
	load_ps_html_tag("blink", 0);
	load_ps_html_tag("center", 0);
	load_ps_html_tag("command", 0);
	load_ps_html_tag("content", 0);
	load_ps_html_tag("dir", 0);
	load_ps_html_tag("element", 0);
	load_ps_html_tag("font", 0);
	load_ps_html_tag("frame", 0);
	load_ps_html_tag("frameset", 0);
	load_ps_html_tag("image", 0);
	load_ps_html_tag("isindex", 0);
	load_ps_html_tag("keygen", 1);
	load_ps_html_tag("listing", 0);
	load_ps_html_tag("marquee", 0);
	load_ps_html_tag("menuitem", 0);
	load_ps_html_tag("multicol", 0);
	load_ps_html_tag("nextid", 0);
	load_ps_html_tag("nobr", 0);
	load_ps_html_tag("noembed", 0);
	load_ps_html_tag("noframes", 0);
	load_ps_html_tag("plaintext", 0);
	load_ps_html_tag("shadow", 0);
	load_ps_html_tag("spacer", 0);
	load_ps_html_tag("strike", 0);
	load_ps_html_tag("tt", 0);
	load_ps_html_tag("xmp", 0);
}

void load_ps_html_tag(const unsigned char* const tag, bool is_single)
{
	ps* pso;
	ps* cpso;

	size_t l = strlen((const char*)tag), p = 0;

	if (l > 0);
	else return;


	pso = ps_html_tag_in;
	cpso = ps_html_close_tag_in;

	for (p = 0; p < l; p++)
	{
		// open tag
		pso = (pso->next[tag[p]] == NULL) ? push_ps_html_tag_name(pso, ps_html_tag_name, tag[p], !p ? EV_TAG_NAME_IN : EV_NONE) : pso->next[tag[p]];

		// close tag
		cpso = (cpso->next[tag[p]] == ps_html_close_tag_name_out) ? push_ps_html_tag_name(cpso, ps_html_close_tag_name, tag[p], EV_NONE) : cpso->next[tag[p]];
	}

	pso->tag_id = tag_new(tag, l);
	cpso->tag_id = pso->tag_id;
	tags[pso->tag_id].is_single = is_single;
	set_ps_html_tag_name_out_event(pso, EV_TAG_NAME_OUT);
	set_ps_html_tag_name_out_event(cpso, EV_CLOSE_TAG_NAME_OUT);
}

void load_ps_html_tag_script()
{
	const unsigned char* tag = "script";
	bool is_single = false;

	ps* pso;
	ps* cpso;

	size_t l = strlen((const char*)tag), p = 0;

	if (l > 0);
	else return;

	pso = ps_html_tag_in;
	cpso = ps_script_close_tag_in;

	for (p = 0; p < l; p++)
	{
		// open tag
		pso = (pso->next[tag[p]] == NULL) ? push_ps_html_tag_name(pso, ps_html_tag_name, tag[p], !p ? EV_TAG_NAME_IN : EV_NONE) : pso->next[tag[p]];

		// close tag
		cpso = (cpso->next[tag[p]] == NULL) ? push_ps_html_tag_name(cpso, ps_script_close_tag_name, tag[p], EV_NONE) : cpso->next[tag[p]];
	}

	pso->tag_id = tag_new(tag, l);
	cpso->tag_id = pso->tag_id;
	tags[pso->tag_id].is_single = is_single;
	set_ps_html_tag_name_out_event(pso, EV_TAG_NAME_OUT_SCRIPT);
	set_ps_html_close_tag_name_out(cpso, EV_CLOSE_TAG_NAME_OUT);
}
