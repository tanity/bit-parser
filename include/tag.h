struct parser_tag
{
	char* name;
	size_t length;
	bool is_single;
};

size_t tag_n = 0;
tag* tags;

size_t tag_new(const unsigned char* p, size_t length)
{
	tag* tags_tmp;

	if (tag_n % 32 == 0)
	{
		//tags_tmp = (tag*)realloc(tags, sizeof(tag) * ((int)tag_n / 32 + 1) * 32);
		tags_tmp = (tag*)malloc(sizeof(tag) * ((int)tag_n / 32 + 1) * 32);
		if (tag_n) memcpy(tags_tmp, tags, sizeof(tag) * ((int)tag_n / 32) * 32);
		tags = tags_tmp;
	}

	tags[tag_n].name = (char*)malloc(sizeof(char) * length + 1);
	tags[tag_n].length = length;

	memcpy(tags[tag_n].name, p, sizeof(char) * length);
	tags[tag_n].name[length] = '\0';

	return tag_n++;
}
