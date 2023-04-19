#pragma once

#include <stdexcept>
#include <vector>
#include <string>

#include "storyboardCAPI.h"

class Note;

using tag_cont_t  = std::vector<std::string>;
using note_cont_t = std::vector<Note>;

/**
 * @class Error
 * @brief C API for error struct
 */
struct Error
{
    Error() : opaque(nullptr)
    {
    }

    ~Error()
    {
        if (opaque)
        {
            error_destruct(opaque);
        }
    }

    error_t_ opaque;
};

/**
 * @class ThrowOnError
 * @brief A class that converts an Error-object into a runtime exception
 */
class ThrowOnError
{
public:
    ~ThrowOnError() noexcept(false)
    {
        if (m_error.opaque)
        {
            throw std::runtime_error(error_message(m_error.opaque));
        }
    }

    operator error_t_*()
    {
        return &m_error.opaque;
    }

private:
    Error m_error;
};

/**
 * @class Note
 * @brief A header only C++ API, based on the C API, for the Note object
 */
class Note
{
public:
    /**
     * @brief Constructor
     */
    Note() : m_opaque(nullptr)
    {
    }

    /**
     * @brief Constructor with parameters
     * @param[in] title : Note's title
     * @param[in] text : Note's text
     * @param[in] tags : Note's tags
     */
    Note(const std::string& title, const std::string& text, tag_cont_t const& tags)
    {
        const char** temp = new const char*[tags.size()];

        for (std::size_t index = 0; index < tags.size(); index++)
        {
            temp[index] = tags.at(index).c_str();
        }

        m_opaque = note_construct(title.c_str(), text.c_str(), temp, tags.size(), ThrowOnError{});

        delete[] temp;
    }

    /**
     * @brief Copy constructor
     * @param[in] other : object being copied
     */
    Note(const Note& other)
    {
        this->operator=(other);
    }

    /**
     * @brief Move constructor
     * @param[in] other : object being moved
     */
    Note(Note&& other)
    {
        this->m_opaque = other.m_opaque;
        other.m_opaque = nullptr;
    }

    /**
     * @brief Destructor
     */
    virtual ~Note()
    {
        if (m_opaque)
        {
            note_destruct(m_opaque);
        }
    }

    /**
     * @brief Copy assignment operator
     * @param[in] other : object that is copied
     * @return this
     */
    Note& operator=(const Note& other)
    {
        this->m_opaque = note_copy(other.m_opaque, ThrowOnError{});
        return *this;
    }

    /**
     * @brief Returns a container with the note's tags
     * @return std-container that contains the note's tags
     */
    tag_cont_t getTags() const
    {
        tag_cont_t tags_container;

        auto callback = [](void* client_data, const char* tag) {
            ((tag_cont_t*)client_data)->push_back(std::string(tag));
        };

        note_get_tags(m_opaque, callback, &tags_container, ThrowOnError{});

        return tags_container;
    }

    /**
     * @brief Returns title
     * @return String containing the note's title
     */
    std::string getTitle() const
    {
        return note_get_title(m_opaque, ThrowOnError{});
    }

    /**
     * @brief Returns text
     * @return String containing the note's text
     */
    std::string getText() const
    {
        return note_get_text(m_opaque, ThrowOnError{});
    }

    friend class StoryBoard;

protected:
    note_t m_opaque;
};

/**
 * @class StoryBoard
 * @brief A header only C++ API, based on the C API, for the StoryBoard object
 */
class StoryBoard
{
public:
    /**
     * @brief Constructor
     */
    StoryBoard()
    {
        m_opaque = storyboard_construct(ThrowOnError{});
    }

    /**
     * @brief Copy constructor
     * @param[in] other : object being copied
     */
    StoryBoard(const StoryBoard& other)
    {
        this->operator=(other);
    }

    /**
     * @brief Move constuctor
     * @param[in, out] other : object being moved
     */
    StoryBoard(StoryBoard&& other)
    {
        this->m_opaque = other.m_opaque;
        other.m_opaque = nullptr;
    }

