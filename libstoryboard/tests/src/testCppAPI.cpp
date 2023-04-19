#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "storyboard/storyboardCppAPI.hpp"

class QueryTestCppAPI : public ::testing::Test
{
protected:
    void SetUp() override
    {
        EXPECT_NO_THROW(note1 = Note("title1", "text", {"tag1", "tag2", "tag3"}));
        EXPECT_NO_THROW(note2 = Note("title2", "text hei hello hola", {"tag1", "tag4", "tag5"}));

        EXPECT_NO_THROW(my_board.addNote(note1));
        EXPECT_NO_THROW(my_board.addNote(note2));
    }

    void TearDown() override
    {
    }

    Note       note1;
    Note       note2;
    StoryBoard my_board;
};

TEST(CppAPI, construct_and_destruct_note)
{
    EXPECT_NO_THROW(Note my_note("note1", "text", {"tag1", "tag2"}));
    EXPECT_NO_THROW(Note my_note2 = Note("note2", "text", {"tag1", "tag2"}));

    Note my_note3;
    EXPECT_NO_THROW(my_note3 = Note("note3", "text", {"tag1", "tag2"}));
}

TEST(CppAPI, exceptions_note)
{
    Note        my_note = Note();
    std::string temp;

    tag_cont_t query_result;

    EXPECT_THROW(query_result = my_note.getTags(), std::runtime_error);
    EXPECT_THROW(temp = my_note.getTitle(), std::runtime_error);
    EXPECT_THROW(temp = my_note.getText(), std::runtime_error);
}

TEST(CppAPI, construct_and_destruct_board)
{
    Note my_note("note1", "text", {"tag1", "tag2"});

    StoryBoard my_board1;
    StoryBoard my_board2;

    EXPECT_NO_THROW(StoryBoard my_board);
    EXPECT_NO_THROW(my_board1.addNote(my_note));
    EXPECT_NO_THROW(my_board2 = StoryBoard(my_board1));
}

TEST(CppAPI, board_add_note)
{
    Note my_note1("note1", "text", {"tag1", "tag2"});
    Note my_note2("note2", "text", {"tag3", "tag4"});

    StoryBoard my_board;
    EXPECT_NO_THROW(my_board.addNote(my_note1));
    EXPECT_NO_THROW(my_board.addNote(my_note2));
    EXPECT_EQ(my_board.length(), 2);
}

TEST(CppAPI, board_delete_note)
{
    Note my_note1("note1", "text", {"tag1", "tag2"});
    Note my_note2("note2", "text", {"tag3", "tag4"});

    StoryBoard my_board;
    EXPECT_NO_THROW(my_board.addNote(my_note1));
    EXPECT_NO_THROW(my_board.addNote(my_note2));
    EXPECT_EQ(my_board.length(), 2);

    // This should not remove anything
    EXPECT_NO_THROW(my_board.deleteNote(Note("note3", "text", {"tag3", "tag4"})));
    EXPECT_EQ(my_board.length(), 2);

    // This should not remove anything
    EXPECT_NO_THROW(my_board.deleteNote(Note("note2", "text2", {"tag3", "tag4"})));
    EXPECT_EQ(my_board.length(), 2);

    // This should not remove anything
    EXPECT_NO_THROW(my_board.deleteNote(Note("note2", "text", {"tag1", "tag4"})));
    EXPECT_EQ(my_board.length(), 2);

    // This should not remove anything
    EXPECT_NO_THROW(my_board.deleteNote(Note("note2", "text", {"tag3", "tag2"})));
    EXPECT_EQ(my_board.length(), 2);

    // This should remove my_note2 from the storyboard
    EXPECT_NO_THROW(my_board.deleteNote(my_note2));
    EXPECT_EQ(my_board.length(), 1);

    // This should remove my_note1 from the storyboard
    EXPECT_NO_THROW(my_board.deleteNote(my_note1));
    EXPECT_EQ(my_board.length(), 0);
}

