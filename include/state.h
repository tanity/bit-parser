#define gen_global(vn) ps* vn;
#ifdef PS_NAME
#define gen_new(vn) vn = ps_new(); sprintf(vn->name, "%s", #vn);
#else
#define gen_new(vn) vn = ps_new();
#endif

#define ps_vars(func) \
func(ps_html_string) \
\
func(ps_html_tag_in) \
func(ps_html_tag_out) \
func(ps_html_tag_name) \
\
func(ps_html_tag_attribute_name) \
func(ps_html_tag_attribute_name_in) \
func(ps_html_tag_attribute_name_out) \
\
func(ps_html_tag_attribute_value_in) \
func(ps_html_tag_attribute_value_single_quoted_in) \
func(ps_html_tag_attribute_value_double_quoted_in) \
func(ps_html_tag_attribute_value_single_quoted) \
func(ps_html_tag_attribute_value_double_quoted) \
func(ps_html_tag_attribute_value_raw) \
\
func(ps_html_tag_exclamation_in) \
func(ps_html_tag_exclamation_name) \
func(ps_html_tag_exclamation_body) \
func(ps_html_tag_exclamation_single_quoted_in) \
func(ps_html_tag_exclamation_double_quoted_in) \
func(ps_html_tag_exclamation_single_quoted) \
func(ps_html_tag_exclamation_double_quoted) \
\
func(ps_html_close_tag_in) \
func(ps_html_close_tag_name) \
func(ps_html_close_tag_name_out) \
func(ps_html_close_tag_out) \
func(ps_html_comment_in) \
func(ps_html_comment_body) \
func(ps_html_comment_out1) \
func(ps_html_comment_out2) \
\
func(ps_script) \
func(ps_script_string_literal_single_quoted_in) \
func(ps_script_string_literal_single_quoted) \
func(ps_script_string_literal_single_quoted_backslash) \
func(ps_script_string_literal_double_quoted_in) \
func(ps_script_string_literal_double_quoted) \
func(ps_script_string_literal_double_quoted_backslash) \
func(ps_script_comment_in) \
func(ps_script_comment_single_line) \
func(ps_script_comment_single_line_backslash) \
func(ps_script_comment_multi_line) \
func(ps_script_comment_multi_line_out) \
func(ps_script_comment_out) \
func(ps_script_tag_in) \
func(ps_script_close_tag_in) \
func(ps_script_close_tag_name) \

ps_vars(gen_global);
