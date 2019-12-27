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

// Assumes 1 Pixel == 1 Byte
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

    // Buffer size is arbitrary (square and big enough)
    const size_t bufferWidth = 512;
    const size_t bufferHeight = 512;
    const size_t bufferSize = bufferWidth * bufferHeight;

    unsigned char * bitmapBuffer = malloc(bufferSize);
    if(bitmapBuffer == NULL)
    {
        ASSERT_MSG(FALSE, "[Font] Unable to malloc");
        LOG_ERR(FALSE, "[Font] Unable to malloc %d bytes", bufferSize);
        return NULL;
    }
    memset(bitmapBuffer, 0, bufferSize);

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

    int currentX = 0;
    int currentY = 0;
    int gap = 10; // Arbitrary gap between letters

    for(int codepoint = charStart; codepoint <= charEnd; ++codepoint)
    {
        FT_UInt glyph_index = FT_Get_Char_Index(face, codepoint);
        FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

        int glyphWidth = face->glyph->bitmap.width;
        int glyphHeight = face->glyph->bitmap.rows;
        ASSERT_MSG(glyphWidth != 0, "FT_Render_Glyph generated a bitmap invalid width=0");
        ASSERT_MSG(glyphHeight != 0, "FT_Render_Glyph generated a bitmap invalid height=0");

        if(currentX + glyphWidth >= bufferWidth)
        {
            currentX = 0;
            currentY += fontSixeInPx + gap;
        }

        if(currentY + glyphHeight > bufferHeight)
        {
            // This means the buffer size is too small!
            LOG_ERR("[Font] The buffer size is to small to have all the requested glyphes");
            ASSERT_MSG(FALSE, "[Font] The buffer size is to small to have all the requested glyphes");
            break; // In release, if this appears, load glyphes until now
        }

        LOG_DBG("char: %c / w=%d / h=%d / currentX = %d / currentY = %d", codepoint, glyphWidth, glyphHeight, currentX, currentY);

        /*
        font->glyphs[codepoint].uvX0 = 0.0f;
        font->glyphs[codepoint].uvX1 = 0.0f;

        font->glyphs[codepoint].uvY0 = 0.0f;
        font->glyphs[codepoint].uvY1 = 0.0f;
        */

        for(int rowY = 0; rowY < face->glyph->bitmap.rows; ++rowY)
        {
            const void * bufferRowSrc = face->glyph->bitmap.buffer + (glyphWidth * rowY);
            void * bufferRowDst = bitmapBuffer + currentX + (currentY * bufferWidth) + (bufferWidth * rowY);

            memcpy(bufferRowDst, bufferRowSrc, glyphWidth);
        }

        currentX += glyphWidth + gap;
    }

    // DEBUG: uncomment to create a debug png with the generated glyphes bitmap
    stbi_write_png("font_generated_bitmap.png", bufferWidth, bufferHeight, 1, bitmapBuffer, 0);

    free(bitmapBuffer);
    FT_Done_Face(face);

    return font;
}

void destroyFont(Font * font)
{
    ASSERT_MSG(font != NULL, "[Font] Invalid parameter");

    if(font != NULL)
    {
        free(font->glyphs);
        free(font);
    }
}

