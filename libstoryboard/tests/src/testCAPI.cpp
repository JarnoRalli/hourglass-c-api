#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "storyboard/storyboardCAPI.h"

class QueryTestCAPI : public ::testing::Test
{
protected:
    void SetUp() override
    {
        EXPECT_NO_THROW(my_note1 = note_construct("title1", "text", tags1, NR_TAGS, &my_error));
        ASSERT_EQ(my_error, nullptr);
        ASSERT_NE(my_note1, nullptr);

        EXPECT_NO_THROW(my_note2 = note_construct("title2", "text hei hello hola", tags2, NR_TAGS, &my_error));
        ASSERT_EQ(my_error, nullptr);
        ASSERT_NE(my_note2, nullptr);

        EXPECT_NO_THROW(my_board = storyboard_construct(&my_error));
        ASSERT_EQ(my_error, nullptr);
        ASSERT_NE(my_board, nullptr);

        EXPECT_NO_THROW(storyboard_add_note(my_board, my_note1, &my_error));
        ASSERT_EQ(my_error, nullptr);

        EXPECT_NO_THROW(storyboard_add_note(my_board, my_note2, &my_error));
        ASSERT_EQ(my_error, nullptr);
    }

    void TearDown() override
    {
        storyboard_destruct(my_board);
    }

    static const int32_t NR_TAGS        = 3;
    const char*          tags1[NR_TAGS] = {"t1", "t2", "t3"};
    const char*          tags2[NR_TAGS] = {"t1", "t4", "t5"};

    note_t   my_note1 = nullptr;
    note_t   my_note2 = nullptr;
    board_t  my_board = nullptr;
    error_t_ my_error = nullptr;
};

const int32_t QueryTestCAPI::NR_TAGS;

TEST(CAPI, construct_and_destruct_note)
{
    note_t   my_note  = nullptr;
    error_t_ my_error = nullptr;

    const int32_t NR_TAGS       = 3;
    const char*   tags[NR_TAGS] = {"t1", "t2", "t3"};

    EXPECT_NO_THROW(my_note = note_construct("note", "text", tags, NR_TAGS, &my_error));
    ASSERT_EQ(my_error, nullptr);
    ASSERT_NE(my_note, nullptr);

    // --- Query title ---
    ASSERT_EQ(std::string(note_get_title(my_note, &my_error)), "note");
    ASSERT_EQ(my_error, nullptr);

    // --- Query text ---
    ASSERT_EQ(std::string(note_get_text(my_note, &my_error)), "text");
    ASSERT_EQ(my_error, nullptr);

    // --- Query tags ---
    typedef std::vector<std::string> tag_t;
    tag_t                            tags_container;

    // A function that is called for each element in the note tag container
    auto callback = [](void* client_data, const char* tag) { ((tag_t*)client_data)->push_back(std::string(tag)); };

    EXPECT_NO_THROW(note_get_tags(my_note, callback, &tags_container, &my_error));
    ASSERT_EQ(my_error, nullptr);
    ASSERT_EQ(tags_container.size(), 3);
    ASSERT_EQ(tags_container.at(0), "t1");
    ASSERT_EQ(tags_container.at(1), "t2");
    ASSERT_EQ(tags_container.at(2), "t3");

    EXPECT_NO_THROW(my_note = note_destruct(my_note));
    ASSERT_EQ(my_note, nullptr);
}

TEST(CAPI, construct_and_destruct_board)
{
    board_t  my_board = nullptr;
    error_t_ my_error = nullptr;

    EXPECT_NO_THROW(my_board = storyboard_construct(&my_error));
    ASSERT_EQ(my_error, nullptr);
    ASSERT_NE(my_board, nullptr);

    EXPECT_NO_THROW(my_board = storyboard_destruct(my_board));
    ASSERT_EQ(my_board, nullptr);
}

