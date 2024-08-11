#include "cjson.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format)                       \
  do {                                                                         \
    test_count++;                                                              \
    if (equality)                                                              \
      test_pass++;                                                             \
    else {                                                                     \
      fprintf(stderr, "%s:%d: expecte: " format " actual:" format " \n",       \
              __FILE__, __LINE__, expect, actual);                             \
      main_ret = 1;                                                            \
    }                                                                          \
  } while (0)

#define EXPECT_EQ_INT(expect, actual)                                          \
  EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

#define TEST_ERROR(error, json)                                                \
  do {                                                                         \
    cjson_value v;                                                             \
    v.type = CJSON_FALSE;                                                      \
    EXPECT_EQ_INT(error, cjson_parse(&v, json));                               \
    EXPECT_EQ_INT(CJSON_NULL, cjson_get_type(&v));                             \
  } while (0)

static void test_parse_null() {
  cjson_value v;
  v.type = CJSON_TRUE;
  EXPECT_EQ_INT(CJSON_PARSE_OK, cjson_parse(&v, "null"));
  EXPECT_EQ_INT(CJSON_NULL, cjson_get_type(&v));
}
static void test_parse_true() {
  cjson_value v;
  v.type = CJSON_TRUE;
  EXPECT_EQ_INT(CJSON_PARSE_OK, cjson_parse(&v, "true"));
  EXPECT_EQ_INT(CJSON_TRUE, cjson_get_type(&v));
}
static void test_parse_false() {
  cjson_value v;
  v.type = CJSON_TRUE;
  EXPECT_EQ_INT(CJSON_PARSE_OK, cjson_parse(&v, "false"));
  EXPECT_EQ_INT(CJSON_FALSE, cjson_get_type(&v));
}

static void test_root_not_singular() {
  TEST_ERROR(CJSON_PARSE_ROOT_NOT_SINGULAR, "null x");
}

static void test_parse() {
  test_parse_null();
  test_parse_true();
  test_parse_false();
  test_root_not_singular();
}

int main() {
  test_parse();
  printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count,
         test_pass * 100.0 / test_count);
  return main_ret;
}