TEST_F(QueryTestCppAPI, board_search_by_title)
{
    note_cont_t query_result;

    int nr_results = 0;
    EXPECT_NO_THROW(nr_results = my_board.searchByTitle("title1", query_result));
    EXPECT_EQ(nr_results, 1);
    EXPECT_EQ(query_result.at(0).getTitle(), "title1");
    EXPECT_EQ(query_result.at(0).getText(), "text");
    EXPECT_EQ(query_result.at(0).getTags().at(0), "tag1");
    EXPECT_EQ(query_result.at(0).getTags().at(1), "tag2");
    EXPECT_EQ(query_result.at(0).getTags().at(2), "tag3");
}

TEST_F(QueryTestCppAPI, board_search_by_text)
{
    note_cont_t query_result;

    int nr_results = 0;
    EXPECT_NO_THROW(nr_results = my_board.searchByText("hello", query_result));
    EXPECT_EQ(nr_results, 1);
    EXPECT_EQ(query_result.at(0).getTitle(), "title2");
    EXPECT_EQ(query_result.at(0).getText(), "text hei hello hola");
    EXPECT_EQ(query_result.at(0).getTags().at(0), "tag1");
    EXPECT_EQ(query_result.at(0).getTags().at(1), "tag4");
    EXPECT_EQ(query_result.at(0).getTags().at(2), "tag5");

    query_result.clear();
    EXPECT_NO_THROW(nr_results = my_board.searchByText("text", query_result));
    EXPECT_EQ(nr_results, 2);
    EXPECT_EQ(query_result.at(0).getTitle(), "title1");
    EXPECT_EQ(query_result.at(0).getText(), "text");
    EXPECT_EQ(query_result.at(0).getTags().at(0), "tag1");
    EXPECT_EQ(query_result.at(0).getTags().at(1), "tag2");
    EXPECT_EQ(query_result.at(0).getTags().at(2), "tag3");
    EXPECT_EQ(query_result.at(1).getTitle(), "title2");
    EXPECT_EQ(query_result.at(1).getText(), "text hei hello hola");
    EXPECT_EQ(query_result.at(1).getTags().at(0), "tag1");
    EXPECT_EQ(query_result.at(1).getTags().at(1), "tag4");
    EXPECT_EQ(query_result.at(1).getTags().at(2), "tag5");

    query_result.clear();
    EXPECT_NO_THROW(nr_results = my_board.searchByText("hej", query_result));
    EXPECT_EQ(nr_results, 0);
}

TEST_F(QueryTestCppAPI, board_search_by_tag)
{
    note_cont_t query_result;

    int nr_results = 0;
    EXPECT_NO_THROW(nr_results = my_board.searchByTag("tag2", query_result));
    EXPECT_EQ(nr_results, 1);
    EXPECT_EQ(query_result.at(0).getTitle(), "title1");
    EXPECT_EQ(query_result.at(0).getText(), "text");
    EXPECT_EQ(query_result.at(0).getTags().at(0), "tag1");
    EXPECT_EQ(query_result.at(0).getTags().at(1), "tag2");
    EXPECT_EQ(query_result.at(0).getTags().at(2), "tag3");

    query_result.clear();
    EXPECT_NO_THROW(nr_results = my_board.searchByTag("tag1", query_result));
    EXPECT_EQ(nr_results, 2);
    EXPECT_EQ(query_result.at(0).getTitle(), "title1");
    EXPECT_EQ(query_result.at(0).getText(), "text");
    EXPECT_EQ(query_result.at(0).getTags().at(0), "tag1");
    EXPECT_EQ(query_result.at(0).getTags().at(1), "tag2");
    EXPECT_EQ(query_result.at(0).getTags().at(2), "tag3");
    EXPECT_EQ(query_result.at(1).getTitle(), "title2");
    EXPECT_EQ(query_result.at(1).getText(), "text hei hello hola");
    EXPECT_EQ(query_result.at(1).getTags().at(0), "tag1");
    EXPECT_EQ(query_result.at(1).getTags().at(1), "tag4");
    EXPECT_EQ(query_result.at(1).getTags().at(2), "tag5");

    query_result.clear();
    EXPECT_NO_THROW(nr_results = my_board.searchByTag("tag", query_result));
    EXPECT_EQ(nr_results, 0);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