TEST(CAPI, board_add_note)
{
    note_t   my_note  = nullptr;
    board_t  my_board = nullptr;
    error_t_ my_error = nullptr;

    EXPECT_NO_THROW(my_note = note_construct("a", "b", nullptr, 0, &my_error));
    ASSERT_EQ(my_error, nullptr);
    ASSERT_NE(my_note, nullptr);

    EXPECT_NO_THROW(my_board = storyboard_construct(&my_error));
    ASSERT_EQ(my_error, nullptr);
    ASSERT_NE(my_board, nullptr);

    EXPECT_NO_THROW(storyboard_add_note(my_board, nullptr, &my_error));
    ASSERT_NE(my_error, nullptr);
    my_error = error_destruct(my_error);

    EXPECT_NO_THROW(storyboard_add_note(nullptr, my_note, &my_error));
    ASSERT_NE(my_error, nullptr);
    my_error = error_destruct(my_error);

    EXPECT_NO_THROW(storyboard_add_note(my_board, my_note, &my_error));
    ASSERT_EQ(my_error, nullptr);
    ASSERT_EQ(storyboard_get_nr_notes(my_board, &my_error), 1);
    ASSERT_EQ(my_error, nullptr);

    error_destruct(my_error);
    note_destruct(my_note);
    storyboard_destruct(my_board);
}

TEST_F(QueryTestCAPI, board_search_by_title)
{
    // A struct where the results are stored
    struct Note
    {
        std::string              name;
        std::string              text;
        std::vector<std::string> tags;
    };

    typedef std::vector<Note> query_result_t;
    query_result_t            query_result;

    // A callback function that is called for each query result
    auto callback = [](void* client_data, const char* name, const char* text, const char* tags[], int32_t nr_tags) {
        std::vector<std::string> tags_vector;

        for (size_t index = 0; index < nr_tags; index++)
        {
            tags_vector.push_back(std::string(tags[index]));
        }

        ((query_result_t*)client_data)->push_back(Note{std::string(name), std::string(text), tags_vector});
    };

    EXPECT_NO_THROW(storyboard_search_by_title(my_board, "title1", callback, &query_result, &my_error));
    ASSERT_EQ(my_error, nullptr);
    ASSERT_EQ(query_result.size(), 1);
    ASSERT_EQ(query_result.at(0).name, "title1");
    ASSERT_EQ(query_result.at(0).text, "text");
    ASSERT_EQ(query_result.at(0).tags.size(), NR_TAGS);
    ASSERT_EQ(query_result.at(0).tags.at(0), "t1");
    ASSERT_EQ(query_result.at(0).tags.at(1), "t2");
    ASSERT_EQ(query_result.at(0).tags.at(2), "t3");
}

TEST_F(QueryTestCAPI, board_search_by_text)
{
    // A struct where the results are stored
    struct Note
    {
        std::string              name;
        std::string              text;
        std::vector<std::string> tags;
    };

    typedef std::vector<Note> query_result_t;
    query_result_t            query_result;

    // A callback function that is called for each query result
    auto callback = [](void* client_data, const char* name, const char* text, const char* tags[], int32_t nr_tags) {
        std::vector<std::string> tags_vector;

        for (size_t index = 0; index < nr_tags; index++)
        {
            tags_vector.push_back(std::string(tags[index]));
        }

        ((query_result_t*)client_data)->push_back(Note{std::string(name), std::string(text), tags_vector});
    };

    EXPECT_NO_THROW(storyboard_search_by_text(my_board, "hello", callback, &query_result, &my_error));
    ASSERT_EQ(my_error, nullptr);
    ASSERT_EQ(query_result.size(), 1);
    ASSERT_EQ(query_result.at(0).name, "title2");
    ASSERT_EQ(query_result.at(0).text, "text hei hello hola");
    ASSERT_EQ(query_result.at(0).tags.size(), NR_TAGS);
    ASSERT_EQ(query_result.at(0).tags.at(0), "t1");
    ASSERT_EQ(query_result.at(0).tags.at(1), "t4");
    ASSERT_EQ(query_result.at(0).tags.at(2), "t5");

    query_result.clear();
    EXPECT_NO_THROW(storyboard_search_by_text(my_board, "text", callback, &query_result, &my_error));
    ASSERT_EQ(my_error, nullptr);
    ASSERT_EQ(query_result.size(), 2);
    ASSERT_EQ(query_result.at(0).name, "title1");
    ASSERT_EQ(query_result.at(0).text, "text");
    ASSERT_EQ(query_result.at(0).tags.size(), NR_TAGS);
    ASSERT_EQ(query_result.at(0).tags.at(0), "t1");
    ASSERT_EQ(query_result.at(0).tags.at(1), "t2");
    ASSERT_EQ(query_result.at(0).tags.at(2), "t3");
    ASSERT_EQ(query_result.at(1).name, "title2");
    ASSERT_EQ(query_result.at(1).text, "text hei hello hola");
    ASSERT_EQ(query_result.at(1).tags.size(), NR_TAGS);
    ASSERT_EQ(query_result.at(1).tags.at(0), "t1");
    ASSERT_EQ(query_result.at(1).tags.at(1), "t4");
    ASSERT_EQ(query_result.at(1).tags.at(2), "t5");

    query_result.clear();
    EXPECT_NO_THROW(storyboard_search_by_text(my_board, "hej", callback, &query_result, &my_error));
    ASSERT_EQ(my_error, nullptr);
    ASSERT_EQ(query_result.size(), 0);
}