    /**
     * @brief Destructor
     */
    ~StoryBoard()
    {
        if (m_opaque)
        {
            m_opaque = storyboard_destruct(m_opaque);
        }
    }

    /**
     * @brief Copy assignment operator
     * @param[in] other : object that is copied
     * @return this
     */
    StoryBoard& operator=(const StoryBoard& other)
    {
        this->m_opaque = storyboard_copy(other.m_opaque, ThrowOnError{});
        return *this;
    }

    /**
     * @brief Adds a new note object into the Storyboard
     * @param[in] : newNote note to be added into the Storyboard
     */
    void addNote(const Note& newNote)
    {
        storyboard_add_note(m_opaque, newNote.m_opaque, ThrowOnError{});
    }

    /**
     * @brief Removes a note from the Storyboard
     * @param[in] deleteMe : Sample of a Note object to be deleted from the Storyboard. Note objects that are equal to
     * deleteMe will be deleted.
     * @return number of elements that were deleted
     */
    int deleteNote(const Note& deleteMe)
    {
        return storyboard_delete_note(m_opaque, deleteMe.m_opaque, ThrowOnError{});
    }

    /**
     * @brief Search the Storyboard for notes that contain the given string in the title field
     * @param[in] title : string that is matched
     * @param[in,out] container : notes in the Storyboard that contain the matched field are added into this container
     * @return number of results
     */
    int searchByTitle(const std::string& title, note_cont_t& container)
    {
        // A callback function that is called for each query result
        auto callback = [](void* client_data, const char* name, const char* text, const char* tags[], int32_t nr_tags) {
            std::vector<std::string> tags_vector;

            for (size_t index = 0; index < nr_tags; index++)
            {
                tags_vector.push_back(std::string(tags[index]));
            }

            ((note_cont_t*)client_data)->push_back(Note{std::string(name), std::string(text), tags_vector});
        };

        return storyboard_search_by_title(m_opaque, title.c_str(), callback, &container, ThrowOnError{});
    }

    /**
     * @brief Search the Storyboard container for notes that contain the given string in the text field
     * @param[in] text : string that is matched
     * @param[in,out] container : notes in the Storyboard that contain the matched field are added into this container
     * @return number of results
     */
    int searchByText(const std::string& text, note_cont_t& container)
    {
        // A callback function that is called for each query result
        auto callback = [](void* client_data, const char* name, const char* text, const char* tags[], int32_t nr_tags) {
            std::vector<std::string> tags_vector;

            for (size_t index = 0; index < nr_tags; index++)
            {
                tags_vector.push_back(std::string(tags[index]));
            }

            ((note_cont_t*)client_data)->push_back(Note{std::string(name), std::string(text), tags_vector});
        };

        return storyboard_search_by_text(m_opaque, text.c_str(), callback, &container, ThrowOnError{});
    }

    /**
     * @brief Search the Storyboard container for notes that contain the given string(s) in the tag field
     * @param[in] tag : string that is matched
     * @param[in,out] container : notes in the Storyboard that contain the matched field are added into this container
     * @return number of results
     */
    int searchByTag(const std::string& tag, note_cont_t& container)
    {
        // A callback function that is called for each query result
        auto callback = [](void* client_data, const char* name, const char* text, const char* tags[], int32_t nr_tags) {
            std::vector<std::string> tags_vector;

            for (size_t index = 0; index < nr_tags; index++)
            {
                tags_vector.push_back(std::string(tags[index]));
            }

            ((note_cont_t*)client_data)->push_back(Note{std::string(name), std::string(text), tags_vector});
        };

        return storyboard_search_by_tag(m_opaque, tag.c_str(), callback, &container, ThrowOnError{});
    }

    /**
     * @brief Number of notes stored
     * @return number of notes in the storyboard
     */
    int length()
    {
        return storyboard_get_nr_notes(m_opaque, ThrowOnError{});
    }

private:
    board_t m_opaque;
};
