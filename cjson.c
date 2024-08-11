#include "cjson.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
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

static int cjson_parse_number(cjson_context *c, cjson_value *v) {
#define ISDIGIT(ch) ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch) ((ch) >= '1' && (ch) <= '9')
  char *end;
  v->n = strtod(c->json, &end);
  if (c->json == end) {
    return CJSON_PARSE_INVALID_VALUE;
  }
  c->json = end;
  v->type = CJSON_NUMBER;
  return CJSON_PARSE_OK;
#undef ISDIGIT
#undef ISDIGIT1TO9
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
    return cjson_parse_number(c, v);
  }
}

cjson_parse_result cjson_parse(cjson_value *v, const char *json) {
  cjson_context c;
  assert(v != NULL);
  c.json = json;
  v->type = CJSON_NULL;
  cjson_parse_whitespace(&c);
  int ret = cjson_parse_value(&c, v);
  if (ret == CJSON_PARSE_OK) {
    if (*c.json != '\0') {
      ret = CJSON_PARSE_ROOT_NOT_SINGULAR;
      v->type = CJSON_NULL;
    }
  }
  return ret;
}

cjson_type cjson_get_type(const cjson_value *v) { return v->type; }

double cjson_get_number(const cjson_value *v) {
  assert(v != NULL && v->type == CJSON_NUMBER);
  return v->n;
}