TEST_F(QueryTestCAPI, board_search_by_tag)
{
    // A struct where the results are stored
    struct Note
    {
        std::string              name;
        std::string              text;
        std::vector<std::string> tags;
    };

    typedef std::vector<Note> query_result_t;
    query_result_t            query_result;

    // A callback function that is called for each query result
    auto callback = [](void* client_data, const char* name, const char* text, const char* tags[], int32_t nr_tags) {
        std::vector<std::string> tags_vector;

        for (size_t index = 0; index < nr_tags; index++)
        {
            tags_vector.push_back(std::string(tags[index]));
        }

        ((query_result_t*)client_data)->push_back(Note{std::string(name), std::string(text), tags_vector});
    };

    EXPECT_NO_THROW(storyboard_search_by_tag(my_board, "t2", callback, &query_result, &my_error));
    ASSERT_EQ(my_error, nullptr);
    ASSERT_EQ(query_result.size(), 1);
    ASSERT_EQ(query_result.at(0).name, "title1");
    ASSERT_EQ(query_result.at(0).text, "text");
    ASSERT_EQ(query_result.at(0).tags.size(), NR_TAGS);
    ASSERT_EQ(query_result.at(0).tags.at(0), "t1");
    ASSERT_EQ(query_result.at(0).tags.at(1), "t2");
    ASSERT_EQ(query_result.at(0).tags.at(2), "t3");

    query_result.clear();
    EXPECT_NO_THROW(storyboard_search_by_tag(my_board, "t1", callback, &query_result, &my_error));
    ASSERT_EQ(my_error, nullptr);
    ASSERT_EQ(query_result.size(), 2);
    ASSERT_EQ(query_result.at(0).name, "title1");
    ASSERT_EQ(query_result.at(0).text, "text");
    ASSERT_EQ(query_result.at(0).tags.size(), NR_TAGS);
    ASSERT_EQ(query_result.at(0).tags.at(0), "t1");
    ASSERT_EQ(query_result.at(0).tags.at(1), "t2");
    ASSERT_EQ(query_result.at(0).tags.at(2), "t3");
    ASSERT_EQ(query_result.at(1).name, "title2");
    ASSERT_EQ(query_result.at(1).text, "text hei hello hola");
    ASSERT_EQ(query_result.at(1).tags.size(), NR_TAGS);
    ASSERT_EQ(query_result.at(1).tags.at(0), "t1");
    ASSERT_EQ(query_result.at(1).tags.at(1), "t4");
    ASSERT_EQ(query_result.at(1).tags.at(2), "t5");

    query_result.clear();
    EXPECT_NO_THROW(storyboard_search_by_tag(my_board, "tag", callback, &query_result, &my_error));
    ASSERT_EQ(my_error, nullptr);
    ASSERT_EQ(query_result.size(), 0);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
