#include "cjson.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#define EXPECT(c, ch)                                                          \
  do {                                                                         \
    assert(*c->json == (ch));                                                  \
    c->json++;                                                                 \
  } while (0)

static void cjson_parse_whitespace(cjson_context *c) {
  const char *p = c->json;
  while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
    p++;
  }
  c->json = p;
}

static int check_word(const char *src, const char *dst, int length) {
  for (int i = 0; i < length; i++) {
    if (src[i] != dst[i]) {
      return CJSON_PARSE_INVALID_VALUE;
    }
  }
  return CJSON_PARSE_OK;
}

static int cjson_parse_keyword(cjson_context *c, cjson_value *v,
                               cjson_type ty) {
  int length = 0;
  cjson_parse_result res;
  switch (ty) {
  case CJSON_NULL:
    length = 4;
    res = check_word(c->json, "null", 4);
    break;
  case CJSON_NUMBER:
  case CJSON_FALSE:
    length = 5;
    res = check_word(c->json, "false", 5);
    break;
  case CJSON_TRUE:
    length = 4;
    res = check_word(c->json, "true", 4);
    break;
  case CJSON_STRUNG:
  case CJSON_ARRAY:
  case CJSON_OBJECT:
    break;
  }
  c->json += length;
  v->type = ty;
  return res;
}

static int cjson_parse_value(cjson_context *c, cjson_value *v) {
  switch (*c->json) {
  case 'n':
    return cjson_parse_keyword(c, v, CJSON_NULL);
  case 'f':
    return cjson_parse_keyword(c, v, CJSON_FALSE);
  case 't':
    return cjson_parse_keyword(c, v, CJSON_TRUE);
  case '\0':
    return CJSON_PARSE_EXPECT_VALUE;
  default:
    return CJSON_PARSE_INVALID_VALUE;
  }
}

cjson_parse_result cjson_parse(cjson_value *v, const char *json) {
  cjson_context c;
  assert(v != NULL);
  c.json = json;
  v->type = CJSON_TRUE;
  cjson_parse_whitespace(&c);
  return cjson_parse_value(&c, v);
}

cjson_type cjson_get_type(const cjson_value *v) { return v->type; }
