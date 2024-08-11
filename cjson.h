#ifndef __CJSON_H__
#define __CJSON_H__

// JSON-text = ws value ws
// ws = *(%x20 | %x09 | %x0A | %x0D)
// value = null | false | true  | number
// null  = "null"
// false = "false"
// true  = "true"
// number = [ "-" ] int [ frac ] [ exp ]
// int = "0" | digit1-9 *digit
// frac = "." 1*digit
// exp = ("e" | "E") ["-" | "+"] 1*digit

typedef enum{
  CJSON_NULL,
  CJSON_NUMBER,
  CJSON_FALSE,
  CJSON_TRUE,
  CJSON_STRUNG,
  CJSON_ARRAY,
  CJSON_OBJECT
}cjson_type;

typedef struct{
  double n;
  cjson_type type;
} cjson_value;

typedef enum {
  CJSON_PARSE_OK=0,
  CJSON_PARSE_EXPECT_VALUE,
  CJSON_PARSE_INVALID_VALUE,
  CJSON_PARSE_ROOT_NOT_SINGULAR,
}cjson_parse_result;

typedef struct {
  const char* json;
}cjson_context;

cjson_parse_result cjson_parse(cjson_value *v,const char* json);
cjson_type cjson_get_type(const cjson_value*);
double cjson_get_number(const cjson_value*);
#endif
