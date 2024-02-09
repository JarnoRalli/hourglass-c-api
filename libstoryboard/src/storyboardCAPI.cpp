#include "storyboard/storyboardCAPI.h"
#include "storyboard/storyboard.hpp"

#include <memory>
#include <algorithm>
#include <iostream>

//------------------
// --- C linkage ---
//------------------
extern "C" {

struct error
{
    std::string message;
};

const char* error_message(error_t_ error)
{
    return error->message.c_str();
}

error_t_ error_destruct(error_t_ error)
{
    if (error)
        delete error;

    return nullptr;
}

const char* error_get_string(error_t_ error)
{
    return error->message.c_str();
}

}  // End of extern "C"

//--------------------
// --- C++ linkage ---
//--------------------
template <typename Fn>
bool translateExceptions(error_t_* out_error, Fn&& fn)
{
    try
    {
        fn();
    }
    catch (const std::exception& e)
    {
        *out_error = new error{e.what()};
        return false;
    }
    catch (...)
    {
        *out_error = new error{"Unknown internal error"};
        return false;
    }
    return true;
}

struct note
{
    template <typename... Args>
    note(Args&&... args) : actual(std::forward<Args>(args)...)
    {
    }

    storyboard::Note actual;
};

struct board
{
    template <typename... Args>
    board(Args&&... args) : actual(std::forward<Args>(args)...)
    {
    }

    storyboard::Storyboard actual;
};

