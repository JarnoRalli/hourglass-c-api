#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <storyboard/storyboardCAPI.h>

#define NR_TAGS 3
#define MAX_NOTES 10

void assert_error(error_t_ error)
{
    if (error != NULL)
    {
        printf("Error: %s\n", error_get_string(error));
        error_destruct(error);
        exit(-1);
    }
};

// A function that prints note information to the output stream
void print_notes(void* client_data, const char* title, const char* text, const char* tags[], int32_t nr_tags)
{
    printf("Title: %s\n", title);
    printf("Text: %s\n", text);
    printf("Tags: ");

    for (int32_t index = 0; index < nr_tags; index++)
    {
        printf("%s", tags[index]);
        if (index < nr_tags - 1)
        {
            printf(", ");
        }
    }
    printf("\n");
    printf("-------------\n");
};

// A function that is called for each tag
void query_tags(void* client_data, const char* tag)
{
    printf("Tag: %s\n", tag);
}

int main(int argc, char* argv[])
{
    error_t_    my_error       = NULL;
    const char* tags1[NR_TAGS] = {"t1", "t2", "t3"};
    const char* tags2[NR_TAGS] = {"t1", "t4", "t5"};

    // Create two notes
    note_t my_note1 = note_construct("title1", "text", tags1, NR_TAGS, &my_error);
    assert_error(my_error);

    note_t my_note2 = note_construct("title2", "text hei hello hola", tags2, NR_TAGS, &my_error);
    assert_error(my_error);

    // Query my_note2 for tags
    printf("QUERY MY_NOTE2 FOR TAGS\n");
    note_get_tags(my_note2, query_tags, NULL, &my_error);
    assert_error(my_error);
    printf("-------------\n");

    // Create a board where to store the notes
    board_t my_board = storyboard_construct(&my_error);
    assert_error(my_error);

    // Add the notes to the board
    storyboard_add_note(my_board, my_note1, &my_error);
    assert_error(my_error);

    storyboard_add_note(my_board, my_note2, &my_error);
    assert_error(my_error);

    // Query the board based on the title
    printf("QUERY A BOARD BY TITLE -> title1\n");
    storyboard_search_by_title(my_board, "title1", &print_notes, NULL, &my_error);
    assert_error(my_error);

    // Query the board based on the text
    printf("\nQUERY A BOARD BY TEXT -> text\n");
    storyboard_search_by_text(my_board, "text", &print_notes, NULL, &my_error);
    assert_error(my_error);

    // Query the board based on the text
    printf("\nQUERY A BOARD BY TEXT -> hola\n");
    storyboard_search_by_text(my_board, "hola", &print_notes, NULL, &my_error);
    assert_error(my_error);

    // Query the board based on the tags
    printf("\nQUERY A BOARD BY TAG -> t1\n");
    storyboard_search_by_tag(my_board, "t1", &print_notes, NULL, &my_error);
    assert_error(my_error);

    // Query the board based on the tags
    printf("\nQUERY A BOARD BY TAG -> t4\n");
    storyboard_search_by_tag(my_board, "t4", &print_notes, NULL, &my_error);
    assert_error(my_error);

    // Release memory
    note_destruct(my_note1);
    note_destruct(my_note2);
    storyboard_destruct(my_board);

    return 0;
}
