#include "engine/fonts.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "engine/log.h"


FT_Library s_ftLibrary;


// Assumes W is never bigger than font size H
// Use pixel size == 1 Byte
Font * loadFontFromFile(const char * path, int fontSixeInPx, int charStart, int charEnd)
{
    FT_Face face;
    FT_Error error;

    error = FT_New_Face(s_ftLibrary, path, 0, &face);
    if(error == FT_Err_Unknown_File_Format)
    {
        ASSERT_MSG(FALSE, "[Font] Unsupported font format");
        LOG_ERR("[Font] Unsupported font format in %s (FT_Error %d)", path, error);
        return NULL;
    }
    else if(error != 0)
    {
        ASSERT_MSG(FALSE, "[Font] Unable to load font face");
        LOG_ERR("[Font] Unable to load font face for %s", path);
        return NULL;
    }

    error = FT_Set_Pixel_Sizes(face, 0, fontSixeInPx);
    if(error != 0)
    {
        ASSERT_MSG(FALSE, "[Font] Unable to set the font size");
        LOG_ERR(FALSE, "[Font] Unable to set the font size to %d", fontSixeInPx);
        return NULL;
    }

    const int nbChars = charEnd - charStart + 1; // +1 because 'a' - 'a' = 0
    if(nbChars <= 0)
    {
        ASSERT_MSG(FALSE, "[Font] Invalid range of characters");
        LOG_ERR(FALSE, "[Font] Invalid range of characters (%d to %d)", charStart, charEnd);
        return NULL;
    }

    // Each glyph is placed in a square box with arbitrary dimension w=h=fontsize+1 (all boxes are the same)
    // We add an empty row and column to devide glyphs in the bitmap (prevents shaders texture approx)
    const size_t rectBoxSizeInPixels = (fontSixeInPx + 1) * (fontSixeInPx + 1);
    const size_t pixelSizeInBytes = sizeof(unsigned char); // Assumes 1 Pixel = 1 Byte
    const size_t bufferSizeInBytes = nbChars * rectBoxSizeInPixels * pixelSizeInBytes;
    const size_t bufferWidthInBytes = (fontSixeInPx + 1) * pixelSizeInBytes;

    unsigned char * bitmapBuffer = malloc(bufferSizeInBytes);
    if(bitmapBuffer == NULL)
    {
        ASSERT_MSG(FALSE, "[Font] Unable to malloc");
        LOG_ERR(FALSE, "[Font] Unable to malloc %d bytes", bufferSizeInBytes);
        return NULL;
    }
    memset(bitmapBuffer, 0, bufferSizeInBytes);

    Font * font = malloc(sizeof(Font));
    if(font == NULL)
    {
        ASSERT_MSG(FALSE, "[Font] Unable to malloc");
        LOG_ERR(FALSE, "[Font] Unable to malloc %d Bytes", sizeof(Font));
        return NULL;
    }
    memset(font, 0, sizeof(Font));

    font->glyphs = malloc(nbChars * sizeof(FontGlyph));
    if(font->glyphs == NULL)
    {
        ASSERT_MSG(FALSE, "[Font] Unable to malloc");
        LOG_ERR(FALSE, "[Font] Unable to malloc %d Bytes", sizeof(FontGlyph));
        return NULL;
    }

    unsigned char * codepointBufferPtr = bitmapBuffer;

    for(int codepoint = charStart; codepoint <= charEnd; ++codepoint)
    {
        FT_UInt glyph_index = FT_Get_Char_Index(face, codepoint);
        FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

        const int bmpWidthInBytes = face->glyph->bitmap.width * pixelSizeInBytes;
        LOG_DBG("codepoint %d / width: %d / index: %d", codepoint, face->glyph->bitmap.width, glyph_index);
        ASSERT_MSG(bmpWidthInBytes != 0, "[Font] FT_Render_Glyph generated a bitmap with invalid width=0");

        const float uvX = 0.0f;
        const float uvY = 0.0f;
        font->glyphs[codepoint].uvX = uvX;
        font->glyphs[codepoint].uvY = uvY;

        for(int rowY = 0; rowY < face->glyph->bitmap.rows; ++rowY)
        {
            const void * bufferRowSrc = face->glyph->bitmap.buffer + (bmpWidthInBytes * rowY);
            void * bufferRowDst = codepointBufferPtr + (bufferWidthInBytes * rowY);

            memcpy(bufferRowDst, bufferRowSrc, bmpWidthInBytes);
        }

        codepointBufferPtr += rectBoxSizeInPixels * pixelSizeInBytes; // Advance to next outlineBox
    }

    // TODO: tmp debug
    stbi_write_png("font_generated_bitmap.png", (fontSixeInPx+1), (fontSixeInPx+1) * nbChars, 1, bitmapBuffer, 0);

    free(bitmapBuffer);
    FT_Done_Face(face);

    return font;
}

int initFontLibrary()
{
    FT_Error error = FT_Init_FreeType(&s_ftLibrary);
    if(error)
    {
        LOG_ERR("[Font] Unable to load the font library (FT_Error %d)", error);
        return error;
    }

    LOG_INFO("[Font] Font library successfully loaded");
    return error;
}

int destroyFontLibrary()
{
    FT_Error error = FT_Done_FreeType(s_ftLibrary);
    if(error)
    {
        LOG_ERR("[Font] Unable to destroy the font library (FT_Error %d)", error);
        return error;
    }

    LOG_INFO("[Font] Font library successfully destroyed");

    return error;
}

