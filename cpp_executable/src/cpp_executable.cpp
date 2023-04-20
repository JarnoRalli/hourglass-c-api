#include <iostream>

#include <storyboard/storyboardCppAPI.hpp>

void print_note(const Note& note)
{
    std::cout << "Title: " << note.getTitle() << std::endl;
    std::cout << "Text: " << note.getText() << std::endl;
    std::cout << "Tags: ";
    for (auto tag : note.getTags())
    {
        std::cout << tag << ", ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    // Create two notes and a storyboard
    Note       note1 = Note("title1", "text", {"tag1", "tag2", "tag3"});
    Note       note2 = Note("title2", "text hei hello hola", {"tag1", "tag4", "tag5"});
    StoryBoard my_board;

    // Add the notes to the storyboard
    my_board.addNote(note1);
    my_board.addNote(note2);

    note_cont_t query_result;

    // Query the board based on the title
    std::cout << "QUERY A BOARD BY TITLE -> title1" << std::endl;
    my_board.searchByTitle("title1", query_result);
    for (auto note : query_result)
    {
        print_note(note);
    }
    query_result.clear();
    std::cout << "-------------" << std::endl;

    // Query the board based on the text
    std::cout << "QUERY A BOARD BY TEXT -> text" << std::endl;
    my_board.searchByText("text", query_result);
    for (auto note : query_result)
    {
        print_note(note);
    }
    query_result.clear();
    std::cout << "-------------" << std::endl;

    // Query the board based on the text
    std::cout << "QUERY A BOARD BY TEXT -> hola" << std::endl;
    my_board.searchByText("hola", query_result);
    for (auto note : query_result)
    {
        print_note(note);
    }
    query_result.clear();
    std::cout << "-------------" << std::endl;

    // Query the board based on the tags
    std::cout << "QUERY A BOARD BY TAG -> tag1" << std::endl;
    my_board.searchByTag("tag1", query_result);
    for (auto note : query_result)
    {
        print_note(note);
    }
    query_result.clear();
    std::cout << "-------------" << std::endl;

    // Query the board based on the tags
    std::cout << "QUERY A BOARD BY TAG -> tag4" << std::endl;
    my_board.searchByTag("tag4", query_result);
    for (auto note : query_result)
    {
        print_note(note);
    }
    query_result.clear();
    std::cout << "-------------" << std::endl;

    return 0;
}
