#pragma once

#include "import_export.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------
// --- Error handling ---
//-----------------------
typedef struct error* error_t_;

/**
 * @brief Returns error message of the @error_t_ object
 * @param[in, out] out_error : error object
 * @return Pointer to the error message
 */
STORYBOARD_EXPORT
const char* error_message(error_t_ error);

/**
 * @brief Deletes the error object
 * @param[in, out] out_error : error object
 * @return nullptr
 */
STORYBOARD_EXPORT
error_t_ error_destruct(error_t_ error);

/**
 * @brief Returns error message of the @error_t_ object
 * @param[in, out] out_error : error object
 * @return Pointer to the error message
 */
STORYBOARD_EXPORT
const char* error_get_string(error_t_ error);

//-------------
// --- Note ---
//-------------
typedef struct note* note_t;

/**
 * @brief Constructs a new note
 * @param[in] title : Title of the note
 * @param[in] text : Text of the note
 * @param[in] tags : Tags for the note
 * @param[in] nr_tags : Number of tags being passed
 * @param[in, out] out_error : error object
 * @return a new @note_t object
 */
STORYBOARD_EXPORT
note_t note_construct(const char* title, const char* text, const char* tags[], int32_t nr_tags, error_t_* out_error);

/**
 * @brief Destructs a note object
 * @param[in] note_in : a note that is being destructed
 * @return nullptr
 */
STORYBOARD_EXPORT
note_t note_destruct(note_t note_in);

/**
 * @brief Copies a note
 * @param[in] note_in : a note that is copied
 * @param[in, out] out_error : error object
 * @return A copy of the note
 */
STORYBOARD_EXPORT
note_t note_copy(const note_t note_in, error_t_* out_error);

/**
 * @brief Get note's title
 * @param[in] note_in : note being queried
 * @param[in, out] out_error : error object
 * @return
 */
STORYBOARD_EXPORT
const char* note_get_title(const note_t note_in, error_t_* out_error);

/**
 * @brief Get note's title
 * @param[in] note_in : note being queried
 * @param[in, out] out_error : error object
 * @return
 */
STORYBOARD_EXPORT
const char* note_get_text(const note_t note_in, error_t_* out_error);

/**
 * @brief Note query handler type. Used for querying note's tags.
 * @param[in] client_data : void pointer to client data object that is passed to the handler
 * @param[in] tag : Tag of the note
 */
typedef void (*note_query_handler)(void* client_data, const char* tag);

/**
 * @brief Get note's tags
 * @details The handler function pointer is called for each tag in the note_in.
 * @param[in] note_in : note being queried
 * @param[in] handler : query handler function
 * @param[in] client_data : void pointer passed to the handler function
 * @param[in, out] out_error : error object
 * @return
 */
STORYBOARD_EXPORT
int32_t note_get_tags(const note_t note_in, note_query_handler handler, void* client_data, error_t_* out_error);

//-------------------
// --- Storyboard ---
//-------------------
typedef struct board* board_t;

/**
 * @brief Constructs a storyboard object
 * @param[in, out] out_error : error object
 * @return new board
 */
STORYBOARD_EXPORT
board_t storyboard_construct(error_t_* out_error);

/**
 * @brief Destructs a storyboard object
 * @param[in] board_in : board to be destructed
 * @return nullptr
 */
STORYBOARD_EXPORT
board_t storyboard_destruct(board_t board_in);

/**
 * @brief Copies a board object
 * @param[in] board_in : board that is copied
 * @param[in, out] out_error : error object
 * @return a copy of the board
 */
STORYBOARD_EXPORT
board_t storyboard_copy(const board_t board_in, error_t_* out_error);

/**
 * @brief Adds a note to a board
 * @param[in] board_in : board where the note is added
 * @param[in] note_in : note that is added to the board
 * @param[in, out] out_error : error object
 * @return
 */
STORYBOARD_EXPORT
void storyboard_add_note(board_t board_in, const note_t note_in, error_t_* out_error);

/**
 * @brief Deletes a note
 * @param[in] board_in : board from which the note is deleted
 * @param[in] note_in : an example of a note that is deleted
 * @param[in, out] out_error : error object
 * @return
 */
STORYBOARD_EXPORT
int32_t storyboard_delete_note(board_t board_in, const note_t note_in, error_t_* out_error);

/**
 * @brief Storyboard query handler type
 * @param[in] client_data : void pointer to client data object that is passed to the handler
 * @param[in] title : Title of the note
 * @param[in] text : Text of the note
 * @param[in] tags : Tags of the note
 * @param[in] nr_tags : Number of tags that the note contains
 */
typedef void (*storyboard_query_handler)(void* client_data, const char* title, const char* text, const char* tags[],
                                         int32_t nr_tags);

/**
 * @brief Search notes based on the title
 * @param[in] board_in : board that is being queried
 * @param[in] title : title/name being queried
 * @param[in] handler : query handler
 * @param[in] client_data : client data that is passed to the query handler
 * @param[in, out] out_error : error object
 * @return
 */
STORYBOARD_EXPORT
int32_t storyboard_search_by_title(const board_t board_in, const char* title, storyboard_query_handler handler,
                                   void* client_data, error_t_* out_error);

/**
 * @brief Search notes based on the text
 * @param[in] board_in : board that is being queried
 * @param[in] text : text being queried
 * @param[in] handler : query handler
 * @param[in] client_data : client data that is passed to the query handler
 * @param[in, out] out_error : error object
 * @return
 */
STORYBOARD_EXPORT
int32_t storyboard_search_by_text(const board_t board_in, const char* text, storyboard_query_handler handler,
                                  void* client_data, error_t_* out_error);

/**
 * @brief Search notes based on the tag
 * @param[in] board_in : board that is being queried
 * @param[in] tag : tag being queried
 * @param[in] handler : query handler
 * @param[in] client_data : client data that is passed to the query handler
 * @param[in, out] out_error : error object
 * @return
 */
STORYBOARD_EXPORT
int32_t storyboard_search_by_tag(const board_t board_in, const char* tag, storyboard_query_handler handler,
                                 void* client_data, error_t_* out_error);

/**
 * @brief Number of notes that the board contains
 * @param[in] board_in : board that is being queried
 * @param[in, out] out_error : error object
 * @return number of notes that the board has
 */
STORYBOARD_EXPORT
int32_t storyboard_get_nr_notes(const board_t board_in, error_t_* out_error);

#ifdef __cplusplus
}  // End of extern "C"
#endif
