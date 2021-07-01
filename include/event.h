#define ps_event(func) \
func(EV_NONE) \
\
func(EV_HTML_STRING) \
func(EV_TAG_IN) \
func(EV_TAG_IN_REJECT) \
func(EV_TAG_IN_CLOSING) \
func(EV_TAG_OUT) \
func(EV_TAG_OUT_NAME_SAVE) \
\
func(EV_TAG_NAME_IN_FIRST) \
func(EV_TAG_NAME_IN) \
func(EV_TAG_NAME) \
func(EV_TAG_NAME_OUT) \
func(EV_TAG_NAME_OUT_CSS) \
func(EV_TAG_NAME_OUT_SCRIPT) \
func(EV_TAG_NAME_SAVE) \
\
func(EV_CLOSE_TAG_IN) \
func(EV_CLOSE_TAG_IN_SCRIPT) \
func(EV_CLOSE_TAG_IN_CSS) \
func(EV_CLOSE_TAG_OUT) \
func(EV_CLOSE_TAG_NAME_OUT) \
\
func(EV_TAG_ATTRIBUTE_NAME_IN) \
func(EV_TAG_ATTRIBUTE_NAME_ING) \
func(EV_TAG_ATTRIBUTE_NAME_OUT) \
func(EV_TAG_ATTRIBUTE_NAME_ERROR) \
\
func(EV_TAG_ATTRIBUTE_VALUE_IN) \
func(EV_TAG_ATTRIBUTE_VALUE_ING) \
func(EV_TAG_ATTRIBUTE_VALUE_OUT) \
func(EV_TAG_ATTRIBUTE_VALUE_ERROR) \
\
func(EV_SCRIPT_TAG_IN) \
func(EV_SCRIPT_TAG_OUT) \
func(EV_SCRIPT_STRING_LITERAL_IN) \
func(EV_SCRIPT_STRING_LITERAL_OUT) \
func(EV_SCRIPT_STRING_LITERAL_SINGLE_QUOTED) \
func(EV_SCRIPT_STRING_LITERAL_DOUBLE_QUOTED) \

#define gen_enum(enum) enum,
#define gen_string(str) #str,

enum ps_event {
	ps_event(gen_enum)
};

const char* ps_event_str[] = {
	ps_event(gen_string)
};
