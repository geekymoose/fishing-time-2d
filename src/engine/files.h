#pragma once

/// Reads the content of a file.
/// The content is allocated on the heap and must be free.
/// @param _path File path
/// @return Byte buffer with the file's content or NULL if error.
void* newReadFileContent(const char* _path);