//------------------
// --- C linkage ---
//------------------
extern "C" {
note_t note_construct(const char* title, const char* text, const char* tags[], int32_t nr_tags, error_t_* out_error)
{
    note_t new_note = nullptr;

    storyboard::tag_cont_t tag_container;

    if (!title)
    {
        *out_error = new error{"title not initialized"};
        return nullptr;
    }

    if (!text)
    {
        *out_error = new error{"text not initialized"};
        return nullptr;
    }

    if ((!tags) && (nr_tags > 0))
    {
        *out_error = new error{"tags[] not initialized"};
        return nullptr;
    }

    if (tags)
    {
        for (std::size_t index = 0; index < nr_tags; index++)
        {
            tag_container.push_back(std::string(tags[index]));
        }
    }

    translateExceptions(out_error, [&] {
        new_note = std::make_unique<note>(std::string(title), std::string(text), tag_container).release();
    });

    return new_note;
}

note_t note_destruct(note_t note_in)
{
    if (note_in)
    {
        delete note_in;
    }

    return nullptr;
}

note_t note_copy(const note_t note_in, error_t_* out_error)
{
    if (!note_in)
    {
        *out_error = new error{"note_in not initialized"};
        return nullptr;
    }

    return std::make_unique<note>(note_in->actual).release();
}

const char* note_get_title(const note_t note_in, error_t_* out_error)
{
    if (!note_in)
    {
        *out_error = new error{"note_in not initialized"};
        return "";
    }

    return note_in->actual.getTitle().c_str();
}

const char* note_get_text(const note_t note_in, error_t_* out_error)
{
    if (!note_in)
    {
        *out_error = new error{"note_in not initialized"};
        return "";
    }

    return note_in->actual.getText().c_str();
}

int32_t note_get_tags(const note_t note_in, note_query_handler handler, void* client_data, error_t_* out_error)
{
    if (!note_in)
    {
        *out_error = new error{"note_in not initialized"};
        return 0;
    }

    translateExceptions(out_error, [&] {
        storyboard::tag_cont_t query_result(note_in->actual.getTags());

        for (auto&& elem : query_result)
        {
            handler(client_data, elem.c_str());
        }
    });

    return note_in->actual.getTags().size();
}

board_t storyboard_construct(error_t_* out_error)
{
    board_t new_storyboard = nullptr;
    translateExceptions(out_error, [&] { new_storyboard = std::make_unique<board>().release(); });

    return new_storyboard;
}

board_t storyboard_destruct(board_t board_in)
{
    if (board_in)
    {
        delete board_in;
    }

    return nullptr;
}

board_t storyboard_copy(const board_t board_in, error_t_* out_error)
{
    if (!board_in)
    {
        *out_error = new error{"board_in not initialized"};
        return nullptr;
    }

    return std::make_unique<board>(board_in->actual).release();
}

void storyboard_add_note(board_t board_in, const note_t note_in, error_t_* out_error)
{
    if (!board_in)
    {
        *out_error = new error{"board_in not initialized"};
        return;
    }

    if (!note_in)
    {
        *out_error = new error{"note_in not initialized"};
        return;
    }

    translateExceptions(out_error, [&] { board_in->actual.addNote(note_in->actual); });
}

int32_t storyboard_delete_note(board_t board_in, const note_t note_in, error_t_* out_error)
{
    int32_t nr_deleted_items = 0;

    if (!board_in)
    {
        *out_error = new error{"board_in not initialized"};
        return nr_deleted_items;
    }

    if (!note_in)
    {
        *out_error = new error{"note_in not initialized"};
        return nr_deleted_items;
    }

    translateExceptions(out_error, [&] { nr_deleted_items = board_in->actual.deleteNote(note_in->actual); });

    return nr_deleted_items;
}

int32_t storyboard_search_by_title(const board_t board_in, const char* title, storyboard_query_handler handler,
                                   void* client_data, error_t_* out_error)
{
    int nr_results = 0;

    if (!board_in)
    {
        *out_error = new error{"board_in not initialized"};
        return nr_results;
    }

    if (!title)
    {
        *out_error = new error{"name not initialized"};
        return nr_results;
    }

    translateExceptions(out_error, [&] {
        storyboard::note_cont_t query_result;
        board_in->actual.searchByTitle(std::string(title), query_result);

        nr_results = query_result.size();

        for (auto& elem : query_result)
        {
            // We need to return the tags, that are stored in std::vector<std::string>, in  const char**.
            // Therefore we gather the data from the tag-vector
            const char** tags = new const char*[elem.getTags().size()];

            for (size_t index = 0; index < elem.getTags().size(); index++)
            {
                tags[index] = elem.getTags().at(index).c_str();
            }

            // Call the handler function
            handler(client_data, elem.getTitle().c_str(), elem.getText().c_str(), tags, elem.getTags().size());

            // Delete the tags
            delete[] tags;
        }
    });

    return nr_results;
}

int32_t storyboard_search_by_text(const board_t board_in, const char* text, storyboard_query_handler handler,
                                  void* client_data, error_t_* out_error)
{
    int nr_results = 0;

    if (!board_in)
    {
        *out_error = new error{"board_in not initialized"};
        return nr_results;
    }

    if (!text)
    {
        *out_error = new error{"text not initialized"};
        return nr_results;
    }

    translateExceptions(out_error, [&] {
        storyboard::note_cont_t query_result;
        board_in->actual.searchByText(std::string(text), query_result);

        nr_results = query_result.size();

        for (auto& elem : query_result)
        {
            // We need to return the tags, that are stored in std::vector<std::string>, in  const char**.
            // Therefore we gather the data from the tag-vector
            const char** tags = new const char*[elem.getTags().size()];

            for (size_t index = 0; index < elem.getTags().size(); index++)
            {
                tags[index] = elem.getTags().at(index).c_str();
            }

            // Call the handler function
            handler(client_data, elem.getTitle().c_str(), elem.getText().c_str(), tags, elem.getTags().size());

            // Delete the tags
            delete[] tags;
        }
    });

    return nr_results;
}

int32_t storyboard_search_by_tag(const board_t board_in, const char* tag, storyboard_query_handler handler,
                                 void* client_data, error_t_* out_error)
{
    int nr_results = 0;

    if (!board_in)
    {
        *out_error = new error{"board_in not initialized"};
        return nr_results;
    }

    if (!tag)
    {
        *out_error = new error{"tag not initialized"};
        return nr_results;
    }

    translateExceptions(out_error, [&] {
        storyboard::note_cont_t query_result;
        storyboard::tag_cont_t  query;
        query.push_back(std::string(tag));

        board_in->actual.searchByTag(query, query_result);

        nr_results = query_result.size();

        for (auto& elem : query_result)
        {
            // We need to return the tags, that are stored in std::vector<std::string>, in  const char**.
            // Therefore we gather the data from the tag-vector
            const char** tags = new const char*[elem.getTags().size()];

            for (size_t index = 0; index < elem.getTags().size(); index++)
            {
                tags[index] = elem.getTags().at(index).c_str();
            }

            // Call the handler function
            handler(client_data, elem.getTitle().c_str(), elem.getText().c_str(), tags, elem.getTags().size());

            // Delete the tags
            delete[] tags;
        }
    });

    return nr_results;
}

int32_t storyboard_get_nr_notes(const board_t board_in, error_t_* out_error)
{
    int32_t length = -1;

    if (!board_in)
    {
        *out_error = new error{"board_in not initialized"};
    }

    translateExceptions(out_error, [&] { length = board_in->actual.length(); });

    return length;
}

}  // End of extern "C"
