#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"

extern void Um_write_sequence(FILE *output, Seq_T instructions);

extern void emit_halt_test(Seq_T instructions);
extern void emit_verbose_halt_test(Seq_T instructions);
extern void emit_add_test(Seq_T instructions);
extern void emit_print_test(Seq_T instructions);
extern void emit_mult_test(Seq_T stream);
extern void emit_div_test(Seq_T stream);
extern void emit_nand_test(Seq_T stream);
extern void div_by_zero(Seq_T stream);
extern void test_map(Seq_T stream);
extern void test_unmap(Seq_T stream);
extern void test_seg_store(Seq_T stream);
extern void test_seg_load(Seq_T stream);
extern void test_cmov(Seq_T stream);
void test_load_p(Seq_T stream);
void test_50_mil(Seq_T stream);
void test_input(Seq_T stream);
void fail_input(Seq_T stream);

/* The array `tests` contains all unit tests for the lab. */

static struct test_info {
        const char *name;
        const char *test_input;          /* NULL means no input needed */
        const char *expected_output;
        /* writes instructions into sequence */
        void (*emit_test)(Seq_T stream);
} tests[] = {
        { "halt",         NULL, "", emit_halt_test },
        { "halt-verbose", NULL, "", emit_verbose_halt_test },
        { "add",          NULL, "d", emit_add_test},
        { "print",       NULL, "6", emit_print_test},
        { "mul",        NULL, "2", emit_mult_test},
        { "div",        NULL, "2", emit_div_test},
        { "nand",       NULL, "", emit_nand_test},
        { "divzero",     NULL, NULL, div_by_zero},
        { "map",        NULL, "F", test_map},
        { "umap",       NULL, "F", test_unmap},
        { "segstore",   NULL, "F", test_seg_store},
        { "segload",    NULL, "e", test_seg_load},
        { "cmov",       NULL, "ON", test_cmov},
        { "loadp",       NULL, "1", test_load_p},
        { "50mil",      NULL,   "A", test_50_mil},
        { "input",      "1",    "1", test_input},
        { "inputBad",   "300", "3", fail_input}
};


#define NTESTS (sizeof(tests)/sizeof(tests[0]))

/*
 * open file 'path' for writing, then free the pathname;
 * if anything fails, checked runtime error
 */
static FILE *open_and_free_pathname(char *path);

/*
 * if contents is NULL or empty, remove the given 'path',
 * otherwise write 'contents' into 'path'.  Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents);

static void write_test_files(struct test_info *test);


int main (int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
                for (unsigned i = 0; i < NTESTS; i++) {
                        printf("***** Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
                }
        else
                for (int j = 1; j < argc; j++) {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                                if (!strcmp(tests[i].name, argv[j])) {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        if (!tested) {
                                failed = true;
                                fprintf(stderr,
                                        "***** No test named %s *****\n",
                                        argv[j]);
                        }
                }
        return failed; /* failed nonzero == exit nonzero == failure */
}


static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(Fmt_string("%s.um", test->name));
        Seq_T instructions = Seq_new(0);
        test->emit_test(instructions);
        Um_write_sequence(binary, instructions);
        Seq_free(&instructions);
        fclose(binary);

        write_or_remove_file(Fmt_string("%s.0", test->name),
                             test->test_input);
        write_or_remove_file(Fmt_string("%s.1", test->name),
                             test->expected_output);
}


static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0') {
                remove(path);
        } else {
                FILE *input = fopen(path, "wb");
                assert(input != NULL);

                fputs(contents, input);
                fclose(input);
        }
        free(path);
}


static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);

        free(path);
        return fp;
}
