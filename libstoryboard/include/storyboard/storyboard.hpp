#pragma once

#include <string>
#include <vector>

namespace storyboard {

class Storyboard;
class Note;

using tag_cont_t  = std::vector<std::string>;
using note_cont_t = std::vector<Note>;

/**
 * @class Note
 * @brief A Note object that has a title, text and a container that can have several tags.
 */
class Note
{
public:
    /**
     * @brief Simple constructor
     */
    Note() : m_title(""), m_text("")
    {
    }

    /**
     * @brief Constructor with parameters
     * @param[in] title : Note's title
     * @param[in] text : Note's text
     * @param[in] tags : Note's tags
     */
    Note(const std::string& title, const std::string& text, tag_cont_t const& tags)
        : m_title(title), m_text(text), m_tags(tags)
    {
    }

    /**
     * @brief Destructor
     */
    virtual ~Note()
    {
    }

    /**
     * @brief Returns a container with the note's tags
     * @return std-container that contains the Note's tags
     */
    tag_cont_t& getTags();

    /**
     * @brief Returns Note's title
     * @return String containing the note's title
     */
    std::string& getTitle();

    /**
     * @brief Returns Note's text
     * @return String containing the note's text
     */
    std::string& getText();

    /**
     * @brief operator==
     * @param[in] other : Compared object
     * @return true if contents of this object are the same as those of the input object, otherwise return false
     */
    bool operator==(const Note& other);

    friend class Storyboard;

protected:
    std::string m_title;  /// Title of the note-object
    std::string m_text;   /// Text of the note-object
    tag_cont_t  m_tags;   /// Tag(s) of the note-object
};

/**
 * @class Storyboard
 * @brief Storyboard is an object that contains  one or more Note objects.
 * @details Has functionality for adding, deleting and searching for Note objects.
 */
class Storyboard
{
public:
    /**
     * @brief Adds a new note object into the Storyboard
     * @param[in] : newNote note to be added into the Storyboard
     */
    void addNote(const Note& newNote);

    /**
     * @brief Adds a new note object into the Storyboard. Uses move semantics (i.e. steals the resources from the given
     * object)
     * @param[in] : newNote note to be added into the Storyboard
     */
    void addNote(Note&& newNote);

    /**
     * @brief Removes a note from the Storyboard
     * @param[in] deleteMe : Sample of a Note object to be deleted from the Storyboard. Note objects that are equal to
     * deleteMe will be deleted.
     * @return number of elements that were deleted
     */
    int deleteNote(const Note& deleteMe);

    /**
     * @brief Search the Storyboard for notes that contain the given string in the title field
     * @param[in] title : string that is matched
     * @param[in,out] container : notes in the Storyboard that contain the matched field are added into this container
     * @return number of search results
     */
    int searchByTitle(const std::string& title, note_cont_t& container);

    /**
     * @brief Search the Storyboard container for notes that contain the given string in the text field
     * @param[in] text : string that is matched
     * @param[in,out] container : notes in the Storyboard that contain the matched field are added into this container
     * @return number of search results
     */
    int searchByText(const std::string& text, note_cont_t& container);

    /**
     * @brief Search the Storyboard container for notes that contain the given string(s) in the tag field
     * @param[in] tags : a vector of tags that are matched
     * @param[in,out] container : notes in the Storyboard that contain the matched field are added into this container
     * @return number of search results
     */
    int searchByTag(const tag_cont_t& tags, note_cont_t& container);

    /**
     * @brief Number of notes stored
     */
    int length();

private:
    note_cont_t m_notes;  /// Container of Note objects
};

}  // End of namespace storyboard
