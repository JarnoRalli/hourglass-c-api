#include <algorithm>
#include <iostream>

#include <storyboard/storyboard.hpp>

namespace storyboard {

tag_cont_t& Note::getTags()
{
    return m_tags;
}

std::string& Note::getTitle()
{
    return m_title;
}

std::string& Note::getText()
{
    return m_text;
}

bool Note::operator==(const Note& other)
{
    return (m_title == other.m_title) && (m_text == other.m_text) && (m_tags == other.m_tags);
}

void Storyboard::addNote(const Note& newNote)
{
    m_notes.push_back(newNote);
}

void Storyboard::addNote(Note&& newNote)
{
    m_notes.push_back(std::move(newNote));
}

int Storyboard::deleteNote(const Note& deleteMe)
{
    int nr_elem = m_notes.size();
    m_notes.erase(std::remove(m_notes.begin(), m_notes.end(), deleteMe), m_notes.end());
    return nr_elem - m_notes.size();
}

int Storyboard::searchByTitle(const std::string& title, note_cont_t& container)
{
    int len = container.size();
    std::copy_if(m_notes.begin(), m_notes.end(), std::back_inserter(container),
                 [&title](const Note& cmp) { return cmp.m_title == title; });

    return container.size() - len;
}

int Storyboard::searchByText(const std::string& text, note_cont_t& container)
{
    int len = container.size();
    std::copy_if(m_notes.begin(), m_notes.end(), std::back_inserter(container),
                 [&text](const Note& cmp) { return cmp.m_text.find(text) != std::string::npos; });

    return container.size() - len;
}

int Storyboard::searchByTag(const tag_cont_t& tags, note_cont_t& container)
{
    int len = container.size();
    std::copy_if(m_notes.begin(), m_notes.end(), std::back_inserter(container), [&tags](const Note& cmp) {
        return std::any_of(tags.begin(), tags.end(), [&cmp](const std::string& str) {
            return std::find(cmp.m_tags.begin(), cmp.m_tags.end(), str) != cmp.m_tags.end();
        });
    });

    return container.size() - len;
}

int Storyboard::length()
{
    return m_notes.size();
}

}  // End of namespace storyboard
